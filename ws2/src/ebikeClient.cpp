#include <string>
#include <iostream>
#include "util/MiscUtils.h"

/***TODO: complete code as per assignment specification***/

// --- Constants ---
namespace ebikeConstants {
    const std::string CONFIG_PATH = "config/client-config.yaml";
}


int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <client_ip> <ebike_id> <csv_file> <num_ports>" << std::endl;
        return 1;
    }
    
    //***IMPORTANT: DO NOT CHANGE THE ORDER, QUANTITY OR TYPE OF THE COMMAND LINE ARGUMENTS*/
    
    // Get the client IP, eBike ID, CSV file, and number of ports from the command line arguments
    std::string clientIp = argv[1]; // Client IP address
    std::string ebikeId = argv[2]; // eBike ID
    std::string csvFile = argv[3]; // CSV file path, normally in the data directory of the project
    int numPorts = std::atoi(argv[4]); //Number of ports of the HAL manager

    int maxReadings = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "client", "max_readings"));
   

    std::cout << "Client IP: " << clientIp << std::endl;
    std::cout << "eBike ID: " << ebikeId << std::endl;
    std::cout << "CSV File: " << csvFile << std::endl;
    std::cout << "Number of Ports: " << numPorts << std::endl;
    std::cout << "Max Readings: " << maxReadings << std::endl;

    

     //TODO: complete code as per assignment specification

}