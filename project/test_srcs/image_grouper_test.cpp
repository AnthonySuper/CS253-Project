#include <catch.hpp>
#include <image_grouper.hpp>
#include <set>
#include <stdexcept>

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
        g.reduceToGroupCount(5);
        REQUIRE(g.getGroups().size() == 5);
        auto indexes = g.getGroups()[0].getIndexes();
        REQUIRE(indexes.size() == 2);
        std::set<int> real{indexes.begin(), indexes.end()};
        std::set<int> desired{0, 1};
        REQUIRE(desired == real);
        auto &nb = g.getGroups()[0].getHistogram().getNormalizedBins();
        REQUIRE(nb[0] == 0.75);
        REQUIRE(nb[1] == 0.125);
        REQUIRE(nb[2] == 0.125);
    }

    SECTION("It merges groups based on closest distances") {
        g.reduceToGroupCount(3);
        REQUIRE(g.getGroups().size() == 3);
        auto indexes1 = g.getGroups()[0].getIndexes();
        REQUIRE(indexes1.size() == 3);
        std::set<int> real1{indexes1.begin(), indexes1.end()};
        std::set<int> desired1{0, 1, 2};
        REQUIRE(real1 == desired1);
        auto indexes2 = g.getGroups()[1].getIndexes();
        std::set<int> real2{indexes2.begin(), indexes2.end()};
        std::set<int> desired2{3, 5};
        REQUIRE(real2 == desired2);
    }

    SECTION("It merges multiple units properly") {
        g.reduceToGroupCount(3);
        REQUIRE(g.getGroups().size() == 3);
        auto group = g.getGroups()[0];
        auto indexes = group.getIndexes();
        REQUIRE(indexes[0] == 0);
        REQUIRE(indexes[1] == 1);
        REQUIRE(indexes[2] == 2);
        auto &nb = group.getHistogram().getNormalizedBins();
        REQUIRE(nb[0] == Approx(0.833333333333));
        REQUIRE(nb[1] == Approx(0.083333333333));
        REQUIRE(nb[2] == Approx(0.083333333333));
    }

    SECTION("It merges into one properly") {
        g.reduceToGroupCount(1);
        REQUIRE(g.getGroups().size() == 1);
        auto& indexes = g.getGroups()[0].getIndexes();
        std::set<int> s(indexes.begin(), indexes.end());
        for(int i = 0; i < f->size(); ++i) {
            REQUIRE(s.find(i) != s.end());;
        }
    }

    SECTION("It breaks on negative reduction") {
        REQUIRE_THROWS_AS(g.reduceToGroupCount(-10), std::invalid_argument);
    }

    SECTION("It breaks on reduction larger than max size") {
        REQUIRE_THROWS_AS(g.reduceToGroupCount(1000), std::invalid_argument);
    }
}
