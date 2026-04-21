/**
 * @file ebikeClient.cpp
 * @brief eBike client application with UDP socket communication.
 * Module: UFCFVK-15-2 Internet of Things
 */
#include <iomanip>
#include <string>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <atomic>
#include <csignal>
#include <ctime>
#include <cstring>
#include <iomanip>
#include <arpa/inet.h>

#include "util/MiscUtils.h"
#include "hal/CSVHALManager.h"
#include "hal/ISensor.h"
#include "sim/socket.h"
#include "sim/in.h"

namespace ebikeConstants {
    const std::string CONFIG_PATH = "config/client-config.yaml";
}

static std::atomic<bool> g_running{true};
void signalHandler(int) { g_running = false; }

std::string currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_info = std::localtime(&t);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return std::string(buf);
}

class GPSSensor : public ISensor {
public:
    explicit GPSSensor(int id) : sensorId_(id), halManager_(nullptr), port_(-1) {}
    ~GPSSensor() override { disconnect(); }
    int getId() const override { return sensorId_; }
    int getDimension() const override { return 2; }
    std::pair<std::string, std::string> format(std::vector<uint8_t> reading) override {
        std::string raw(reading.begin(), reading.end());
        double lat = 0.0, lon = 0.0;
        std::istringstream ss(raw);
        std::string token;
        int col = 0;
        while (std::getline(ss, token, ';')) {
            if (col == 0) lat = std::stod(token);
            if (col == 1) lon = std::stod(token);
            col++;
        }
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6);
        oss << "{\"lat\":" << lat << ",\"lon\":" << lon << "}";
        return {"gps", oss.str()};
    }
    bool connect(CSVHALManager* hal, int port, const std::string& csvFile = "") {
        if (!hal) return false;
        halManager_ = hal;
        port_ = port;
        halManager_->attachDevice(port_, std::shared_ptr<IDevice>(this, [](IDevice*){}));
        if (!csvFile.empty()) halManager_->initialise(csvFile);
        return true;
    }
    void disconnect() {
        if (halManager_ && port_ >= 0) {
            halManager_->releaseDevice(port_);
            halManager_ = nullptr;
            port_ = -1;
        }
    }
    std::string read() {
        if (!halManager_ || port_ < 0) return "";
        std::vector<uint8_t> raw = halManager_->read(port_);
        if (raw.empty()) return "";
        auto [key, val] = format(raw);
        return val;
    }
private:
    int sensorId_;
    CSVHALManager* halManager_;
    int port_;
};

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <client_ip> <ebike_id> <csv_file> <num_ports>\n";
        return 1;
    }

    std::string clientIp = argv[1];
    int ebikeId = std::atoi(argv[2]);
    std::string csvFile = argv[3];
    int numPorts = std::atoi(argv[4]);

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    int maxReadings = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "client", "max_readings"));
    int gpsSensorId = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "client", "gps_sensor_id"));
    std::string serverIp = readConfigValue(ebikeConstants::CONFIG_PATH, "server", "ip");
    int serverPort = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "server", "port"));
    int clientPort = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "client", "port"));

    std::string lockStatus = "unlocked";

    sim::set_ipaddr(clientIp.c_str());
    sim::socket clientSocket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, clientIp.c_str(), &clientAddr.sin_addr);
    clientAddr.sin_port = htons(clientPort);
    clientSocket.bind(clientAddr);

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);
    serverAddr.sin_port = htons(serverPort);

    std::cout << "[EBCLIENT] eBike Client started on: " << clientIp << ":" << clientPort << "." << std::endl;

    CSVHALManager hal(numPorts);
    GPSSensor gpsSensor(gpsSensorId);

    if (!gpsSensor.connect(&hal, 1, csvFile)) {
        std::cerr << "[EBCLIENT] Failed to connect GPS sensor.\n";
        return 1;
    }

    std::string joinMsg = "{\"type\":\"JOIN\",\"ebike_id\":" + std::to_string(ebikeId) +
                          ",\"timestamp\":\"" + currentTimestamp() + "\"}";
    clientSocket.sendto(joinMsg.c_str(), joinMsg.size(), 0, serverAddr);
    std::cout << "[EBCLIENT] Joined! Sampling every 5s" << std::endl;

    int readCount = 0;
    while (g_running && readCount < maxReadings) {
        std::string gpsData = gpsSensor.read();
        if (gpsData.empty()) break;

        std::string timestamp = currentTimestamp();
        std::cout << "[EBCLIENT] " << timestamp << " gps: " << gpsData << "(" << lockStatus << ")\n";

        double lat = 0.0, lon = 0.0;
        sscanf(gpsData.c_str(), "{\"lat\":%lf,\"lon\":%lf}", &lat, &lon);

        std::ostringstream dataMsg;
        dataMsg << "{\"type\":\"DATA\",\"ebike_id\":" << ebikeId
                << ",\"timestamp\":\"" << timestamp << "\""
                << ",\"lat\":" << lat
                << ",\"lon\":" << lon
                << ",\"status\":\"" << lockStatus << "\"}";

        clientSocket.sendto(dataMsg.str().c_str(), dataMsg.str().size(), 0, serverAddr);

        char buffer[256];
        struct sockaddr_in fromAddr;
        memset(&fromAddr, 0, sizeof(fromAddr));
        ssize_t received = clientSocket.recvfrom(buffer, sizeof(buffer) - 1, 0, fromAddr);
        if (received > 0) {
            buffer[received] = '\0';
            std::cout << "[EBCLIENT] ACK: " << buffer << std::endl;
        }

        readCount++;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    gpsSensor.disconnect();
    std::cout << "[EBCLIENT] Shutting down.\n";
    return 0;
}