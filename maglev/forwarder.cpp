#include "forwarder.h"
#include <iostream>


void Forwarder::assignPacket(const Packet& packet) {
    // get hash
    auto five_tuple = (packet.src_ip, packet.src_port, packet.dst_ip, packet.dst_port, packet.protocol);


    // hash tuple


    // check connection tracking table 

    // check consistency hash table

}


void Forwarder::populateConsistencyHashTable() {
    std::vector<int> backend_ips;
    for (const auto& pair : backends) {
        backend_ips.push_back(pair.first);
    }
    
    int N = backend_ips.size();
    
    int M = 65537;
    
    // resize to correct size
    consistency_hash_table.resize(M, -1);
    
    // permutation fo reach backend
    std::vector<std::vector<int>> permutation(N);
    
    for (int i = 0; i < N; i++) {
        permutation[i] = generatePermutation(backend_ips[i], M);
    }
    
    std::vector<int> next(N, 0);
    
    for (int j = 0; j < M; j++) {
        consistency_hash_table[j] = -1; 
    }
    
    int n = 0;
    
    while (true) {
        for (int i = 0; i < N; i++) {
            int c = permutation[i][next[i]];
            
            // find next position
            while (consistency_hash_table[c] >= 0) {
                next[i] = next[i] + 1;
                c = permutation[i][next[i]];
            }
            
            // assign ip
            consistency_hash_table[c] = backend_ips[i];
            next[i] = next[i] + 1;
            n = n + 1;
            
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

void Forwarder::removeBackend(int ip) {
    backends.erase(ip);
}

void Forwarder::setHealthCheck(int ip, int health_check) {
    Backend backend = backends[ip];
    backend.health_check = health_check;
}