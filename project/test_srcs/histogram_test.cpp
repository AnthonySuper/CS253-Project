#include <catch.hpp>
#include <histogram.hpp>

TEST_CASE("Histograms fail on numbers that are too big", "[Histogram]") {
    std::vector<int> v = {1, 2, 9000};
    REQUIRE_THROWS_AS(new Histogram(v), BadNumberError);
}

TEST_CASE("Histograms fail on numbers that are too small", "[Histogram]") {
    std::vector<int> v = {1, 2, -100};
    REQUIRE_THROWS_AS(new Histogram(v), BadNumberError);
}

TEST_CASE("Histograms return normalized binned numbers properly", 
        "[Histogram]") {
    std::vector<int> v = {1, 5, 9, 14};
    Histogram h(v);
    auto q = h.getNormalizedBins();
    REQUIRE(q[0] == 0.25);
    REQUIRE(q[1] == 0.25);
    REQUIRE(q[2] == 0.25);
    REQUIRE(q[3] == 0.25);
}
