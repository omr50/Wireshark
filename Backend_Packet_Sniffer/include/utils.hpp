#pragma once
#include <string>
#include <vector>

std::string timeval_to_string(timeval time);
std::vector<std::string> parse_line(std::string line);
std::string to_binary_string(int val, int num_bytes, bool from_end);
std::string to_hex(int val);
uint8_t binary_to_int(uint8_t value, uint8_t start_bit, uint8_t end_bit);
