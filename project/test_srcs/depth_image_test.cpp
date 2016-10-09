#include <catch.hpp>
#include <depth_image.hpp>

#define PGM_FILE(x) "test_files/pgm/" x ".pgm"

TEST_CASE("DepthImages are constructed from PgmImages", "[DepthImage]") {
    std::vector<int> pixels = {1, 2, 3, 4};
    DepthImage i(PGM_FILE("quad_a"));
    REQUIRE(i.getHeight() == 2);
    REQUIRE(i.getWidth() == 2);
    REQUIRE(i.getPixelData() == pixels);
}

TEST_CASE("DepthImages compare minimum sums correctly", "[DepthImage]") {
    DepthImage img_a(PGM_FILE("quad_a"));
    DepthImage img_b(PGM_FILE("quad_b"));
    REQUIRE(img_a.minimumSumComparison(img_a) == 1.00);
    REQUIRE(img_a.minimumSumComparison(img_b) == 0.75);
}
