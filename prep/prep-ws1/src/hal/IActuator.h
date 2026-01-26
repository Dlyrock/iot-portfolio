#ifndef IACTUATOR_H
#define IACTUATOR_H

#include "IDevice.h"
#include <vector>
#include <cstdint>

class IActuator : public IDevice{
public:
    virtual ~IActuator() = default;

    // Send data to the actuator
    virtual void send(const std::vector<uint8_t>& data) = 0;
};

#endif // IACTUATOR_H