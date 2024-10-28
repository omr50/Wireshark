#include "../include/PacketClasses/Ether_Packet.hpp"

Ether_Packet::Ether_Packet(const u_char *data, size_t length)
{
    this->eth_hdr = (ether_header *)start_data;
    this->parentPacket = nullptr;
    this->encapsulatedPacket = nullptr;
    this->parse();
    // copy the memory first into heap allocated value.
}

void Ether_Packet::parse()
{
}

void Ether_Packet::print_dest_mac()
{
    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth_hdr->ether_dhost[0], eth_hdr->ether_dhost[1], eth_hdr->ether_dhost[2],
           eth_hdr->ether_dhost[3], eth_hdr->ether_dhost[4], eth_hdr->ether_dhost[5]);
}

void Ether_Packet::print_source_mac()
{
    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth_hdr->ether_shost[0], eth_hdr->ether_shost[1], eth_hdr->ether_shost[2],
           eth_hdr->ether_shost[3], eth_hdr->ether_shost[4], eth_hdr->ether_shost[5]);
}

void Ether_Packet::print_type()
{
    u_short ether_type = ntohs(eth_hdr->ether_type);
    printf("EtherType: 0x%04x\n", ether_type);
}