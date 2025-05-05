#include "controller.h"
#include "forwarder.h"
#include <iostream>

Controller::Controller(Forwarder *fwd) : forwarder(fwd), isHealthy(false) {
    // return error if fwd not initalized
    if (!forwarder) {
        std::cerr << "Error: Null forwarder" <<std::endl;
    }
}

void Controller::announceBGP (bool announce) {
    // TODO
    return;
}

void Controller::updateHealth() {
    std::unordered_map<int, Backend> backends = forwarder->getBackends();
    
    // init to true will set to false if any backend is false for simplicity
    bool allHealthy = true;
    
    for (const auto& pair : backends) {
        const Backend& backend = pair.second;
        
        // Assuming health_check value of 1 means healthy
        if (backend.health_check != 1) {
            allHealthy = false;
            break;
        }
    }
    
    // Update health status
    isHealthy = allHealthy;
}