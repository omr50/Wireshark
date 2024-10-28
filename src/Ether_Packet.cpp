#include "../include/PacketClasses/Ether_Packet.hpp"
#include "../include/PacketClasses/IP_Packet.hpp"

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
    // basically use the eth_hdr to determine the EtherType and
    // find the next packet and create it.
    // ip
    if (ntohs(eth_hdr->ether_type) == 0x0800)
    {
        // THIS IS AN IP PACKET
        // ip header pointer = eth hdr + eth hdr length
        std::shared_ptr<IP_Packet> ip_packet = std::make_shared<IP_Packet>(start_data + 1, this->data_length - sizeof(eth_hdr));
        ip_packet->parse();
    }
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