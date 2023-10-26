#include "resonancetype.hpp"

ResonanceType::ResonanceType(const char* name, const double mass,
                             const int charge, const double width)
    : ParticleType(name, mass, charge), fWidth(width){};

double ResonanceType::getWidth() const { return fWidth; }

void ResonanceType::Print() const {
  this->ParticleType::Print();
  std::cout << "Width: " << this->getWidth() << '\n';
}