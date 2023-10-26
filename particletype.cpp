#include "particletype.hpp"

ParticleType::ParticleType(const char* name_, const double mass_,
                           const int charge_)
    : fName(name_), fMass(mass_), fCharge(charge_){};

const char* ParticleType::getName() const { return fName; }
double ParticleType::getMass() const { return fMass; }
int ParticleType::getCharge() const { return fCharge; }

void ParticleType::Print() const {
  std::cout << "The particle attributes are:" << '\n';
  std::cout << "Name: " << this->getName() << '\n';
  std::cout << "Mass: " << this->getMass() << '\n';
  std::cout << "Charge: " << this->getCharge() << '\n';
}