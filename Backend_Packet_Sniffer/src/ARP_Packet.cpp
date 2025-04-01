#include "../include/PacketClasses/ARP_Packet.hpp"
#include <iostream>
#include <fstream>

ARP_Packet::ARP_Packet(const u_char *data, size_t length, timeval time_stamp)

{

    this->start_data = (u_char *)data;
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
    return this->detailed_protocol_info_print();
}

json ARP_Packet::detailed_protocol_info_print()
{
    json ARP_Packet;
    int opcode = ntohs(this->arp_hdr->ea_hdr.ar_op);
    std::string opcode_string = ((opcode == 1) ? "request" : "response");
    int hardware_type = ntohs(this->arp_hdr->ea_hdr.ar_hrd);
    std::ofstream arp_binary_file("arp_binary_file.bin", std::ios::out | std::ios::binary);
    arp_binary_file.write((char *)this->arp_hdr, sizeof(struct ether_arp));

    std::cout << "ar_hrd (raw, network byte order): 0x" << std::hex << ntohs(this->arp_hdr->ea_hdr.ar_hrd) << std::endl;

    std::string hardware_type_string = ((hardware_type == 1) ? "Ethernet (1)" : "Unknown Hardware Type");
    int protocol = ntohs(this->arp_hdr->ea_hdr.ar_pro);
    std::string protocol_type_string = ((protocol == 0x0800) ? "IPv4 (0x0800)" : "Unknown Target Protocol");

    int hardware_protocol_size = this->arp_hdr->ea_hdr.ar_hln;
    int target_protocol_size = this->arp_hdr->ea_hdr.ar_pln;

    std::string sender_hardware_address;
    std::string target_hardware_address;
    std::string sender_protocol_address = this->print_protocol_address(this->arp_hdr->arp_spa);
    std::string target_protocol_address = this->print_protocol_address(this->arp_hdr->arp_tpa);
    if ((sender_hardware_address = this->print_hardware_address(this->arp_hdr->arp_sha)) == "00:00:00:00:00:00")
    {
        sender_hardware_address = "Broadcast";
    }

    if ((target_hardware_address = this->print_hardware_address(this->arp_hdr->arp_tha)) == "00:00:00:00:00:00")
    {
        target_hardware_address = "Broadcast";
    }

    ARP_Packet["title"] = "Address Resolution Protocol (" + opcode_string + ")";
    ARP_Packet["hardware_type"] = "Hardware type: " + hardware_type_string;
    ARP_Packet["protocol_type"] = "Protocol type: " + protocol_type_string;
    ARP_Packet["hardware_size"] = "Hardware size: " + std::to_string(hardware_protocol_size);
    ARP_Packet["target_size"] = "Protocol size: " + std::to_string(target_protocol_size);
    ARP_Packet["opcode"] = opcode_string + " (" + std::to_string(opcode) + ")";
    ARP_Packet["sender_mac"] = "Sender MAC address: " + sender_hardware_address + "(placeholder also add manuf prefix)";
    ARP_Packet["sender_ip"] = "Sender IP address: " + sender_protocol_address;
    ARP_Packet["target_mac"] = "Target MAC address: " + target_hardware_address + "(placeholder also add manuf prefix)";
    ARP_Packet["target_ip"] = "Target IP address: " + target_protocol_address;

    return ARP_Packet;
}

std::string ARP_Packet::print_hardware_address(uint8_t addr_arr[6])
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (int i = 0; i < 6; i++)
    {
        oss << std::setw(2) << static_cast<int>(addr_arr[i]);
        if (i != 5)
            oss << ":";
    }
    return oss.str();
}

std::string ARP_Packet::print_protocol_address(uint8_t addr_arr[4])
{
    std::string daddr = "";
    for (int i = 0; i < 4; i++)
    {
        daddr += std::to_string(addr_arr[i]);
        if (i != 3)
        {
            daddr += ".";
        }
    }

    return daddr;
}