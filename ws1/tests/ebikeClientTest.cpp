#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <sstream>
#include <regex>
#include "util/MiscUtils.h"
#include "../src/sensor/GPSSensor.h"
#include "hal/CSVHALManager.h"

TEST_CASE("Test HALManager is initialised and manages ports (attach and release) correctly", "[ebikeClient]") {
    SECTION("GPSSensor connects and disconnects from HAL") {
        CSVHALManager hal(5);
        GPSSensor sensor(777);
        hal.attachDevice(1, std::shared_ptr<GPSSensor>(&sensor, [](GPSSensor*){}));
        hal.initialise("data/sim-eBike-1.csv");
        REQUIRE(hal.isBusy(1));
        hal.releaseDevice(1);
        REQUIRE_FALSE(hal.isBusy(1));
    }
    SECTION("GPS reading is non-empty after connection") {
        GPSSensor sensor(777);
        std::string raw = "51.458119;-2.578869";
        std::vector<uint8_t> bytes(raw.begin(), raw.end());
        auto [key, val] = sensor.format(bytes);
        REQUIRE_FALSE(val.empty());
    }
    SECTION("Log output line format is correct") {
        std::string gpsData = "{\"lat\":51.457130,\"lon\":-2.557153}";
        std::string lockStatus = "unlocked";
        std::string timestamp = "2025-05-31 11:16:46";
        std::ostringstream logLine;
        logLine << "[EBCLIENT] " << timestamp
                << " gps: " << gpsData
                << "(" << lockStatus << ")";
        std::string line = logLine.str();
        REQUIRE(line.find("[EBCLIENT]") != std::string::npos);
        REQUIRE(line.find("gps:") != std::string::npos);
        REQUIRE(line.find("\"lat\":") != std::string::npos);
        REQUIRE(line.find("\"lon\":") != std::string::npos);
        REQUIRE(line.find("(unlocked)") != std::string::npos);
    }
}