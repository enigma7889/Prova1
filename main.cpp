#include "particle.hpp"
#include "particletype.hpp"
#include "resonancetype.hpp"

int main() {
  ResonanceType P("proton", 1.6, 1, 1.7);
  ParticleType Q("electron", 1.2, -1);

  std::array<ParticleType*, 2> A{{&P, &Q}};

  for (int i{0}; i < 2; ++i) {
    A[i]->Print();
  }
}