#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../particletype.hpp"
#include "doctest.h"

TEST_CASE("ParticleType Test") {
  ParticleType P("proton", 1.6, 1);
  SUBCASE("Testing Getters and Setters") {
    CHECK(P.GetName() == "proton");
    CHECK(P.GetMass() == 1.6);
    CHECK(P.GetCharge() == 1);
  }
};
