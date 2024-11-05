#include <string>

std::string timeval_to_string(timeval time)
{
    long long milliseconds = static_cast<long long>(time.tv_sec) * 1000 + time.tv_usec / 1000;
    return std::to_string(milliseconds);
}