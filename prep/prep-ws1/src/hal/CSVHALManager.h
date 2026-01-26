#pragma once

#ifndef CSVHALMANAGER_H
#define CSVHALMANAGER_H

#include "ISensor.h"
#include "IActuator.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

class CSVHALManager {
private:
    std::vector<std::vector<std::string>> data; // CSV data
    std::unordered_map<int, std::shared_ptr<IDevice>> portDeviceMap; // Port ID -> Device (generic pointer)
    size_t sequence; // Current sequence (row index)
    int numPorts; // Total number of ports

    std::vector<uint8_t> convertToByteVector(const std::vector<std::string>& stringVector);

public:
    // Constructor
    CSVHALManager(int numPorts);
    
    // Initialise the CSV file
    void initialise(const std::string& filePath);

    // Get the device attached to a port
    std::shared_ptr<IDevice> getDevice(int portId) const;

    // Attach a device to a port
    void attachDevice(int portId, const std::shared_ptr<IDevice>& device);

    // Release a device from a port
    void releaseDevice(int portId);

    // Check if a port is busy
    bool isBusy(int portId) const;

    // Read data from a sensor
    std::vector<uint8_t> read(int portId);

    // Write data to an actuator
    void write(int portId, const std::vector<uint8_t>& data);

    // Getters for portDeviceMap
    const std::unordered_map<int, std::shared_ptr<IDevice>>& getPortDeviceMap() const;
    std::unordered_map<int, std::shared_ptr<IDevice>>& getPortDeviceMap();
};

#endif // CSVHALMANAGER_H