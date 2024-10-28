#include "../include/PacketClasses/packet.hpp"

void Packet::copy_data(const u_char *data, size_t length)
{
    this->data_length = length;
    this->start_data = (u_char *)malloc(length);
    for (int i = 0; i < this->data_length; i++)
    {
        this->start_data[i] = data[i];
    }
}