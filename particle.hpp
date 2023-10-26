#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <array>

#include "resonancetype.hpp"

static const int fMaxNumParticleType = 10;

class Particle {
 private:
  static std::array<ParticleType*, fMaxNumParticleType> fParticleType;
  static int fNParticleType;  // Non-null elements of fParticleType
  int fIndex;                 // index of fParticleType elements
  std::array<double, 3> fMomentum{{0, 0, 0}};

  int FindParticle(char*);

 public:
  Particle(char*, double, double, double);

  int getIndex() const;
  // static void AddParticleType(char*, double, int, double);
};

#endif