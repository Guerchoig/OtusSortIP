#include "ip_filter_lib.h"
#include "config.h"

// Inputs, sorts and filter-outputs IP addresses
int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    IPList ip_list;

    // Makes the pool of IP-addresses in memory
    ip_list.fill_the_pool();

    // Performs sorting
    ip_list.sort_the_pool();

    // Perform output
    ip_list.output_the_pool();

    return 0;
}
