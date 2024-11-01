#include "../include/PacketClasses/UDP_Packet.hpp"

UDP_Packet::UDP_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    printf("UDP Packet Created!\n");
    this->udp_hdr = (udphdr *)data;
    this->data_length = length;
    this->timestamp = time_stamp;
}

void UDP_Packet::parse()
{
    printf("Parsed UDP!\n");
}

void UDP_Packet::print()
{
    printf("UDP Packet print!\n");
}