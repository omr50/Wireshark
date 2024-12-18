#pragma once

#include "parser.hpp"
#include <string>
#include "TCP_Server.hpp"

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