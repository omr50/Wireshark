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
    int space_count = 0;
    std::string temp = "";
    std::vector<std::string> all_strings;
    for (int i = 0; i < line.size(); i++)
    {
        if (space_count == 2)
        {
            temp = line.substr(i);
            break;
        }
        if (line[i] == ' ')
        {

            all_strings.push_back(temp);
            space_count++;
            temp = "";
            while (line[i] == ' ')
            {
                i++;
            }
        }
        temp += line[i];
    }

    printf("%s %s %s\n", all_strings[0].c_str(), all_strings[1].c_str(), all_strings[2].c_str());
    return all_strings;
}