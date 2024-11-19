#include "../include/PacketClasses/ARP_Packet.hpp"

ARP_Packet::ARP_Packet(const u_char *data, size_t length, timeval time_stamp)

{
    this->arp_hdr = (ether_arp *)start_data;
    this->timestamp = time_stamp;
    this->data_length = length;
    this->packet_type = "ARP";
    this->layer = 2;
    // this will set the child node
    printf("ARP packet created!\n");
    this->parse();
}

void ARP_Packet::parse()
{
    // ARP has no encapsulated packets
    this->encapsulatedPacket = nullptr;
    return;
}

json ARP_Packet::print()
{
    std::string msg((char *)this->arp_hdr, this->data_length);
    return msg;
}

json ARP_Packet::detailed_protocol_info_print()
{
    json temp_msg;
    temp_msg["protocol"] = this->packet_type;
    return temp_msg;
}