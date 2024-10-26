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
        // Determine class type and return a pointer to it
        std::weak_ptr<Packet> Determine_Packet(pcap_pkthdr *header, const u_char *packet)
        {
                // use the header to determine packet, create packet, allow packet
                // to parse its own headers. Then continue on to the next packet.
                // Also another function will be responsible for linking all the
                // packets together.

                // does the callback allocate pointer on the heap?
        }
};