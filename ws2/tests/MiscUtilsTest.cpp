#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "TestUtils.h"
#include "util/MiscUtils.h"

#include <memory>
#include <iostream>
#include <vector>
#include <string>

std::vector<TestUtils::TestCase> testCases = TestUtils::getTestCasesForSuite("tests/config/unit_tests.yaml", "MiscUtilsTest" ,"tests/data/json/");


TEST_CASE("Test that, given vector of key-value pairs, a JSON string is correctly generated with the key-values of the vector", "[MiscUtils]") {
    // Get the test name from the current test case and load data
    std::string testName = Catch::getResultCapture().getCurrentTestName();
    TestUtils::TestCaseData testData = TestUtils::getTestCaseDataByName(testCases, testName);

    // Extract test parameters
    int loc_id = TestUtils::getInt(testData.at("location_id"));
    std::string timestamp = TestUtils::getString(testData.at("timestamp"));
    std::string reading_temp = TestUtils::getString(testData.at("reading_temperature"));
    
    // Create a vector of key-value pairs to simulate sensor data
    std::vector<std::pair<std::string, std::string>> data = {
        {"location_id", std::to_string(loc_id)},
        {"timestamp", timestamp},
        {"reading", "{\"temperature\":" + reading_temp +  "}"}
    };
    // Generate JSON string from the data
    std::string json = generateJsonString(data);
    // Check if the JSON string contains the expected keys and values
    REQUIRE(json.find("\"location_id\":") != std::string::npos);
    REQUIRE(json.find("\"timestamp\":") != std::string::npos);
    REQUIRE(json.find("\"reading\":") != std::string::npos);
    REQUIRE(json.front() == '{');
    REQUIRE(json.back() == '}');
    // Check if the reading contains the expected temperature
    REQUIRE(json.find("\"temperature\":" + reading_temp) != std::string::npos);
  

}
