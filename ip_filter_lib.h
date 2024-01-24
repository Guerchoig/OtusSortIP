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

class IPList
{
private:
    // Outputs the whole vector of IP-addresses,
    // - once for every provided filter
    template <typename... Lambda>
    static void multi_output_ip_pool(Lambda &&...filters)
    {
        (void(std::for_each(ip_pool.begin(), ip_pool.end(), std::forward<Lambda>(filters))), ...);
    }
    static void output_address(const ip_addr_t &addr);
    static pool_t ip_pool;

public:
    IPList() = default;
    ~IPList() = default;
    static void fill_the_pool();
    static void sort_the_pool();
    static void output_the_pool();
    static inline const pool_t &get_IP_List() { return ip_pool; };
    static inline unsigned int make_int(const ip_addr_t &addr)
    {
        unsigned int res = 0;
        for (auto i : addr)
            res = res * _256_ + i;
        return res;
    };
    static inline bool compare(const ip_addr_t &a, const ip_addr_t &b) { return (make_int(a) > make_int(b)); };
};
