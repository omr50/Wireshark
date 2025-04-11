#include "../include/PacketClasses/UDP_Packet.hpp"
#include "../include/utils.hpp"

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
    return detailed_protocol_info_print();
}

json UDP_Packet::detailed_protocol_info_print()
{

    json UDP_Packet;
    json time;
    uint16_t src_port = ntohs(udp_hdr->source);
    uint16_t dest_port = ntohs(udp_hdr->dest);
    uint16_t length = ntohs(udp_hdr->len);
    uint16_t checksum = ntohs(udp_hdr->check);
    uint16_t payload_size = ntohs(udp_hdr->len) - sizeof(udp_hdr);

    std::string src_string = std::to_string(src_port);
    std::string dest_string = std::to_string(dest_port);
    std::string length_string = std::to_string(length);
    std::string checksum_string = std::to_string(checksum);
    std::string payload = toHex(reinterpret_cast<uint8_t *>(this->udp_hdr + 1), payload_size);
    size_t packet_size = sizeof(ether_header) + sizeof(iphdr) + sizeof(udp_hdr) + payload_size;
    // get the entire packet in hexadecimal.
    std::string entirePacketHex = toHex(reinterpret_cast<uint8_t *>((void *)this->udp_hdr + ntohs(udp_hdr->len) - packet_size), packet_size);

    UDP_Packet["title"] = "User Datagram Protocol, Src Port: " + src_string + ", Dst Port: " + dest_string;
    UDP_Packet["Source_Port"] = "Source Port: " + src_string;
    UDP_Packet["Destination_Port"] = "Destination Port: " + dest_string;
    UDP_Packet["Length"] = "Length: " + length_string;
    UDP_Packet["Checksum"] = "Checksum: " + checksum_string + " [unverified]";
    UDP_Packet["Checksum_status"] = "[Checksum Status: Unverified]";
    UDP_Packet["Stream_Index"] = "[Stream index: 0 (change later)]";
    /*
    For the time stats basically just keep an in memory
    log of the times of each frame, and basically each
    (src ip, src port), (dest ip, dest port) mapping will
    have its own log and basically you can easily keep track
    of these statistics.

    */
    time["title"] = "[Timestamps]";
    time["first_frame"] = "[Time since first Frame: 123456789(change later) seconds]";
    time["prev_frame"] = "[Time since previous Frame: 123456789(change later) seconds]";
    UDP_Packet["Timestamps"] = time;
    UDP_Packet["UDP_Payload"] = "UDP payload (" + std::to_string(payload_size) + ")";
    UDP_Packet["data"] = payload;
    UDP_Packet["all_data"] = entirePacketHex;

    return UDP_Packet;
}