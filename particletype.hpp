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

  const char* GetName() const;
  double GetMass() const;
  int GetCharge() const;

  virtual void Print() const;
};

#endif