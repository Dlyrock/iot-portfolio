#ifndef IDEVICE_H
#define IDEVICE_H

class IDevice {
public:
    virtual ~IDevice() = default;    
    virtual int getId() const = 0;
};

#endif // IDEVICE_H