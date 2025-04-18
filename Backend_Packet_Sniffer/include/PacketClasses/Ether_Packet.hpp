#pragma once
#include "./packet.hpp"
#include "../json.hpp"
#include <unordered_map>

class Ether_Packet : public Packet, public std::enable_shared_from_this<Packet>
{
public:
    ether_header *eth_hdr;
    // initialized in the capture function before we start

    // define the variable because declaring it doesn't define it
    static std::unordered_map<std::string, std::string> manufacturer_info;

    Ether_Packet(const u_char *data, size_t length, timeval time_stamp);
    ~Ether_Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    void parse() override;
    json print() override;
    std::string print_dest_mac();
    std::string print_source_mac();
    std::string print_type();
    std::pair<std::string, std::string> determine_source_dest_addr();
    std::string get_protocol();
    static void create_manufacturer_info_table();
    json detailed_protocol_info_print();
};