#include "../include/parser.hpp"

// change function type later.
void Parser::Determine_Packet(pcap_pkthdr *header, const u_char *packet)
{
    // parser will keep track of the root parent packet node, and build up the object linked
    // list, and then return the root packet which we can later use to transfer to the front
    // end and basically get rid of it after. The memory should no longer be needed once we
    // have sent to the frontend.
    return;
}

Parser::Parser()
{
    printf("Test\n");
}