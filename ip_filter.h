#pragma once
#include <vector>
#include <array>
#include <utility>

constexpr std::size_t nof_addr_groups = 4;
constexpr unsigned int _256_ = 256;

constexpr unsigned char FILT_ADR1 = 1;
constexpr unsigned char FILT_ADR2 = 46;
constexpr unsigned char FILT_ADR3 = 70;
constexpr unsigned char FILT_ADR4 = 46;

// The type of one IP address (
// * no need to build a special container
// * can be used as an element of a vector
using ip_addr_t = std::array<unsigned char, nof_addr_groups>; // size = nof_addr_groups

// The type of the whole list of IP addresses
using pool_t = std::vector<ip_addr_t>;

// The storage for the numeric list of IPs
static pool_t ip_pool;

// Outputs the whole vector of IP-addresses,
// - once for every provided filter
template <typename... Lambda>
void multi_output_ip_pool(Lambda &&...filters)
{
    (void(std::for_each(ip_pool.begin(), ip_pool.end(), std::forward<Lambda>(filters))), ...);
}

// Forms up a a 4-bytes uint representation of an IP address
// to enable comparison of addresses
inline unsigned int make_int(const ip_addr_t &addr)
{
    unsigned int res = 0;
    for (auto i : addr)
        res = res * _256_ + i;
    return res;
}