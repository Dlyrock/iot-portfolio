#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>
#include <regex>
#include "util/MiscUtils.h"
#include "../src/sensor/GPSSensor.h"
#include "hal/CSVHALManager.h"

TEST_CASE("Test GPSSensor is built correctly (id and dimensions)", "[GPSSensor]") {
    GPSSensor sensor(777);
    REQUIRE(sensor.getId() == 777);
    REQUIRE(sensor.getDimensions() == 2);
    REQUIRE_FALSE(sensor.isConnected());
}

TEST_CASE("Test GPSSensor formats correctly returning a valid pair of latitude and longitude in JSON format", "[GPSSensor]") {
    SECTION("Reading contains lat and lon keys") {
        GPSSensor sensor(777);
        std::string raw = "51.458119;-2.578869";
        std::vector<uint8_t> bytes(raw.begin(), raw.end());
        auto [key, val] = sensor.format(bytes);
        REQUIRE(val.find("\"lat\"") != std::string::npos);
        REQUIRE(val.find("\"lon\"") != std::string::npos);
    }
    SECTION("Reading is valid JSON format") {
        GPSSensor sensor(777);
        std::string raw = "51.458119;-2.578869";
        std::vector<uint8_t> bytes(raw.begin(), raw.end());
        auto [key, val] = sensor.format(bytes);
        std::regex gpsPattern(R"(\{"lat":-?\d+\.\d+,"lon":-?\d+\.\d+\})");
        REQUIRE(std::regex_match(val, gpsPattern));
    }
}