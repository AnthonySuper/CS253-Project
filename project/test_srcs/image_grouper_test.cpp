#include <catch.hpp>
#include <image_grouper.hpp>
#include <set>
#include <stdexcept>
#include <histogram_group.hpp>

using IPtr = std::shared_ptr<DepthImage>;
using ISet = std::set<IPtr>;

TEST_CASE("ImageGrouper holds images", "[ImageGrouper]") {
    HistogramGroup::Factory fac;
    auto f = ImageDataset::fromFile("test_files/list/big.txt");
    ImageGrouper g{f, &fac};;
    auto groups = g.getGroups();
    REQUIRE(groups.size() == f.size());
}

TEST_CASE("ImageGrouper groups images", "[ImageGrouper]") {
    HistogramGroup::Factory fac;
    auto ds = ImageDataset::fromFile("test_files/list/big.txt");
    ImageGrouper g{ds, &fac};
    SECTION("It reduces to the right size") {
        g.reduceToGroupCount(2);
        REQUIRE(g.getGroups().size() == 2);
    }

    SECTION("It merges properly initially") {
        g.reduceToGroupCount(5);
        REQUIRE(g.getGroups().size() == 5);
        auto images = g.getGroups()[0]->getImages();
        REQUIRE(images.size() == 2);
        ISet real{images.begin(), images.end()};
        ISet desired{ds.at(0), ds.at(1)};
        REQUIRE(desired == real);
    }

    SECTION("It merges groups based on closest distances") {
        g.reduceToGroupCount(3);
        REQUIRE(g.getGroups().size() == 3);
        auto images1 = g.getGroups()[0]->getImages();
        REQUIRE(images1.size() == 3);
        ISet real1{images1.begin(), images1.end()};
        ISet desired1{ds.at(0), ds.at(1), ds.at(2)};
        REQUIRE(real1 == desired1);
        auto images2 = g.getGroups()[1]->getImages();
        ISet real2{images2.begin(), images2.end()};
        ISet desired2{ds.at(3), ds.at(5)};
        REQUIRE(real2 == desired2);
    }

    SECTION("It merges into one properly") {
        g.reduceToGroupCount(1);
        REQUIRE(g.getGroups().size() == 1);
        auto images = g.getGroups()[0]->getImages();
        ISet s(images.begin(), images.end());
        for(unsigned int i = 0; i < ds.size(); ++i) {
            REQUIRE(s.find(ds.at(i)) != s.end());
        }
    }

    SECTION("It breaks on negative reduction") {
        REQUIRE_THROWS_AS(g.reduceToGroupCount(-10), std::invalid_argument);
    }

    SECTION("It breaks on reduction larger than max size") {
        REQUIRE_THROWS_AS(g.reduceToGroupCount(1000), std::invalid_argument);
    }
}
