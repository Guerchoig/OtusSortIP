#include "ip_filter_lib.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

// Storage for static members
static IPList list;
pool_t IPList::ip_pool;

// Fill up the list of IPs
void IPList::fill_the_pool()
{
    try
    {
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
            list.ip_pool.push_back(v);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

// Outputs an IP-address to std::cout
void IPList::output_address(const ip_addr_t &addr)
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

void IPList::output_the_pool()
{
    try
    {
        // Perform filtered output
        multi_output_ip_pool(
            [](auto &addr)
                -> void
            { output_address(addr); },

            [](auto &addr) -> void
            { if(addr[0] == FILT_ADR1) output_address(addr); },

            [](auto &addr) -> void
            { if(addr[0] == FILT_ADR2 && addr[1] == FILT_ADR3) output_address(addr); },

            [](auto &addr) -> void
            { bool res = false;
            for(auto i:addr)
                res = res || i == FILT_ADR4;
            if (res)
                IPList::output_address(addr); });
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void IPList::sort_the_pool()
{
    std::sort(list.ip_pool.begin(), list.ip_pool.end(), IPList::compare);
}
