#include "forwarder.h"
#include <iostream>


void Forwarder::assignPacket(const Packet& packet) {
    // create tuple
    std::string five_tuple = std::to_string(packet.src_ip) + ":" + 
                            std::to_string(packet.src_port) + ":" + 
                            std::to_string(packet.dst_ip) + ":" + 
                            std::to_string(packet.dst_port) + ":" + 
                            std::to_string(packet.protocol);
    
    // create hash from tuple
    std::hash<std::string> hasher;
    size_t hash_value = hasher(five_tuple);
    
    // check if connection exists
    auto conn_it = connection_tracking.find(five_tuple);

    if (conn_it != connection_tracking.end()) {
        int backend_ip = conn_it->second;
        return;
    }

    // mod for new index
    int table_index = hash_value % consistency_hash_table.size();
    int backend_ip = consistency_hash_table[table_index];

    // add to connection tracking table
    connection_tracking[five_tuple] = backend_ip;
}


void Forwarder::populateConsistencyHashTable() {
    std::vector<int> backend_ips;
    for (const auto& pair : backends) {
        backend_ips.push_back(pair.first);
    }
    
    int N = backend_ips.size();
    
    // specified in paper as default or M = n * 100 for even dist.
    int M = 65537;
    
    // resize to correct size
    // init to -1
    consistency_hash_table.resize(M, -1);
    
    // permutation for each backend
    std::vector<std::vector<int>> permutation(N);
    
    for (int i = 0; i < N; i++) {
        // can also be thought of as each preference list
        permutation[i] = generatePermutation(backend_ips[i], M);
    }
    
    std::vector<int> next(N, 0);
    
    for (int j = 0; j < M; j++) {
        consistency_hash_table[j] = -1; 
    }
    
    int n = 0;

    // here we fill hash table based on the preference lists
    // "Round Robin" fashion where each backend fills one consistency table position until all are filled
    while (true) {
        // for every backend
        for (int i = 0; i < N; i++) {
            // get backend's preference list and get where we are in that preference ordering
            int c = permutation[i][next[i]];
            
            // if a backend has already been set
            while (consistency_hash_table[c] >= 0) {
                // update index
                next[i] = next[i] + 1;

                // for the same backend get the next preference
                c = permutation[i][next[i]];
            }
            
            // assign ip
            consistency_hash_table[c] = backend_ips[i];
            next[i] = next[i] + 1;
            n = n + 1;
            
            // terminate when table is filled
            if (n == M) {
                return;
            }
        }
    }
}

// helper implementing paper permutation
std::vector<int> Forwarder::generatePermutation(int backend_index, int M) {
    std::vector<int> perm(M);

    std::hash<int> hasher;
    size_t h1 = hasher(backend_index);
    
    // for unique second hash
    size_t h2 = hasher(backend_index + 1000);
    
    int offset = h1 % M;
    int skip = (h2 % (M - 1)) + 1;

    for (int j = 0; j < M; j++) {
        perm[j] = (offset + j * skip) % M;
    }
    
    return perm;
}

void Forwarder::addBackend(const Backend& backend) {
    backends[backend.ip] = backend;
}

const std::unordered_map<int, Backend>& Forwarder::getBackends() {
    return backends;
}

void Forwarder::removeBackend(int ip) {
    backends.erase(ip);
}

void Forwarder::setHealthCheck(int ip, int health_check) {
    Backend backend = backends[ip];
    backend.health_check = health_check;
}