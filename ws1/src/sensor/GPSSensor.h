#ifndef GPS_SENSOR_H
#define GPS_SENSOR_H
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <memory>
#include "hal/CSVHALManager.h"
#include "hal/ISensor.h"

class GPSSensor : public ISensor {
public:
    explicit GPSSensor(int id)
        : sensorId_(id), halManager_(nullptr), port_(-1) {}

    ~GPSSensor() override { disconnect(); }

    int getId() const override { return sensorId_; }
    int getDimension() const override { return 2; }
    int getDimensions() const { return 2; }

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
        if (!csvFile.empty()) {
            halManager_->initialise(csvFile);
        }
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

    bool isConnected() const {
        return (halManager_ != nullptr && port_ >= 0);
    }

private:
    int sensorId_;
    CSVHALManager* halManager_;
    int port_;
};

#endif // GPS_SENSOR_H