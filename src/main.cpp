#include <stdio.h>
#include <stdlib.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <pcap/pcap.h>
#include <err.h>

int main()
{
    char errbuff[PCAP_ERRBUF_SIZE];
    pcap_if_t *devices;

    if (pcap_findalldevs(&devices, errbuff) == 0)
    {
        printf("Successfully found devices!\n");
    }
    else
    {
        printf("%s\n", errbuff);
        exit(0);
    }

    int i = 0;
    for (pcap_if_t *d = devices; d != NULL; d = d->next)
    {
        printf("%d. %s", ++i, d->name);
        if (d->description)
            printf(" (%s)\n", d->description);
        else
            printf(" (No description available)\n");
    }

    printf("Working?\n");

    int dev_num;
    printf("Enter the interface number (1-%d): ", i);
    scanf("%d", &dev_num);

    if (dev_num < 1 || dev_num > i)
    {
        fprintf(stderr, "Invalid device number.\n");
        pcap_freealldevs(devices);
        exit(1);
    }

    // get the selected device
    pcap_if_t *d;
    for (d = devices, i = 1; i < dev_num; d = d->next, i++)
        ;

    printf("Selected device: %s\n", d->name);

    pcap_t *handle;

    // Open the device
    // max size = 65536 to ensure we can capture all packets correctly
    // promiscuous mode = 1 = true (sniff all packets)
    // timeout in milliseconds = 1000
    // error buffer
    handle = pcap_open_live(d->name, 65536, 1, 1000, errbuff);
    printf("handle opened successfully!\n");

    if (handle == NULL)
    {
        fprintf(stderr, "Couldn't open device %s: %s\n", d->name, errbuff);
        pcap_freealldevs(devices);
        exit(1);
    }

    // create bpf program to indicate what we want to capture
    struct bpf_program fp;
    char filter_exp[] = "dst port 8000"; // filter expression
    bpf_u_int32 net = 0, mask = 0;

    // get network address and netmask (extra info but not needed)
    if (pcap_lookupnet(d->name, &net, &mask, errbuff) == -1)
    {
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", d->name, errbuff);
        net = 0;
        mask = 0;
    }
    printf("network lookup worked!\n");

    // compile filter (why does this take in the handle if we need to again use handle in the set filter
    // function below)
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
    {
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        pcap_freealldevs(devices);
        pcap_close(handle);
        exit(1);
    }

    printf("Compiled bpf successfully\n");

    // Set the filter
    // make sure to run with correct
    // privileges to avoid errors.
    if (pcap_setfilter(handle, &fp) == -1)
    {
        fprintf(stderr, "Couldn't install iflter %s: %s\n", filter_exp, pcap_geterr(handle));
        pcap_freealldevs(devices);
        pcap_close(handle);
        exit(1);
    }

    pcap_freecode(&fp); // free compiled filter
}