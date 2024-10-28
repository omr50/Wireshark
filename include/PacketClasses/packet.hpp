// abstract packet class which will be a parent
// for more concrete packet types (eth, ip, arp, tcp)
#pragma once
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>

class Packet
{
public:
    std::shared_ptr<Packet> encapsulatedPacket;
    std::shared_ptr<Packet> parentPacket;
    std::shared_ptr<Packet> rootPacket;
    u_char *start_data;
    size_t data_length;

    Packet() = default;
    virtual ~Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    virtual void parse() = 0;
    virtual void print() = 0;
};