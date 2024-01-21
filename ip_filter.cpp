#include "config.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

constexpr std::size_t nof_addr_groups = 4;
constexpr unsigned int _256_ = 256;
constexpr unsigned char FILT_ADR1 = 1;
constexpr unsigned char FILT_ADR2 = 46;
constexpr unsigned char FILT_ADR3 = 70;
constexpr unsigned char FILT_ADR4 = 46;

// A fixed size int array represents an IP address
using ip_addr_t = std::array<unsigned char, nof_addr_groups>;

// The type of the whole list of IP addresses
// is a vector of fixed size arrays
using pool_t = std::vector<ip_addr_t>;

// The storage for the numeric list of IP's
static pool_t ip_pool;

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

// Folds up all uchar (binary) parts of an IP address into a 4-bytes int
// to enable comparison
inline unsigned int make_int(const ip_addr_t &addr)
{
    unsigned int res = 0;
    for (auto i : addr)
        res = res * _256_ + i;
    return res;
}

// Console-outputs an IP-address vector
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

// Outputs the whole vector of IP-addresses,
// for each of provided lambda-ed output filters
template <typename... Lambda>
void multi_output_ip_pool(Lambda &&...filters)
{
    (void(std::for_each(ip_pool.begin(), ip_pool.end(), std::forward<Lambda>(filters))), ...);
}

// Inputs, sorts and filter-outputs IP-addresses
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
