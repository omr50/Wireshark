#include "../include/PacketClasses/UDP_Packet.hpp"

UDP_Packet::UDP_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    printf("UDP Packet Created!\n");
    this->start_data = (u_char *)data;
    this->udp_hdr = (udphdr *)data;
    this->data_length = length;
    this->timestamp = time_stamp;
    this->packet_type = "UDP";
    this->layer = 4;
}

void UDP_Packet::parse()
{
    printf("Parsed UDP!\n");
}

json UDP_Packet::print()
{
    std::string msg((char *)this->udp_hdr, this->data_length);
    return msg;
}

json UDP_Packet::detailed_protocol_info_print()
{
    json temp_msg;
    temp_msg["protocol"] = this->packet_type;
    return temp_msg;
}