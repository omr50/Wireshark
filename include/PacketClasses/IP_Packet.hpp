#include "./packet.hpp"
#include <netinet/ip.h>

class IP_Packet : public Packet
{
public:
    iphdr *ip_hdr;

    IP_Packet(const u_char *data, size_t length);
    ~IP_Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    void parse() override;
    void print() override;
};