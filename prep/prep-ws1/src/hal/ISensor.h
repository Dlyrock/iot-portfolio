#ifndef ISENSOR_H
#define ISENSOR_H

#include "IDevice.h"
#include <vector>
#include <string>
#include <cstdint>

class ISensor : public IDevice {
    
public:
    virtual ~ISensor() = default;
    virtual int getDimension() const = 0;
    virtual std::pair<std::string, std::string> format(std::vector<uint8_t> reading) = 0;
};

#endif // ISENSOR_H