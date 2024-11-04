// abstract packet class which will be a parent
// for more concrete packet types (eth, ip, arp, tcp)
#pragma once
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
// #include <boost/asio.hpp>

class Packet
{
public:
    std::shared_ptr<Packet> encapsulatedPacket;
    std::weak_ptr<Packet> parentPacket;

    u_char *start_data;
    size_t data_length;
    timeval timestamp;
    //  boost::asio::io_service io_service;

    Packet() = default;
    virtual ~Packet() = default;
    // should parse its own protocol specific data within the parser class
    // make sure virtual functions are implemented in the concrete classes
    // the parser can also give back valuable information in the host byte
    // order instead of the network byte order so that we can use it more
    // effectively.
    virtual void parse() = 0;
    virtual void print() = 0;
    // we will create a function that should take the current packets structure
    // and prepare a partial json structure, and that too will be added to the
    // total json structure.

    // virtual void prepare_json() = 0;
};