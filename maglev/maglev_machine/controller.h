#ifndef MAGLEV_CONTROLLER_H

#include <thread>
#include <atomic>

class Forwarder;

class Controller {
private:
    // assign forwarder reference to controller
    Forwarder* forwarder;

    std::atomic<bool> running;
    std::atomic<bool> isHealthy;
    std::thread healthCheckThread;

public:
    Controller(Forwarder* fwd);
    ~Controller();

    void startHealthChecks();
    void stopHealthChecks();
    bool isRunning() const;

private:
    void healthCheckLoop();
    void announceBGP(bool announce);
};

#endif