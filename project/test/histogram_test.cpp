#include <catch.hpp>
#include <histogram.hpp>

TEST_CASE("Histograms fail on bad number") {
    std::vector<int> v = {1, 2, 9000};
    REQUIRE_THORWS_AS(Histogram(v), BadNumberError);
}
