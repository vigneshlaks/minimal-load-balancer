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
        std::vector<int> consistency_hash_table; // index to backend
        
        // assign packet to backend
        void assignPacket(const Packet& packet);

        void addBackend(const Backend& backend);
        void removeBackend(int ip);

        void setHealthCheck(int ip, int health_check);

        void populateConsistencyHashTable();
        void addConnectionTracking();
    
    private:
    
        std::unordered_map<int, Backend> backends;
        std::unordered_map<std::string, int> connection_tracking; // string (5 tuple) to backend ip
        
        // TODO: Add consistent hash table
        // TODO: Add lru table

        int computeHash(const Packet& packet);
        std::vector<int> generatePermutation(int backend_index, int M);
};


#endif

