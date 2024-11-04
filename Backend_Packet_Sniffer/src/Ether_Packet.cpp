#include "../include/PacketClasses/Ether_Packet.hpp"
#include "../include/PacketClasses/IP_Packet.hpp"
#include "../include/PacketClasses/ARP_Packet.hpp"

Ether_Packet::Ether_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    this->start_data = (u_char *)data;
    this->eth_hdr = (ether_header *)start_data;
    this->timestamp = time_stamp;
    // not sure if there is a better way to make the
    // parent node set to null
    std::shared_ptr<Packet> null_packet = nullptr;
    this->parentPacket = null_packet;
    this->encapsulatedPacket = nullptr;
    printf("Ether packet created!\n");
}

void Ether_Packet::parse()
{
    printf("Reached parse\n");
    this->print_type();

    std::shared_ptr<Packet> shared = shared_from_this();
    // Create a weak_ptr from self
    std::weak_ptr<Packet> weak_self = shared;

    if (ntohs(eth_hdr->ether_type) == 0x0800)
    {
        // THIS IS AN IP PACKET
        // ip header pointer = eth hdr + eth hdr length
        printf("IP PACKET PARSE\n");
        std::shared_ptr<IP_Packet> ip_packet = std::make_shared<IP_Packet>((const u_char *)(eth_hdr + 1), (size_t)(this->data_length - sizeof(eth_hdr)), this->timestamp);
        ip_packet->parse();
        this->encapsulatedPacket = ip_packet;
        ip_packet->parentPacket = weak_self;
    }
    else if (ntohs(eth_hdr->ether_type) == 0x0806)
    {

        std::shared_ptr<ARP_Packet> arp_packet = std::make_shared<ARP_Packet>((const u_char *)(eth_hdr + 1), this->data_length - sizeof(eth_hdr), weak_self, this->timestamp);
        this->encapsulatedPacket = arp_packet;
        arp_packet->parentPacket = weak_self;
    }
}

std::string Ether_Packet::print()
{
    std::string msg((char *)this->eth_hdr, this->data_length);
    return msg;
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