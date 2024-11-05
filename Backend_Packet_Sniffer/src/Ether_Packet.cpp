#include "../include/PacketClasses/Ether_Packet.hpp"
#include "../include/PacketClasses/IP_Packet.hpp"
#include "../include/PacketClasses/ARP_Packet.hpp"
#include "../include/utils.hpp"
#include <sstream>

using json = nlohmann::json;

Ether_Packet::Ether_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    this->start_data = (u_char *)data;
    this->eth_hdr = (ether_header *)start_data;
    this->timestamp = time_stamp;
    this->data_length = length;
    this->packet_type = "ETH";
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

json Ether_Packet::print()
{
    json full_packet;
    json packet_info;

    // timeval to seconds
    packet_info["time"] = timeval_to_string(this->timestamp);
    // packet_info["source"] =

    full_packet["dest_mac"] = this->print_dest_mac();
    full_packet["src_mac"] = this->print_source_mac();
    full_packet["eth_type"] = this->print_type();
    return full_packet;
}

std::string Ether_Packet::print_dest_mac()
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (int i = 0; i < 6; i++)
    {
        oss << std::setw(2) << static_cast<int>(eth_hdr->ether_dhost[i]);
        if (i != 5)
            oss << ":";
    }
    oss << "\n";
    return oss.str();
}

std::string Ether_Packet::print_source_mac()
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (int i = 0; i < 6; i++)
    {
        oss << std::setw(2) << static_cast<int>(eth_hdr->ether_shost[i]);
        if (i != 5)
            oss << ":";
    }
    oss << "\n";
    return oss.str();
}

std::string Ether_Packet::print_type()
{
    std::ostringstream oss;
    oss << "0x" << std::hex << std::setw(4) << std::setfill('0') << ntohs(eth_hdr->ether_type);
    return oss.str();
}
