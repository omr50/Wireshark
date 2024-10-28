#include "../include/PacketClasses/ARP_Packet.hpp"

ARP_Packet::ARP_Packet(const u_char *data, size_t length, std::weak_ptr<Packet> parent, timeval time_stamp)
{
    this->arp_hdr = (ether_arp *)start_data;
    this->timestamp = time_stamp;
    this->parentPacket = parent;
    // this will set the child node
    this->parse();
}

void ARP_Packet::parse()
{
    // ARP has no encapsulated packets
    this->encapsulatedPacket = nullptr;
    return;
}