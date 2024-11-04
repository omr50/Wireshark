#include "./packet.hpp"

class Ether_Packet : public Packet, public std::enable_shared_from_this<Packet>
{
public:
    ether_header *eth_hdr;

    Ether_Packet(const u_char *data, size_t length, timeval time_stamp);
    ~Ether_Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    void parse() override;
    std::string print() override;
    void print_dest_mac();
    void print_source_mac();
    void print_type();
};