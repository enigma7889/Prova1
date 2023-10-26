#include "particletype.hpp"

ParticleType::ParticleType(const char* name_, const double mass_,
                           const int charge_)
    : fName(name_), fMass(mass_), fCharge(charge_){};

const char* ParticleType::GetName() const { return fName; }
double ParticleType::GetMass() const { return fMass; }
int ParticleType::GetCharge() const { return fCharge; }

void ParticleType::Print() const {
  std::cout << "The particle attributes are:" << '\n';
  std::cout << "Name: " << this->GetName() << '\n';
  std::cout << "Mass: " << this->GetMass() << '\n';
  std::cout << "Charge: " << this->GetCharge() << '\n';
}