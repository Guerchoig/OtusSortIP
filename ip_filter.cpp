#include "ip_filter.h"
#include "config.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

// Fill up the list of IPs
void fill_the_pool(pool_t &res)
{
    try
    {
        // std::ifstream in("input/ip_filter.tsv");
        // std::cin.rdbuf(in.rdbuf()); // redirect std::cin to input/ip_filter.tsv
        for (std::string line; std::getline(std::cin, line);)
        {
            ip_addr_t v{0};
            auto i = 0;
            for (std::string::size_type start = 0; start != std::string::npos; ++i)
            {
                auto stop = line.find_first_of("\t.", start);
                v.at(i) = std::stoi(line.substr(start, stop - start));
                if (line.at(stop) == '\t')
                    break;
                start = stop + 1;
            };
            res.push_back(v);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

// Outputs an IP-address to std::cout
void output_address(const ip_addr_t &addr)
{
    std::stringstream ss;
    for (auto triade : addr)
    {
        ss << static_cast<int>(triade) << '.';
    };
    auto s = ss.str();
    s = s.substr(0, s.size() - 1);
    std::cout << s << std::endl;
}

// Inputs, sorts and filter-outputs IP addresses
int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    // Makes the pool of IP-addresses in memory
    fill_the_pool(ip_pool);

    // Performs sorting
    std::sort(ip_pool.begin(), ip_pool.end(),
              [](const ip_addr_t &a, const ip_addr_t &b) -> bool
              {
                  return (make_int(a) > make_int(b));
              });

    // Does filtered output
    multi_output_ip_pool(
        [](auto &addr) -> void
        { output_address(addr); },

        [](auto &addr) -> void
        { if(addr[0] == FILT_ADR1) output_address(addr); },

        [](auto &addr) -> void
        { if(addr[0] == FILT_ADR2 && addr[1] == FILT_ADR3) output_address(addr); },

        [](auto &addr) -> void
        { bool res = false;
            for(auto i:addr)
                res = res || i == FILT_ADR4;
            if (res) output_address(addr); });
    return 0;
}
