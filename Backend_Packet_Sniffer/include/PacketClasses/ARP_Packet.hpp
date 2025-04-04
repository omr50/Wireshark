#include "./packet.hpp"
#include <netinet/if_ether.h>

class ARP_Packet : public Packet
{
public:
    ether_arp *arp_hdr;

    ARP_Packet(const u_char *data, size_t length, timeval time_stamp);
    ~ARP_Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    void parse() override;
    json print() override;
    void print_sender_protocol();
    void print_target_protocol();
    void print_type();
    json detailed_protocol_info_print();
    std::string print_hardware_address(uint8_t addr_arr[6]);
    std::string print_protocol_address(uint8_t addr_arr[4]);
};