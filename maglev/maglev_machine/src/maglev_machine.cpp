#include "maglev_machine.h"
#include <iostream>

MaglevMachine::MaglevMachine(const std::string& vip) : vip_(vip) {
    // controller with reference to forwarder
    controller_ = new Controller(&forwarder_);
}

MaglevMachine::~MaglevMachine() {
    delete controller_;
}

void MaglevMachine::initialize() {
    // populate table
    forwarder_.populateConsistencyHashTable();

    // initialize health 
    controller_->updateHealth();

    std::cout << "Maglev machine initialized with VIP: " << vip_ << std::endl;
}

void MaglevMachine::processPacket(const Packet& packet) {
    forwarder_.assignPacket(packet);
}

void MaglevMachine::addBackend(int ip, int health_check) {
    Backend backend = {ip, health_check};
    forwarder_.addBackend(backend);
    
    // After adding a backend, update the hash table
    forwarder_.populateConsistencyHashTable();
    
    // check health again
    controller_->updateHealth();
}