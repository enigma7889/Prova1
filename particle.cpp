#include "particle.hpp"

#include <cstdlib>  //for RAND_MAX
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

int Particle::GetCharge() const {
  return this->fParticleType[this->fIndex]->GetCharge();
}

int Particle::Decay2body(Particle &dau1, Particle &dau2) const {
  if (GetMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if (fIndex > -1) {  // fIParticle??? Maybe fIndex

    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    printf(
        "Decayment cannot be preformed because mass is too low in this "
        "channel\n");
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy =
      sqrt(GetPx() * GetPx() + GetPy() * GetPy() + GetPz() * GetPz() +
           massMot * massMot);  // fPx, fPy, fPz??? Maybe fMomentum[i]

  double bx = GetPx() / energy;  // fPx, fPy, fPz??? Maybe fMomentum[i]
  double by = GetPy() / energy;
  double bz = GetPz() / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz) {
  double energy = GetEnergy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * GetPx() + by * GetPy() + bz * GetPz();
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fMomentum[0] += gamma2 * bp * bx +
                  gamma * bx * energy;  // fPx, fPy, fPz??? Maybe fMomentum[i]
  fMomentum[1] += gamma2 * bp * by + gamma * by * energy;
  fMomentum[2] += gamma2 * bp * bz + gamma * bz * energy;
}
