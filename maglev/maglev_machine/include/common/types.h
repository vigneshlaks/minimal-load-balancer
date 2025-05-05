#ifndef MAGLEV_TYPES_H
#define MAGLEV_TYPES_H

struct Packet {
    int src_ip;
    int src_port;
    int dst_ip;
    int dst_port;
    int protocol;
};


struct Backend {
    int ip;
    int health_check;
};

#endif // MAGLEV_
