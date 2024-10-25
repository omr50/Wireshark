#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <err.h>
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet);

int main()
{
}
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
    printf("Packet captured with length: %d\n", header->len);
}