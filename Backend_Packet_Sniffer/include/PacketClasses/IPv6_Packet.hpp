#include "./packet.hpp"
#include <netinet/ip6.h>

class IPv6_Packet : public Packet, public std::enable_shared_from_this<Packet>
{
public:
    ip6_hdr *ipv6_hdr;

    IPv6_Packet(const u_char *data, size_t length, timeval time_stamp);
    ~IPv6_Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    void parse() override;
    json print() override;
    std::string print_source_addr();
    std::string print_dest_addr();
    json detailed_protocol_info_print();
};