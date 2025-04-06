#pragma once

#include "parser.hpp"
#include <string>
#include "TCP_Server.hpp"
#include <unordered_map>

struct FourTuple
{

    uint32_t srcIP;
    uint32_t dstIP;
    uint16_t srcPort;
    uint16_t dstPort;

    // equality operator
    bool operator==(const FourTuple &other) const
    {
        return (srcIP == other.srcIP &&
                dstIP == other.dstIP &&
                srcPort == other.srcPort &&
                dstPort == other.dstPort);
    }
};

struct FourTupleHash
{
    std::size_t operator()(const FourTuple &key) const
    {
        // hash each field
        // then combine each one
        // into a unique hash

        std::size_t h1 = std::hash<uint32_t>()(key.srcIP);
        std::size_t h2 = std::hash<uint32_t>()(key.dstIP);
        std::size_t h3 = std::hash<uint32_t>()(key.srcPort);
        std::size_t h4 = std::hash<uint32_t>()(key.dstPort);

        // combining function

        auto combine = [](std::size_t seed, std::size_t value)
        {
            const std::size_t magic = 0x7e3879bc7f3a7d13ULL;
            seed ^= (value + magic + (seed << 6) + (seed >> 2));
            return seed;
        };

        std::size_t result = h1;
        result = combine(result, h2);
        result = combine(result, h3);
        result = combine(result, h4);

        return result;
    }
};

class Capture
{
    // this class should help set up the filter and have a
    // stable consistent capture
public:
    pcap_if_t *devices;
    pcap_if_t *active_device;
    int dev_num;
    int total_devs;
    pcap_t *handle;
    std::string filter_exp;
    struct bpf_program fp;
    bpf_u_int32 net = 0, mask = 0;
    std::shared_ptr<TCP_Server> server;
    // collection of all udp streams and their packets
    std::unordered_map<FourTuple, std::vector<std::shared_ptr<Packet>>, FourTupleHash> UDP_Packet_Stream;

    Capture(std::string filter_exp, std::shared_ptr<TCP_Server> server);
    void start();
    void get_all_devs();
    void select_interface();
    void present_dev_options();
    void change_filter_expression(std::string new_filter_expression);
    void create_handle();
    void compile_filter();
    void set_filter();
    void start_loop();
    // when new setting are added (bpf, changed device, etc.) restart
    void restart_sniffer();
    static void packet_handler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet);
};