#include <cmath>

#include "particle.hpp"

int main() {
  Particle::AddParticleType("π+", 0.13957, +1, 0);
  Particle::AddParticleType("π-", 0.13957, -1, 0);
  Particle::AddParticleType("K+", 0.49367, +1, 0);
  Particle::AddParticleType("K-", 0.49367, -1, 0);
  Particle::AddParticleType("p+", 0.93827, +1, 0);
  Particle::AddParticleType("p-", 0.93827, -1, 0);
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);

  int MaxParticlesPerEvent = 120;  // Maybe remove, could be useless
  Particle EventParticles[MaxParticlesPerEvent];

  gRandom->SetSeed();

  for (int i{0}; i < 1.E5; ++i) {    // 10^5 Events
    for (int j{0}; j < 1.E2; ++j) {  // 10^2 Particles per event
      Particle P; //Particle j out of 100 of the i-th event

      double phi = gRandom()->Uniform(0, 2 * M_PI);
      double theta = gRandom()->Uniform(0, M_PI);
      double impulse = gRandom()->Exp(1);  // module of momentum

      P.SetP(impulse * sin(theta) * cos(phi), impulse * sin(theta) * sin(phi),
             impulse * cos(theta));

      double x = gRandom->Rndm();
      if(x < 0.40) P.SetIndex("π+");
      else if (x < 0.80) P.SetIndex("π-");
      else if (x < 0.85) P.SetIndex("K+");
      else if (x < 0.90) P.SetIndex("K-");
      else if (x < 0.945) P.SetIndex("p+");
      else if (x < 0.99) P.SetIndex("p-");
      else P.SetIndex("K*");
      
      //EventParticles[j] = P;
    }
  }
}