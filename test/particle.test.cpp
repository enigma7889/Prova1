#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../particle.hpp"

#include "doctest.h"

TEST_CASE("Particle Class") {
  Particle::AddParticleType("proton", 1.6, 1, 0);
  Particle::AddParticleType("electron", 1.7, -1, 2.9);
  Particle::AddParticleType("electron", 1.7, -1, 2.9);
  Particle::AddParticleType("proton", 1.6, 1, 0);
  Particle::AddParticleType("muon", 1.8, 0, 2.2);
  Particle::AddParticleType("electron", 1.7, -1, 2.9);
  Particle::AddParticleType("a", 1.9, 0, 7);
  Particle::AddParticleType("b", 1.5, -3, 0);
  Particle::AddParticleType("c", 2, 0, 0);

  Particle p("proton", 0.99, 0.98, 0.97);
  Particle m("muon", 0.91, 0.92, 0.93);
  Particle e("electron", 0.96, 0.95, 0.94);
  Particle a("a",4,5,6);

  SUBCASE("Setters and getters") {
    CHECK(p.GetIndex() == 0);
    CHECK(m.GetIndex() == 2);
    CHECK(e.GetIndex() == 1);
    
    CHECK(e.GetPx() == 0.96);
    CHECK(e.GetPy() == 0.95);
    CHECK(e.GetPz() == 0.94);
    
    e.SetP(1, 2, 3);
    CHECK(e.GetPx() == 1);
    CHECK(e.GetPy() == 2);
    CHECK(e.GetPz() == 3);
    
    CHECK(p.GetMass() == 1.6);
    CHECK(m.GetMass() == 1.8);
    CHECK(e.GetMass() == 1.7);

    e.SetIndex("a");
    CHECK(e.GetPx() == 1);
    CHECK(e.GetPy() == 2);
    CHECK(e.GetPz() == 3);
    CHECK(e.GetMass() == 1.9);

    p.SetIndex(3);
    CHECK(p.GetMass() == 1.9);
  }
  SUBCASE("Matematical functions") {
    CHECK(p.GetEnergy() == doctest::Approx(2.332680));
    CHECK(m.GetEnergy() == doctest::Approx(2.404038));
    CHECK(p.InvMass(m) == m.InvMass(p));
    CHECK(p.InvMass(m) == doctest::Approx(3.40683));
  }
};
