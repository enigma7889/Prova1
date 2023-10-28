#include "particle.hpp"
#include "particletype.hpp"
#include "resonancetype.hpp"

int main() {
  /*
  ResonanceType P("proton", 1.6, 1, 1.7);
  ParticleType Q("electron", 1.2, -1);

  std::array<ParticleType*, 2> A{{&P, &Q}};

  for (int i{0}; i < 2; ++i) {
    A[i]->Print();
  }
  */
  Particle::AddParticleType("proton", 1.6, 1, 0);

  Particle::AddParticleType("electron", 1.7, -1, 2.9);
  Particle::AddParticleType("electron", 1.7, -1, 2.9);
  Particle::AddParticleType("electron", 1.7, -1, 2.9);
  Particle::AddParticleType("proton", 1.6, 1, 0);
  Particle::AddParticleType("muon", 1.8, 0, 2.2);
  Particle::AddParticleType("electron", 1.7, -1, 2.9);

  Particle P("proton",0.99,0.98,0.97);
  Particle R("muon",0.91,0.92,0.93);
  Particle Q("electron",0.96,0.95,0.94);

  P.PrintParticle();
  R.PrintParticle();
  Q.PrintParticle();
  std::cout<<P.GetMass()<<std::endl;
  std::cout<<P.GetPx()<<std::endl;
  std::cout<<P.GetPy()<<std::endl;
  std::cout<<P.GetPz()<<std::endl;

}