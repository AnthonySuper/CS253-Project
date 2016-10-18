#include <histogram_group.hpp>
#include <catch.hpp>
#include <image_dataset.hpp>
#include <set>
#include <stdexcept>


TEST_CASE("With a good dataset", "[HistogramGroup]") {
    using IPtr = std::shared_ptr<DepthImage>;
    using ISet = std::set<IPtr>;
    auto ds = ImageDataset::fromFile("test_files/list/big.txt");

    SECTION("It throws no errors on construction") {
        REQUIRE_NOTHROW(HistogramGroup(ds.at(0)));
    }

    SECTION("It sets indexes") {
        auto d = HistogramGroup(ds.at(0));
        auto imgs = d.getImages();
        ISet real(imgs.begin(), imgs.end());
        ISet desired{ds.at(0)};
        REQUIRE(real == desired);
    }

    SECTION("It properly sets the histogram") {
        auto d = HistogramGroup(ds.at(0));
        auto img = ds.at(0);
        auto bins1 = d.getHistogram().getNormalizedBins();
        auto bins2 = img->getHistogram().getNormalizedBins();
        REQUIRE(bins1 == bins2);
    }

    SECTION("It properly merges") {
        auto d1 = HistogramGroup(ds.at(0));
        auto d2 = HistogramGroup(ds.at(1));
        d1.merge(d2);
        SECTION("The histograms merge") {
            Histogram des(ds.at(0)->getHistogram(), ds.at(1)->getHistogram());
            REQUIRE(d1.getHistogram() == des);
        }
        SECTION("The indexes merge") {
            auto images = d1.getImages();
            ISet real{images.begin(), images.end()};
            auto i1 = ds.at(0);
            auto i2 = ds.at(1);
            ISet desired{i1, i2};
            REQUIRE(real == desired);
        }
    }

    SECTION("Construction fails on nullptr") {
        REQUIRE_THROWS_AS(HistogramGroup(nullptr), std::invalid_argument);
    }

    SECTION("The factory works", "[HistogramGroup::Factory]") {
        HistogramGroup::Factory f;
        auto q = f.create(ds.at(0));
        REQUIRE(dynamic_cast<HistogramGroup*>(q) != nullptr);
    }
}
