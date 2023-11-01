#include "particle.hpp"

#include <cassert>
#include <cmath>

std::array<ParticleType*, fMaxNumParticleType> Particle::fParticleType;

int Particle::fNParticleType = 0;

int Particle::FindParticle(const char* name) {
  int i{0};
  for (; i < fNParticleType; ++i) {
    if (fParticleType[i]->GetName() == name) {
      return i;
    }
  }
  std::cout << "No corrispondence. New particle revealed" << '\n';
  return i;
}

Particle::Particle(const char* name, double px, double py, double pz)
    : fMomentum({px, py, pz}) {
  fIndex = FindParticle(name);
}

int Particle::GetIndex() const { return fIndex; }

double Particle::GetPx() const { return fMomentum[0]; }
double Particle::GetPy() const { return fMomentum[1]; }
double Particle::GetPz() const { return fMomentum[2]; }

double Particle::GetMass() const { return fParticleType[fIndex]->GetMass(); }

double Particle::GetEnergy() const {
  return std::sqrt(std::pow(this->GetMass(), 2) + std::pow(this->GetPx(), 2) +
                   std::pow(this->GetPy(), 2) + std::pow(this->GetPz(), 2));
}

double Particle::InvMass(Particle& P) {
  return std::sqrt(std::pow(this->GetEnergy() + P.GetEnergy(), 2) -
                   (std::pow(this->GetPx() + P.GetPx(), 2) +
                    std::pow(this->GetPy() + P.GetPy(), 2) +
                    std::pow(this->GetPz() + P.GetPz(), 2)));
}

void Particle::SetP(double px, double py, double pz) {
  this->fMomentum[0] = px;
  this->fMomentum[1] = py;
  this->fMomentum[2] = pz;
}

void Particle::SetIndex(int i) {
  assert(i < fNParticleType);
  this->fIndex = i;
}

void Particle::SetIndex(const char* name) {
  int i = FindParticle(name);
  if (i != fNParticleType) {
    this->fIndex = i;
  } else {
    std::cout << "Particle name not in list" << '\n';
  }
}

void Particle::AddParticleType(const char* name, double mass, int charge,
                               double width = 0) {
  int i = FindParticle(name);
  if (i == (fNParticleType)) {
    ++fNParticleType;
    std::cout << "Number of Particles known increased : " << fNParticleType
              << '\n';
    if (fNParticleType > fMaxNumParticleType) {
      throw std::runtime_error("Maximum particle types number reached");
    }
    if (width == 0) {
      fParticleType[i] = new ParticleType(name, mass, charge);
    } else {
      fParticleType[i] = new ResonanceType(name, mass, charge, width);
    }
  }
}

void Particle::PrintParticleType() {
  for (int i{0}; i < fNParticleType; ++i) {
    fParticleType[i]->Print();
  }
}

void Particle::PrintParticle() const {
  std::cout << "This particle attributes are :" << '\n';
  std::cout << "Name : " << this->fParticleType[this->fIndex]->GetName()
            << '\n';
  std::cout << "Index : " << this->fIndex << '\n';
  std::cout << "Px : " << this->fMomentum[0] << '\n';
  std::cout << "Py : " << this->fMomentum[1] << '\n';
  std::cout << "Pz : " << this->fMomentum[2] << '\n' << '\n';
}
