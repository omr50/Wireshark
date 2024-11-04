#include "./packet.hpp"
#include <netinet/if_ether.h>

class ARP_Packet : public Packet
{
public:
    ether_arp *arp_hdr;

    ARP_Packet(const u_char *data, size_t length, std::weak_ptr<Packet> parent, timeval time_stamp);
    ~ARP_Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    void parse() override;
    json print() override;
    void print_sender_protocol();
    void print_target_protocol();
    void print_type();
};