/**
 * @file ebikeClient.cpp
 * @brief Main eBike on-board client application.
 * Module: UFCFVK-15-2 Internet of Things
 */

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
#include <csignal>
#include <ctime>

#include "util/MiscUtils.h"
#include "hal/CSVHALManager.h"
#include "sensor/GPSSensor.h"

namespace ebikeConstants {
    const std::string CONFIG_PATH = "config/client-config.yaml";
}

static std::atomic<bool> g_running{true};

void signalHandler(int) {
    g_running = false;
}

std::string currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_info = std::localtime(&t);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return std::string(buf);
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0]
                  << " <client_ip> <ebike_id> <csv_file> <num_ports>\n";
        return 1;
    }

    std::string clientIp = argv[1];
    std::string ebikeId  = argv[2];
    std::string csvFile  = argv[3];
    int numPorts         = std::atoi(argv[4]);

    std::signal(SIGINT,  signalHandler);
    std::signal(SIGTERM, signalHandler);

    int maxReadings = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "client", "max_readings"));
    int gpsSensorId = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "client", "gps_sensor_id"));

    std::string lockStatus = "unlocked";

    CSVHALManager hal(numPorts);
    GPSSensor gpsSensor(gpsSensorId);

    if (!gpsSensor.connect(&hal, 1, csvFile)) {
        std::cerr << "[EBCLIENT] Failed to connect GPS sensor.\n";
        return 1;
    }

    int readCount = 0;
    while (g_running && readCount < maxReadings) {
        std::string gpsData = gpsSensor.read();
        if (gpsData.empty()) break;

        std::cout << "[EBCLIENT] " << currentTimestamp()
                  << " gps: " << gpsData
                  << "(" << lockStatus << ")\n";

        readCount++;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    gpsSensor.disconnect();
    std::cout << "[EBCLIENT] Shutting down.\n";
    return 0;
}