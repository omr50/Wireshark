#include "../include/parser.hpp"
#include "../include/PacketClasses/Ether_Packet.hpp"

// change function type later.
std::shared_ptr<Packet> Parser::Determine_Packet(pcap_pkthdr *header, const u_char *packet)
{
    // parser will keep track of the root parent packet node, and build up the object linked
    // list, and then return the root packet which we can later use to transfer to the front
    // end and basically get rid of it after. The memory should no longer be needed once we
    // have sent to the frontend.

    u_char *data = Parser::copy_data(packet, header->len);
    //
    return std::make_shared<Ether_Packet>(data, header->len);
}

Parser::Parser()
{
    printf("Test\n");
}

u_char *Parser::copy_data(const u_char *data, size_t length)
{
    u_char *copied_data = (u_char *)malloc(length);
    for (int i = 0; i < length; i++)
    {
        copied_data[i] = data[i];
    }

    return copied_data;
}