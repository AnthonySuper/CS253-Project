#include <histogram_group.hpp>
#include <catch.hpp>
#include <image_dataset.hpp>
#include <set>
#include <stdexcept>

TEST_CASE("With a good dataset", "[HistogramGroup]") {
    auto ds = ImageDataset::fromFile("test_files/list/big.txt");

    SECTION("It throws no errors on construction") {
        REQUIRE_NOTHROW(HistogramGroup(ds, 0));
    }

    SECTION("It sets indexes") {
        auto d = HistogramGroup(ds, 0);
        auto ind = d.getIndexes();
        std::set<int> real(ind.begin(), ind.end());
        std::set<int> desired{0};
        REQUIRE(real == desired);
    }

    SECTION("It properly sets the histogram") {
        auto d = HistogramGroup(ds, 0);
        auto &img = ds->at(0);
        auto bins1 = d.getHistogram().getNormalizedBins();
        auto bins2 = img.getHistogram().getNormalizedBins();
        REQUIRE(bins1 == bins2);
    }

    SECTION("It properly merges") {
        auto d1 = HistogramGroup(ds, 0);
        auto d2 = HistogramGroup(ds, 1);
        d1.merge(d2);
        SECTION("The histograms merge") {
            Histogram des(ds->at(0).getHistogram(), ds->at(1).getHistogram());
            REQUIRE(d1.getHistogram() == des);
        }
        SECTION("The indexes merge") {
            auto indexes = d1.getIndexes();
            std::set<int> real{indexes.begin(), indexes.end()};
            std::set<int> desired{0, 1};
            REQUIRE(real == desired);
        }
    }

    SECTION("Construction fails on out-of-range index") {
        SECTION("Indexes too big") {
          REQUIRE_THROWS_AS(HistogramGroup(ds, 1000), std::exception);
        }
        SECTION("indexes too small") {
            REQUIRE_THROWS_AS(HistogramGroup(ds, -1), std::exception);
        }
    }

    SECTION("The factory works", "[HistogramGroup::Factory]") {
        HistogramGroup::Factory f;
        auto q = f.create(ds, 0);
        REQUIRE(dynamic_cast<HistogramGroup*>(q) != nullptr);
    }
}

TEST_CASE("With a bad dataset", "[HistogramGroup]") {
    std::shared_ptr<ImageDataset> ds(nullptr);
    SECTION("It throws an error") {
        REQUIRE_THROWS_AS(HistogramGroup(ds, 0), std::invalid_argument);
    }
}
