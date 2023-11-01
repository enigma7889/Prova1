#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <array>

#include "resonancetype.hpp"

static const int fMaxNumParticleType = 10;

class Particle {
 private:
  static std::array<ParticleType*, fMaxNumParticleType> fParticleType;
  static int fNParticleType;  // Number of non-null elements of fParticleType

  int fIndex;  // index of fParticleType elements
  std::array<double, 3> fMomentum;

  static int FindParticle(const char*);

  void Boost(double bx, double by, double bz);

 public:
  Particle(const char*, double, double, double);

  int GetIndex() const;

  double GetPx() const;
  double GetPy() const;
  double GetPz() const;

  double GetMass() const;

  double GetEnergy() const;

  double InvMass(Particle&);

  void SetP(double, double, double);

  void SetIndex(int);
  void SetIndex(const char*);

  static void AddParticleType(const char*, double, int, double);

  static void PrintParticleType();

  void PrintParticle() const;

  int Decay2body(Particle &dau1,Particle &dau2) const;
};

#endif