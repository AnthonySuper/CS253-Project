#include <catch.hpp>
#include <pgm_image.hpp>

#define PGM_FILE(x) "test_files/pgm/" x ".pgm"

TEST_CASE("PGM image fails on bad header", "[PgmImage]") {
    REQUIRE_THROWS_AS(new PgmImage(PGM_FILE("bad_header")),
            InvalidFormatError);
}

TEST_CASE("PGM image fails on invalid maximum", "[PgmImage]") {
    REQUIRE_THROWS_AS(new PgmImage(PGM_FILE("bad_max")),
            InvalidFormatError);
}

TEST_CASE("PGM image fails on too little pixels", "[PgmImage]") {
    REQUIRE_THROWS_AS(new PgmImage(PGM_FILE("bad_pixel_count")),
            InvalidFormatError);
}

TEST_CASE("PGM Image works on valid files", "[PgmImage]") {
    PgmImage *q;
    REQUIRE_NOTHROW(q = new PgmImage(PGM_FILE("quad_a")));
    delete q;
}
