#include <catch.hpp>
#include <image_grouper.hpp>

TEST_CASE("ImageGrouper holds images", "[ImageGrouper]") {
    auto f = ImageDataset::fromFile("test_files/list/big.txt");
    ImageGrouper g{f};
    auto groups = g.getGroups();
    REQUIRE(groups.size() == f->size());
}

TEST_CASE("ImageGrouper groups images", "[ImageGrouper]") {
    auto f = ImageDataset::fromFile("test_files/list/big.txt");
    ImageGrouper g{f};

    SECTION("It reduces to the right size") {
        g.reduceToGroupCount(2);
        REQUIRE(g.getGroups().size() == 2);
    }

    SECTION("It merges properly initially") {
        g.reduceToGroupCount(4);
        REQUIRE(g.getGroups().size() == 4);
        auto indexes = g.getGroups()[0].getIndexes();
        REQUIRE(indexes.size() == 2);
        REQUIRE(indexes[0] == 0);
        REQUIRE(indexes[1] == 1);
        REQUIRE(indexes[0] != indexes[1]);
        auto &nb = g.getGroups()[0].getHistogram().getNormalizedBins();
        REQUIRE(nb[0] == 0.75);
        REQUIRE(nb[1] == 0.125);
        REQUIRE(nb[2] == 0.125);
    }

    SECTION("It merges multiple units properly") {
        g.reduceToGroupCount(3);
        REQUIRE(g.getGroups().size() == 3);
        auto group = g.getGroups()[0];
        auto indexes = group.getIndexes();
        REQUIRE(indexes[0] == 0);
        REQUIRE(indexes[1] == 1);
        REQUIRE(indexes[2] == 3);
        auto &nb = group.getHistogram().getNormalizedBins();
        REQUIRE(nb[0] == Approx(0.833333333333));
        REQUIRE(nb[1] == Approx(0.083333333333));
        REQUIRE(nb[2] == Approx(0.083333333333));
    }

    SECTION("It breaks on negative reduction") {
        REQUIRE_THROWS(g.reduceToGroupCount(-10));
    }

    SECTION("It breaks on reduction larger than max size") {
        REQUIRE_THROWS(g.reduceToGroupCount(1000));
    }
}
