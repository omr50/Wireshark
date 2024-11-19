#include "./packet.hpp"
#include <netinet/ip.h>
#include <netinet/tcp.h>

class TCP_Packet : public Packet
{
public:
    tcphdr *tcp_hdr;
    TCP_Packet(const u_char *data, size_t length, timeval time_stamp);
    ~TCP_Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    void parse() override;
    json print() override;
    json detailed_protocol_info_print();
};