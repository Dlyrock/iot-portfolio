

//TODO: Include the CSVHALManager and TemperatureSensor headers
#include <thread>
#include <chrono>
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <csv_file> <num_ports>" << std::endl;
        return 1;
    }

    //Get the CSV file and number of ports from the command line arguments
    std::string csvFile = argv[1];
    int numPorts = std::atoi(argv[2]);

    //TODO: Create HAL manager
    

    // Create a TemperatureSensor object with ID 0 and dimension 1. Just need to uncomment the line below
    //auto tsensor = std::make_shared<TemperatureSensor>(0,1);

    // Attach devices to ports
    //TODO Attach the temperature sensor to the HAL manager

    // Initialize and read data

    //TODO: Initialize the HAL manager with the CSV file
 
    //TODO: A For loop that reads the temperature data 3 times. Between each reading, sleep for 1 second, for this you can send the thread to sleep using std::this_thread::sleep_for(std::chrono::seconds(1));
    

    //TODO: Release the device from the HAL manager

    return 0;
}