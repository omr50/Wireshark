#include "../include/PacketClasses/TCP_Packet.hpp"
#include "../include/utils.hpp"

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
    return detailed_protocol_info_print();
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

    json TCP_Packet;
    json time;

    size_t packet_size = sizeof(ether_header) + sizeof(iphdr) + sizeof(tcphdr);
    std::string entirePacketHex = toHex(reinterpret_cast<uint8_t *>((void *)this->tcp_hdr + sizeof(tcphdr) - packet_size), packet_size);

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
    uint8_t reserved = (flags >> 8 & 0xf) >> 1;
    uint8_t ecn = (flags >> 8 & 0x1);

    uint16_t flag_big_endian = (flags & 0xff) | (reserved << 8);
    // 1 bit flags
    uint8_t fin = flags & 1;
    uint8_t syn = flags >> 1 & 1;
    uint8_t rst = flags >> 2 & 1;
    uint8_t psh = flags >> 3 & 1;
    uint8_t ack = flags >> 4 & 1;
    uint8_t urg = flags >> 5 & 1;
    uint8_t ece = flags >> 6 & 1;
    uint8_t cwr = flags >> 7 & 1;

    uint16_t window = ntohs(tcp_hdr->window);
    uint16_t check = ntohs(tcp_hdr->check);
    uint16_t urg_ptr = ntohs(tcp_hdr->urg_ptr);

    std::string src_string = std::to_string(src_port);
    std::string dest_string = std::to_string(dest_port);
    std::string seq_string = std::to_string(seq_num);
    std::string ack_string = std::to_string(ack_num);
    std::string data_offset_hex_string = to_hex(flags);
    std::string data_offset_binary_string = to_binary_string(data_offset, 4, false);
    std::string reserved_string = to_binary_string(reserved, 3, false);
    std::string flags_big_endian_string = to_hex(flag_big_endian);
    std::string window_string = "Window: " + std::to_string(window);
    std::string checksum_string = "Checksum: " + std::to_string(check) + "[unverified]";
    std::string urgent_pointer_string = "Urgent Pointer: " + std::to_string(urg_ptr);

    TCP_Packet["title"] = "Transmission Control Protocol, Src Port: " + src_string + ", Dst Port: " + dest_string + ", Seq: " + seq_string + ", Ack: " + ack_string;
    TCP_Packet["Source_Port"] = "Source Port: " + src_string;
    TCP_Packet["Destination_Port"] = "Destination Port: " + dest_string;
    TCP_Packet["Sequence_Number"] = "Sequence Number (raw): " + seq_string;
    TCP_Packet["Acknowledgement_Number"] = "Acknowledgement Number (raw): " + ack_string;
    TCP_Packet["Header_Length"] = data_offset_binary_string + " .... = Header Length: " + std::to_string(4 * data_offset) + " bytes (" + std::to_string(data_offset) + ")";

    json Flags;
    Flags["Title"] = "Flags: " + flags_big_endian_string;
    Flags["Reserved"] = reserved_string + ". .... .... = Reserved: " + set_not_set(reserved);
    Flags["ECN"] = "..." + std::to_string(ecn) + " .... .... = Accurate ECN: " + set_not_set(ecn);
    Flags["Congestion_Window"] = ".... " + std::to_string(cwr) + "... .... = Congestion Window Reduction: " + set_not_set(cwr);
    Flags["ECN-Echo"] = ".... ." + std::to_string(ece) + ".. .... = ECN-Echo: " + set_not_set(ece);
    Flags["Urgent"] = ".... .." + std::to_string(urg) + ". .... = Urgent: " + set_not_set(urg);
    Flags["Acknowledgement"] = ".... ..." + std::to_string(ack) + " .... = Acknowledgement: " + set_not_set(ack);
    Flags["Push"] = ".... .... " + std::to_string(psh) + "... = Push: " + set_not_set(psh);
    Flags["Reset"] = ".... .... ." + std::to_string(rst) + ".. = Reset: " + set_not_set(rst);
    Flags["Syn"] = ".... .... .." + std::to_string(syn) + ". = Syn: " + set_not_set(syn);
    Flags["Fin"] = ".... .... ..." + std::to_string(fin) + " = Fin: " + set_not_set(fin);

    TCP_Packet["Flags"] = Flags;

    TCP_Packet["Window"] = window_string;
    TCP_Packet["Checksum"] = checksum_string;
    TCP_Packet["Urgent_Pointer"] = urgent_pointer_string;
    TCP_Packet["all_data"] = entirePacketHex;
    /*
        For the time stats basically just keep an in memory
        log of the times of each frame, and basically each
        (src ip, src port), (dest ip, dest port) mapping will
        have its own log and basically you can easily keep track
        of these statistics.

        */

    return TCP_Packet;
}