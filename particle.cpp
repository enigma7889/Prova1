#include "particle.hpp"

int Particle::FindParticle(char* name) {
  /*
  int i{0};
  for (; i < fNParticleType; ++i) {             // reference not defined ...
    if (name == fParticleType[i]->getName()) {  // reference not defined ...
      return i;
    }
  }
  std::cout << "No corrispondence. New particle revealed" << '\n';

  return i;
  */
}

Particle::Particle(char* name, double px, double py, double pz) {
  fMomentum[0] = px;
  fMomentum[1] = py;
  fMomentum[2] = pz;
  fIndex = FindParticle(name);
  /*
  if (fIndex == fNParticleType) {
    ++fNParticleType;
    fParticleType[fIndex]->setName(name);
  }
  */
}

int Particle::getIndex() const { return fIndex; }

// void Particle::AddParticleType(char* name, double mass, int charge, double width = 0){}
