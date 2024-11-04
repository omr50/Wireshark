/*
    This class will iniitialize the packet capturer
    and then basically forward everything to the parser.
*/
#include "../include/PacketClasses/packet.hpp"
#include "../include/Capture.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>

Capture::Capture(std::string filter_exp, std::shared_ptr<TCP_Server> server) : filter_exp(filter_exp), server(server)
{
    printf("Capture begin!\n");
}

void Capture::start()
{
    this->get_all_devs();
    this->present_dev_options();

    printf("Enter the interface number (1-%d): ", this->total_devs);
    scanf("%d", &this->dev_num);

    this->select_interface();
    this->create_handle();
    this->change_filter_expression(this->filter_exp.c_str());
    this->compile_filter();
    this->set_filter();

    // if datalink = 1 then we have ethernet as the link layer
    int datalink = pcap_datalink(handle);
    printf("Data Link Type: %d\n", datalink);

    this->start_loop();
}

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
    printf("Going to send \n");
    std::shared_ptr<TCP_Server> server = *((std::shared_ptr<TCP_Server> *)userData);
    std::shared_ptr<Packet> root_packet = Parser::Determine_Packet(pkthdr, packet);
    // after determining packet, we will inform the server of the data we want to send.
    // for now send the pure binary string
    std::string packet_data = root_packet->print();
    // queue up string data to be sent through tcp server.
    // basically call post here, the server pointer has access to the io_context, and socket, and other things
    printf("Packet DATA: %d\n", packet_data.size());
    auto self = server->shared_from_this();
    if (server->client_connected)
    {
        printf("Posted?\n");

        server->io_context.post([self, packet_data]()
                                { printf("Before write\n"); self->start_write(packet_data); printf("Posted to client?\n"); });
        if (server->io_context.stopped())
        {
            std::cout << "io_context has been stopped." << std::endl;
            // server->io_context.run();
        }
        else
        {
            std::cout << "io_context is running." << std::endl;
        }
    }
}

void Capture::start_loop()
{
    int res = pcap_loop(this->handle, 0, packet_handler, (u_char *)(&(this->server)));
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
