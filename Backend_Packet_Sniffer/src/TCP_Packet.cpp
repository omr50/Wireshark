#include "../include/PacketClasses/TCP_Packet.hpp"

TCP_Packet::TCP_Packet(const u_char *data, size_t length, timeval time_stamp)
{
    printf("IP Packet Created!\n");
    this->start_data = (u_char *)data;
    this->tcp_hdr = (tcphdr *)data;
    this->data_length = length;
    this->timestamp = time_stamp;
    this->packet_type = "TCP";
    this->layer = 4;
}

void TCP_Packet::parse()
{
    uint16_t dst_port = ntohs(this->tcp_hdr->th_dport);
    // http can also be on other ports (have to modify this later)
    if (dst_port == 80)
    {
        // CREATE ICMP PACKET
        printf("THIS IS HTTP PORT IS 80!\n");
    }
    else if (dst_port == 443)
    {
        printf("This is HTTPS PORT IS 443!\n");
    }
    else
    {
        printf("Source port is %d\n", dst_port);
    }
}

json TCP_Packet::print()
{
    std::string msg((char *)this->tcp_hdr, this->data_length);
    return msg;
}

json TCP_Packet::detailed_protocol_info_print()
{
    // Fields
    /*
    - src port
    - dest port
    - sequence num
    - ack num
    - data offset
    - reserved
    - flags
        - cwr
        - ece
        - urg
        - ack
        - psh
        - rst
        - syn
        - fin
    - window
    - checksum
    - urgent pointer
    - options
    - data

    */

    json UDP_Packet;
    json time;
    uint16_t src_port = ntohs(tcp_hdr->source);
    uint16_t dest_port = ntohs(tcp_hdr->dest);
    uint32_t seq_num = ntohl(tcp_hdr->seq);
    uint32_t ack_num = ntohl(tcp_hdr->ack_seq);
    // doff and res1 need to first be extracted
    // from the converted little endian short
    // The highest bit will contain reserved and
    // doff while the lowest byte contains the flags.
    uint16_t flags = ntohs(*(uint16_t *)((void *)tcp_hdr + 12));
    uint8_t data_offset = flags >> 12;
    uint8_t reserved = flags >> 8 & 0xff;

    // 1 bit flags
    uint8_t fin = flags & 1;
    uint8_t syn = flags >> 1 & 1;
    uint8_t rst = flags >> 2 & 1;
    uint8_t psh = flags >> 3 & 1;
    uint8_t ack = flags >> 4 & 1;
    uint8_t urg = flags >> 5 & 1;
    uint8_t res2 = flags >> 6 & 1;

    std::string src_string = std::to_string(src_port);
    std::string dest_string = std::to_string(dest_port);
    std::string seq_string = std::to_string(seq_num);
    std::string ack_string = std::to_string(ack_num);
    std::string data_offset_string = std::to_string(data_offset);

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