#include "./packet.hpp"
#include <netinet/ip.h>
#include <netinet/udp.h>

class UDP_Packet : public Packet
{
public:
    udphdr *udp_hdr;
    UDP_Packet(const u_char *data, size_t length, timeval time_stamp);
    ~UDP_Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    void parse() override;
    void print() override;
};