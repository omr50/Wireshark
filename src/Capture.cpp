/*
    This class will iniitialize the packet capturer
    and then basically forward everything to the parser.
*/
#include "packet.hpp"
#include "./include/Capture.hpp"
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
}

void Capture::start_loop()
{
    int res = pcap_loop(this->handle, 0, packet_handler, NULL);
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
    // char errbuff[PCAP_ERRBUF_SIZE];
    // pcap_if_t *devices;

    // if (pcap_findalldevs(&devices, errbuff) == 0)
    // {
    //     printf("Successfully found devices!\n");
    // }
    // else
    // {
    //     printf("%s\n", errbuff);
    //     exit(0);
    // }

    this->get_all_devs();
    this->present_dev_options();

    printf("Enter the interface number (1-%d): ", this->total_devs);
    scanf("%d", &this->dev_num);
    this->select_interface();
    this->create_handle();
    this->change_filter_expression("port 8000");
    this->set_filter();

    // get the selected device
    // lo for loop back or any to capture on all

    // get network address and netmask (extra info but not needed)
    // if (pcap_lookupnet(d->name, &net, &mask, errbuff) == -1)
    // {
    //     fprintf(stderr, "Couldn't get netmask for device %s: %s\n", d->name, errbuff);
    //     net = 0;
    //     mask = 0;
    // }
    // printf("network lookup worked!\n");

    // compile filter (why does this take in the handle if we need to again use handle in the set filter
    // function below)

    // Set the filter
    // make sure to run with correct
    // privileges to avoid errors.
    // Start capturing packets
    // pcap_loop(handle, 10, packet_handler, NULL);
    int res = pcap_loop(handle, 0, packet_handler, NULL);
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