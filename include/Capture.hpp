#pragma once

#include "parser.hpp"
#include <string>

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

    Parser parser;
    Capture();
    void get_all_devs();
    void select_interface();
    void present_dev_options();
    void change_filter_expression(std::string new_filter_expression);
    // when new setting are added (bpf, changed device, etc.) restart
    void restart_sniffer();
};