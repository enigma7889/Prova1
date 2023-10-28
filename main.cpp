#include "particle.hpp"
#include "particletype.hpp"
#include "resonancetype.hpp"

int main() {
  Particle::AddParticleType("proton", 1.6, 1, 0);
  Particle::AddParticleType("electron", 1.7, -1, 2.9);
  Particle::AddParticleType("electron", 1.7, -1, 2.9);
  Particle::AddParticleType("proton", 1.6, 1, 0);
  Particle::AddParticleType("muon", 1.8, 0, 2.2);
  Particle::AddParticleType("electron", 1.7, -1, 2.9);
  Particle::AddParticleType("a", 1.9, 0, 7);
  Particle::AddParticleType("b", 1.5, -3, 0);
  Particle::AddParticleType("c", 2, 0, 0);

  Particle P("proton", 0.99, 0.98, 0.97);
  Particle R("muon", 0.91, 0.92, 0.93);
  Particle Q("electron", 0.96, 0.95, 0.94);

  P.PrintParticle();
  R.PrintParticle();
  Q.PrintParticle();

  std::cout << P.GetMass() << std::endl;
  std::cout << P.GetPx() << std::endl;
  std::cout << P.GetPy() << std::endl;
  std::cout << P.GetPz() << std::endl;

  Particle::PrintParticleType();

  std::cout << "InvMass between proton and electron : " <<P.InvMass(Q)<<std::endl;
}