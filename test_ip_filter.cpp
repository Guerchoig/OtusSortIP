#include "ip_filter_lib.h"
#include <gtest/gtest.h>
#include <fstream>
#include <numeric>
#include <filesystem>

static IPList list;
constexpr std::size_t nof_lines = 1000;

// Test input
TEST(test_ip_filter, InputAddresses)
{
    std::ifstream infile("test/ip_filter.tsv");

    // redirect std::cin to ip_filter.tsv
    std::cin.rdbuf(infile.rdbuf());

    // Call fill method
    IPList::fill_the_pool();

    // Check number of input lines
    auto ip_pool = list.get_IP_List();
    EXPECT_EQ(ip_pool.size(), nof_lines);
}

// Test sorting
TEST(test_ip_filter, Sorting)
{
    // Call sorting the pool
    IPList::sort_the_pool();

    // Check if every member is smaller than the previous
    auto ip_pool = list.get_IP_List();
    unsigned int prev = IPList::make_int(ip_pool.front());
    auto isGT = [&prev](bool acc, const ip_addr_t &el) -> bool
    {
        auto curr = IPList::make_int(el);
        acc = acc && prev >= curr;
        prev = curr;
        return acc;
    };
    bool res = std::accumulate(ip_pool.begin() + 1, ip_pool.end(), true, isGT);

    // Was ip_pool still there?
    EXPECT_EQ(ip_pool.size(), nof_lines);
    // Check the order
    EXPECT_EQ(res, true);
}

// Test output
TEST(test_ip_filter, Output)
{
    std::streambuf *cout_oldBuf = nullptr;
    std::string out_path("test/outfile.txt");
    std::ofstream out(out_path.c_str());

    // Saving old buf of std::cout
    cout_oldBuf = std::cout.rdbuf();

    // Redirecting std::cout to the file
    std::cout.rdbuf(out.rdbuf());

    // Output of IP addresses to the file
    IPList::output_the_pool();

    // Restoring old buf of std::cout
    std::cout.rdbuf(cout_oldBuf);

    // Getting the hash of the output file
    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setw(16)
       << std::hash<std::filesystem::path>{}(out_path.c_str());

    // Is ip_pool still there?
    auto ip_pool = list.get_IP_List();
    EXPECT_EQ(ip_pool.size(), nof_lines);

    // Verify the hash of the output file
    EXPECT_EQ(ss.str().compare(std::string("57E9E15335E082FF")), 0);

    // Deleating temporary file
    std::remove(out_path.c_str());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}