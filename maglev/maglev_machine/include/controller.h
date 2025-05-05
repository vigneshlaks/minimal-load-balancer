#ifndef MAGLEV_CONTROLLER_H
#define MAGLEV_CONTROLLER_H 

#include "common/types.h"
#include "forwarder.h"

class Forwarder;

class Controller {
private:
    // assign forwarder reference to controller
    Forwarder* forwarder;
    bool isHealthy;

public:
    Controller(Forwarder* fwd);
    ~Controller();

    // todo: incorporate bgp logic for vip
    void announceBGP(bool announce);

    // for now just return health boolean
    void updateHealth();
    
};

#endif