#include "./packet.hpp"
#include <netinet/ip.h>

class IP_Packet : public Packet, public std::enable_shared_from_this<Packet>
{
public:
    iphdr *ip_hdr;

    IP_Packet(const u_char *data, size_t length, timeval time_stamp);
    ~IP_Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    void parse() override;
    std::string print() override;
};