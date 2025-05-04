// define the forwarder class

#ifndef MAGLEV_FORWARDER_H
#define MAGLEV_FORWARDER_H

#include <string>
#include <vector>
#include <unordered_map>

// packet info
struct Packet {
    int src_ip;
    int src_port;
    int dst_ip;
    int dst_port;
    int protocol;
};

// parsing for backend
struct Backend {
    int ip;
    int health_check;
};

class Forwarder {
    public:
        Forwarder();
        ~Forwarder();

        // parse packet and send to frontend
        void processPacket(const Packet& packet);

        void addBackend(const Backend& backend);
        void removeBackend(int ip);

        void setHealthCheck(int ip, int health_check);
    
    private:
        std::unordered_map<int, Backend> backends; // ip to backend
        // TODO: Add consistent hash table
        // TODO: Add lru table

        int computeHash(int ip);
};


#endif

