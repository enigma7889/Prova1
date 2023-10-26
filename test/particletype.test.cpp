#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../particletype.hpp"

#include "../particle.hpp"
#include "../resonancetype.hpp"
#include "doctest.h"

TEST_CASE("prova1 Test") {
  ParticleType P("proton", 1.6, 1);
  ResonanceType Q("electron", 1.7, -1, 2.9);
  SUBCASE("Testing Getters and setters") {
    CHECK(P.getName() == "proton");
    CHECK(Q.getName() == "electron");
    CHECK(P.getMass() == 1.6);
    CHECK(Q.getMass() == 1.7);
    CHECK(P.getCharge() == 1);
    CHECK(Q.getCharge() == -1);
    CHECK(Q.getWidth() == doctest::Approx(2.9));
  }
};
