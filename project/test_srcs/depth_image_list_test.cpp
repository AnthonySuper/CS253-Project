#include <catch.hpp>
#include <depth_image_list.hpp>

TEST_CASE("It reads from a filename", "[DepthImageList]") {
    auto q = "test_files/list/good.txt";
    REQUIRE_NOTHROW(auto d = DepthImageList::fromFile(q));
}

TEST_CASE("Errors are propogated", "[DepthImageList]") {
    auto q = "test_files/list/bad.txt";
    REQUIRE_THROWS(auto d = DepthImageList::fromFile(q));
}

TEST_CASE("It works properly", "[DepthImageList]") {
    auto q = "test_files/list/big.txt";
    auto d = DepthImageList::fromFile(q);
    auto& s = d.calculateNearestNeighbors();
    REQUIRE(s[0].getNearestNeighborIndex() == 1);
    REQUIRE(s[1].getNearestNeighborIndex() == 0);
    REQUIRE(s[2].getNearestNeighborIndex() == 0);
    REQUIRE(s[3].getNearestNeighborIndex() == 1);
    REQUIRE(s[4].getNearestNeighborIndex() == 1);
}
