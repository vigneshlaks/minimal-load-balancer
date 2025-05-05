#ifndef MAGLEV_MACHINE_H
#define MAGLEV_MACHINE_H

#include "controller.h"
#include "forwarder.h"
#include "common/types.h"
#include <memory>
#include <string>

class MaglevMachine {
public:
    // Constructor and destructor
    MaglevMachine(const std::string& vip);
    ~MaglevMachine();
    
    // Core functionality
    void initialize();
    void processPacket(const Packet& packet);
    void addBackend(int ip, int health_check = 1);
    
private:
    std::string vip_;
    Forwarder forwarder_;
    Controller* controller_;
};

#endif // MAGLEV_MACHINE_H