#include "../include/parser.hpp"

Parser::Parser()
{
    printf("Test\n");
}

// change function type later.
std::shared_ptr<Packet> Parser::Determine_Packet(const struct pcap_pkthdr *header, const u_char *packet)
{
    // parser will keep track of the root parent packet node, and build up the object linked
    // list, and then return the root packet which we can later use to transfer to the front
    // end and basically get rid of it after. The memory should no longer be needed once we
    // have sent to the frontend.

    // USE THE TIME STAMP AS WELL IN THE CLASS (not sure if only in base or all)
    timeval time_stamp = header->ts;
    printf("HEADER LENGTH %d\n", header->len);
    u_char *data = Parser::copy_data(packet, header->len);
    // In the end we want to return the ether packet, but
    // we also want to build up the entire list( of packets ) before we
    // actually return.

    // ref count = 1
    auto root_packet = std::make_shared<Ether_Packet>(data, header->len, time_stamp);
    // call the parse of ethernet packet and it will cause a chain reaction
    // that will cause all packets to be parsed and created.
    printf("Started parser\n");
    root_packet->parse();
    return root_packet;
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