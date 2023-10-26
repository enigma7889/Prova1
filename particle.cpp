#include "particle.hpp"

#include <cassert>

std::array<ParticleType*, fMaxNumParticleType> Particle::fParticleType;

int Particle::fNParticleType = 0;

int Particle::FindParticle(const char* name) {
  int i{0};
  for (; i < fNParticleType; ++i) {
    if (name == fParticleType[i]->getName()) {
      return i;
    }
  }
  std::cout << "No corrispondence. New particle revealed" << '\n';

  return i;
}

Particle::Particle(char* name, double px, double py, double pz) {
  fMomentum[0] = px;
  fMomentum[1] = py;
  fMomentum[2] = pz;
  fIndex = FindParticle(name);
  /*
  if(fIndex == fMaxNumParticleType) {
    Particle::AddParticleType();
  }
  */
}

int Particle::getIndex() const { return fIndex; }

void Particle::setIndex(int i) {
  assert(i < fNParticleType);
  this->fIndex = i;
}

void Particle::setIndex(const char* name) {
  int i = FindParticle(name);
  if (i != fNParticleType) {
    this->fIndex = i;
  } else {
    std::cout << "Particle name not in list" << '\n';
  }
}

void Particle::AddParticleType(char* name, double mass, int charge,
                               double width = 0) {
  int i = FindParticle(name);
  if (i == fNParticleType) {
    ++fNParticleType;
    if (fNParticleType > fMaxNumParticleType) {
      throw std::runtime_error("Maximum particle types number reached");
    }
    ResonanceType R(name, mass, charge, width);
    // ParticleType* P = &R;
    fParticleType[i] = &R;
  }
}
