#ifndef MAGLEV_FORWARDER_H
#define MAGLEV_FORWARDER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "common/types.h"

class Forwarder {
    public:
        std::vector<int> consistency_hash_table; // index to backend
        
        // assign packet to backend
        void assignPacket(const Packet& packet);

        void addBackend(const Backend& backend);
        void removeBackend(int ip);
        const std::unordered_map<int, Backend>& getBackends();

        void setHealthCheck(int ip, int health_check);

        void populateConsistencyHashTable();
        void addConnectionTracking();
    
    private:
    
        std::unordered_map<int, Backend> backends; // ip to backend
        std::unordered_map<std::string, int> connection_tracking; // string (5 tuple) to backend ip

        int computeHash(const Packet& packet);
        std::vector<int> generatePermutation(int backend_index, int M);
};


#endif

