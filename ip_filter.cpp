// #include "config.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>

constexpr int nof_addr_groups = 4;
constexpr unsigned int _256_ = 256;
using ip_addr_t = std::vector<unsigned char>;
using pool_t = std::vector<ip_addr_t>;

static pool_t ip_pool;

void fill_in_the_pool(pool_t &res)
{
    try
    {
        std::ifstream in("input/ip_filter.tsv");
        std::cin.rdbuf(in.rdbuf()); // redirect std::cin to input/ip_filter.tsv
        for (std::string line; std::getline(std::cin, line);)
        {
            ip_addr_t v(nof_addr_groups, 0);
            ip_addr_t::iterator i = v.begin();
            for (std::string::size_type start = 0; start != std::string::npos; ++i)
            {
                auto stop = line.find_first_of("\t.", start);
                *i = std::stoi(line.substr(start, stop - start));
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

unsigned int make_int(const ip_addr_t &addr)
{
    return std::accumulate(addr.begin(), addr.end(), 0, [](unsigned int acc, unsigned char it)
                           { return acc * _256_ + it; });
}

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

template <typename... Lambda>
void multi_output_ip_pool(Lambda &&...filters)
{
    (void(std::for_each(ip_pool.begin(), ip_pool.end(), std::forward<Lambda>(filters))), ...);
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    fill_in_the_pool(ip_pool);

    std::sort(ip_pool.begin(), ip_pool.end(),
              [](const ip_addr_t &a, const ip_addr_t &b) -> bool
              {
                  return (make_int(a) > make_int(b));
              });

    multi_output_ip_pool(
        [](auto addr) -> void
        { output_address(addr); },

        [](auto addr) -> void
        { if(addr[0] == 1) output_address(addr); },

        [](auto addr) -> void
        { if(addr[0] == 46 && addr[1] == 70) output_address(addr); },

        [](auto addr) -> void
        { if (std::accumulate(addr.begin(), addr.end(), false, [](bool acc, unsigned char it)
                                       { return acc || it == 46; })) output_address(addr); });
    return 0;
}
