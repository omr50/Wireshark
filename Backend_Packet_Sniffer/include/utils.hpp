#include <string>

std::string timeval_to_string(timeval time)
{
    return std::to_string(time.tv_sec) + ":" + std::to_string(time.tv_usec);
}