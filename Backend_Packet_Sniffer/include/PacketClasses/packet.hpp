// abstract packet class which will be a parent
// for more concrete packet types (eth, ip, arp, tcp)
#pragma once
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <string>
#include "../json.hpp"
// #include <boost/asio.hpp>
using json = nlohmann::json;
class Packet
{
public:
    std::shared_ptr<Packet> encapsulatedPacket;
    std::weak_ptr<Packet> parentPacket;

    std::string packet_type;
    u_char *start_data;
    size_t data_length;
    timeval timestamp;
    int layer;
    static int packet_id;
    //  boost::asio::io_service io_service;

    Packet() = default;
    virtual ~Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    // the parser can also give back valuable information in the host byte
    // order instead of the network byte order so that we can use it more
    // effectively.
    virtual void parse() = 0;
    virtual json detailed_protocol_info_print() = 0;
    virtual json print() = 0;
    // we will create a function that should take the current packets structure
    // and prepare a partial json structure, and that too will be added to the
    // total json structure.

    // virtual void prepare_json() = 0;
};