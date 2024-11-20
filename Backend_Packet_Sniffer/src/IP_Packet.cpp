#include "../include/PacketClasses/IP_Packet.hpp"
#include "../include/PacketClasses/TCP_Packet.hpp"
#include "../include/PacketClasses/UDP_Packet.hpp"
#include "../include/utils.hpp"

IP_Packet::IP_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    printf("IP Packet Created!\n");
    this->start_data = (u_char *)data;
    this->ip_hdr = (iphdr *)data;
    this->data_length = length;
    this->timestamp = time_stamp;
    this->packet_type = "IP";
    this->layer = 3;
}

void IP_Packet::parse()
{
    std::shared_ptr<Packet> shared = shared_from_this();
    std::weak_ptr<Packet> weak_self = shared;
    // 1 byte so no need to conveert to little endian
    if (this->ip_hdr->protocol == 0x01)
    {
        // CREATE ICMP PACKET
        printf("ICMP PACKET!\n");
    }
    else if (this->ip_hdr->protocol == 0x06)
    {
        printf("TCP PACKET!\n");
        std::shared_ptr<TCP_Packet> tcp_packet = std::make_shared<TCP_Packet>((u_char *)(this->ip_hdr + 1), this->data_length - sizeof(ip_hdr), this->timestamp);
        tcp_packet->parse();

        this->encapsulatedPacket = tcp_packet;
        tcp_packet->parentPacket = weak_self;
    }
    else if (this->ip_hdr->protocol == 0x11)
    {
        printf("UDP PACKET!\n");
        std::shared_ptr<UDP_Packet> udp_packet = std::make_shared<UDP_Packet>((u_char *)(this->ip_hdr + 1), this->data_length - sizeof(ip_hdr), this->timestamp);
        udp_packet->parse();

        this->encapsulatedPacket = udp_packet;
        udp_packet->parentPacket = weak_self;
    }
}

json IP_Packet::print()
{
    std::string msg((char *)this->ip_hdr, this->data_length);
    return msg;
}

std::string IP_Packet::print_source_addr()
{
    unsigned char *ip_byte = (unsigned char *)(&(this->ip_hdr->saddr));
    std::string saddr = "";
    for (int i = 0; i < 4; i++)
    {
        saddr += std::to_string(ip_byte[i]);
        if (i != 3)
        {
            saddr += ".";
        }
    }

    return saddr;
}

std::string IP_Packet::print_dest_addr()
{
    unsigned char *ip_byte = (unsigned char *)(&(this->ip_hdr->daddr));
    std::string daddr = "";
    for (int i = 0; i < 4; i++)
    {
        daddr += std::to_string(ip_byte[i]);
        if (i != 3)
        {
            daddr += ".";
        }
    }

    return daddr;
}

json IP_Packet::detailed_protocol_info_print()
{
    json IP_Packet;
    json Differentiated_Services_Field;
    json Flags;
    IP_Packet["version"] = "0100 .... = Version: 4";
    int ihl = this->ip_hdr->ihl;
    std::string ihl_string = std::to_string(ihl);

    std::string byte_string = to_binary_string(ihl, 4, true);
    IP_Packet["header_length"] = ".... " + byte_string + " = Header Length: " + std::to_string((ihl * 32) / 8) + " bytes (" + ihl_string + ")";
    // Map to the actual valeus on the wikipedia page https://en.wikipedia.org/wiki/Differentiated_services
    Differentiated_Services_Field["diff_services_codepoint"] = to_binary_string(this->ip_hdr->tos, 6, true);
    // Map to the actual valeus on the https://en.wikipedia.org/wiki/Explicit_Congestion_Notification
    Differentiated_Services_Field["explicit_congestion_notification"] = to_binary_string(this->ip_hdr->tos, 2, false);
    IP_Packet["total_length"] = "Total Length: " + ntohs(this->ip_hdr->tot_len);
}