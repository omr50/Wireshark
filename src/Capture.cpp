/*
    This class will iniitialize the packet capturer
    and then basically forward everything to the parser.
*/
#include "../include/packet.hpp"
#include "../include/Capture.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>

void Capture::get_all_devs()
{
    char errbuff[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&this->devices, errbuff) == 0)
    {
        printf("Successfully found devices!\n");
    }
    else
    {
        printf("%s\n", errbuff);
        exit(0);
    }

    printf("Obtained all devices successfully!\n");
}

void Capture::select_interface()
{

    if (dev_num < 1 || dev_num > this->total_devs)
    {
        fprintf(stderr, "Invalid device number.\n");
        pcap_freealldevs(devices);
        exit(1);
    }
    pcap_if_t *d;
    int i = 0;
    for (d = this->devices; d != NULL; d = d->next)
    {
        if (i + 1 == this->dev_num)
        {
            this->active_device = d;
            break;
        }
        i++;
    }
    printf("Selected device: %s successfully!\n", this->active_device->name);
}

void Capture::present_dev_options()
{
    this->total_devs = 0;
    for (pcap_if_t *d = this->devices; d != NULL; d = d->next)
    {
        printf("%d. %s", ++this->total_devs, d->name);
        if (d->description)
            printf(" (%s)\n", d->description);
        else
            printf(" (No description available)\n");
    }
    printf("Presented all devices successfully!\n");
}
void Capture::change_filter_expression(std::string new_filter_expression)
{
    this->filter_exp = new_filter_expression;
    printf("Changed filter expression to '%s' successfully!\n", new_filter_expression.c_str());
}

void Capture::create_handle()
{

    // Open the device
    // max size = 65536 to ensure we can capture all packets correctly
    // promiscuous mode = 1 = true (sniff all packets)
    // timeout in milliseconds = 1000
    char errbuff[PCAP_ERRBUF_SIZE];
    this->handle = pcap_open_live(this->active_device->name, 65536, 1, 1000, errbuff);
    printf("handle opened successfully!\n");

    if (handle == NULL)
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", this->active_device->name, errbuff);
        pcap_freealldevs(devices);
        exit(1);
    }

    printf("Created handle successfully!\n");
}

void Capture::compile_filter()
{
    if (pcap_compile(this->handle, &this->fp, this->filter_exp.c_str(), 0, net) == -1)
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        pcap_freealldevs(devices);
        pcap_close(handle);
        exit(1);
    }
    printf("Compiled bpf successfully!\n");
}

void Capture::set_filter()
{
    if (pcap_setfilter(this->handle, &this->fp) == -1)
    {
        fprintf(stderr, "Couldn't install filter %s: %s\n", this->filter_exp, pcap_geterr(this->handle));
        pcap_freealldevs(this->devices);
        pcap_close(this->handle);
        exit(1);
    }
    printf("Correctly set filter!\n");
}

void Capture::packet_handler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    printf("Packet handler currently working!\n");
    printf("Packet header size = %d\n", pkthdr->len);
    // cast to eth header
    ether_header *eth_hdr = (ether_header *)packet;

    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth_hdr->ether_dhost[0], eth_hdr->ether_dhost[1], eth_hdr->ether_dhost[2],
           eth_hdr->ether_dhost[3], eth_hdr->ether_dhost[4], eth_hdr->ether_dhost[5]);

    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth_hdr->ether_shost[0], eth_hdr->ether_shost[1], eth_hdr->ether_shost[2],
           eth_hdr->ether_shost[3], eth_hdr->ether_shost[4], eth_hdr->ether_shost[5]);

    u_short ether_type = ntohs(eth_hdr->ether_type);
    printf("EtherType: 0x%04x\n", ether_type);
}

void Capture::start_loop()
{
    int res = pcap_loop(this->handle, 0, this->packet_handler, NULL);
    if (res == -1)
    {
        fprintf(stderr, "Error occurred in pcap_loop: %s\n", pcap_geterr(handle));
    }
    else if (res == -2)
    {
        fprintf(stderr, "pcap_breakloop() was called; loop terminated.\n");
    }
    else
    {
        printf("pcap_loop() exited with code %d\n", res);
    }

    pcap_freecode(&fp); // free compiled filter   pcap_freecode(&fp); // free compiled filter
}

Capture::Capture()
{
    this->get_all_devs();
    this->present_dev_options();

    printf("Enter the interface number (1-%d): ", this->total_devs);
    scanf("%d", &this->dev_num);

    this->select_interface();
    this->create_handle();
    this->change_filter_expression("tcp");
    this->compile_filter();
    this->set_filter();
    int datalink = pcap_datalink(handle);
    printf("Data Link Type: %d\n", datalink);

    this->start_loop();
}