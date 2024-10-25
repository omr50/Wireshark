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
    std::weak_ptr<Packet> parentPacket;

    Packet() = default;
    virtual ~Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    virtual void parse(const u_char *data, size_t length) = 0;
    virtual void print() const = 0;
};