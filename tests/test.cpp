#include <string>
#include <sstream>

#include <asdf.hpp>

#include "gtest/gtest.h"

extern std::string test_data_path;
using namespace Asdf;

TEST(ReaderTest, Tag)
{
    Asdf::AsdfFile asdf(test_data_path + std::string("test.asdf"));
    Asdf::Node tree = asdf.get_tree();
    EXPECT_EQ("tag:stsci.edu:asdf/core/asdf-1.1.0", tree.Tag());
}

TEST(ReaderTest, FromStream)
{
    AsdfFile asdf;
    Node tree = asdf.get_tree();
    tree["top"] = "hello";

    std::vector<int> nums;
    for (int i = 0; i < 100; i++)
    {
        nums.push_back(i);
    }

    tree["nums"] = NDArray<int>(nums.data(), nums.size());

    std::stringstream asdf_stream;
    asdf_stream << asdf;

    AsdfFile new_asdf(asdf_stream);
    Node new_tree = new_asdf.get_tree();
    EXPECT_EQ(new_tree["top"].as<std::string>(), "hello");

    auto num_array = new_tree["nums"].as<NDArray<int>>();
    int *data = num_array.read();
    for (int i = 0; i < 100; i++)
    {
        EXPECT_EQ(data[i], i);
    }
}
