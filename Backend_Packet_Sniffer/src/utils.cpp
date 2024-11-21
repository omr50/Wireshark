#include "../include/utils.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>

std::string timeval_to_string(timeval time)
{
    long long milliseconds = static_cast<long long>(time.tv_sec) * 1000 + time.tv_usec / 1000;
    return std::to_string(milliseconds);
}

std::vector<std::string> parse_line(std::string line)
{
    // separate by space
    int space_count = 0;
    std::string temp = "";
    std::vector<std::string> all_strings;
    for (int i = 0; i < line.size(); i++)
    {
        if (space_count == 2)
        {
            temp = line.substr(i);
            all_strings.push_back(temp);
            break;
        }
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
        {

            all_strings.push_back(temp);
            space_count++;
            temp = "";
            while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
            {
                i++;
            }
        }
        temp += line[i];
    }

    printf("%s %s %s www\n", all_strings[0].c_str(), all_strings[1].c_str(), all_strings[2].c_str());
    return all_strings;
}

std::string to_binary_string(int val, int num_bytes, bool from_end)
{
    std::string byte_string;
    if (from_end)
    {
        for (int i = 0; i < num_bytes; i++)
        {
            byte_string += std::to_string((val & (1 << 7 - i)) >> (7 - i));
        }
    }
    else
    {
        for (int i = 0; i < num_bytes; i++)
        {
            byte_string = std::to_string((val & (1 << i)) >> (i)) + byte_string;
        }
    }

    return byte_string;
}

std::string to_hex(int val)
{
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    oss << "0x";
    oss << std::setw(2) << val;
    return oss.str();
}

uint8_t binary_to_int(uint8_t value, uint8_t start_bit, uint8_t end_bit)
{
    uint8_t int_val = 0;
    uint8_t scale = 1;
    for (int i = start_bit; i <= end_bit; i++)
    {
        // bit val either 1 or 0
        uint8_t shifted_bit = 1 << i;
        uint8_t bit_val = ((shifted_bit & value) >> i);
        int_val += (bit_val * scale);
        scale *= 2;
    }
    return int_val;
}