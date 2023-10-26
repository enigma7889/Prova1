#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

#include <iostream>
#include <string>

class ParticleType {
 private:
  const char* fName;
  const double fMass;
  const int fCharge;

 public:
  ParticleType() = default;
  ParticleType(const char*, const double, const int);

  const char* getName() const;
  double getMass() const;
  int getCharge() const;

  virtual void Print() const;
};

#endif