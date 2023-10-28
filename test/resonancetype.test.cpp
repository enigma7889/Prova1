#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../resonancetype.hpp"

#include "doctest.h"

TEST_CASE("ResonanceType Class") {
  ParticleType P("proton", 1.6, 1);
  ResonanceType Q("electron", 1.7, -1, 2.9);
  SUBCASE("Testing Getters and Setters") {
    CHECK(P.GetName() == "proton");
    CHECK(Q.GetName() == "electron");
    CHECK(P.GetMass() == 1.6);
    CHECK(Q.GetMass() == 1.7);
    CHECK(P.GetCharge() == 1);
    CHECK(Q.GetCharge() == -1);
    CHECK(Q.GetWidth() == doctest::Approx(2.9));
  }
};
