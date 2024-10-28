/*
Responsible for taking in the data from the callback and determine
the packet structure contained with in.

        Input                             output

pointer to packet data          packet classes eth -> ip -> tcp -> http (etc.)

*/
#pragma once

#include <memory>
#include "./PacketClasses/packet.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>

class Parser
{

public:
        Parser();
        // Determine class type and return a pointer to it
        // CHANGE FUNCTION TYPE LATER
        static std::shared_ptr<Packet> Determine_Packet(const struct pcap_pkthdr *header, const u_char *packet);
        static u_char *copy_data(const u_char *packet, size_t length);
};