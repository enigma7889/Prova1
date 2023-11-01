#include "particle.hpp"

int main() {
  Particle::AddParticleType("π+", 0.13957, +1, 0);
  Particle::AddParticleType("π-", 0.13957, -1, 0);
  Particle::AddParticleType("K+", 0.49367, +1, 0);
  Particle::AddParticleType("K-", 0.49367, -1, 0);
  Particle::AddParticleType("p+", 0.93827, +1, 0);
  Particle::AddParticleType("p-", 0.93827, -1, 0);
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);
  /*
  int NParticles = 120;
  std::array<Particle,120>EventParticles{};

  gRandom->SetSeed();
  */
}