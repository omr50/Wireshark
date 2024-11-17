#include <string>
#include <vector>

std::string timeval_to_string(timeval time)
{
    long long milliseconds = static_cast<long long>(time.tv_sec) * 1000 + time.tv_usec / 1000;
    return std::to_string(milliseconds);
}

std::vector<std::string> parse_line(std::string line)
{
    // separate by space
    std::string temp = "";
    std::vector<std::string> all_strings;
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == ' ')
        {
            all_strings.push_back(temp);
            temp = "";
        }
        temp += line[i];
    }

    return all_strings;
}