#ifndef MAGLEV_TYPES_H
#define MAGLEV_TYPES_H

// Packet structure for network traffic
struct Packet {
    int src_ip;
    int src_port;
    int dst_ip;
    int dst_port;
    int protocol;
};

// Backend structure for load balancing targets
struct Backend {
    int ip;
    int health_check;
};

#endif // MAGLEV_
