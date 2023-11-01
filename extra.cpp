#include "particle.hpp"
#include <cmath>
#include <cstdlib> //for RAND_MAX

int Particle::Decay2body(Particle &dau1,Particle &dau2) const {
  if(GetMass() == 0.0){
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }
  
  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if(fIndex > -1){ // fIParticle??? Maybe fIndex

    // gaussian random numbers

    float x1, x2, w, y1;
    
    double invnum = 1./RAND_MAX;
    do {
      x1 = 2.0 * rand()*invnum - 1.0;
      x2 = 2.0 * rand()*invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while ( w >= 1.0 );
    
    w = sqrt( (-2.0 * log( w ) ) / w );
    y1 = x1 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;

  }

  if(massMot < massDau1 + massDau2){
    printf("Decayment cannot be preformed because mass is too low in this channel\n");
    return 2;
  }
  
  double pout = sqrt((massMot*massMot - (massDau1+massDau2)*(massDau1+massDau2))*(massMot*massMot - (massDau1-massDau2)*(massDau1-massDau2)))/massMot*0.5;

  double norm = 2*M_PI/RAND_MAX;

  double phi = rand()*norm;
  double theta = rand()*norm*0.5 - M_PI/2.;
  dau1.SetP(pout*sin(theta)*cos(phi),pout*sin(theta)*sin(phi),pout*cos(theta));
  dau2.SetP(-pout*sin(theta)*cos(phi),-pout*sin(theta)*sin(phi),-pout*cos(theta));

  double energy = sqrt(GetPx()*GetPx() + GetPy()*GetPy() + GetPz()*GetPz() + massMot*massMot); //fPx, fPy, fPz??? Maybe fMomentum[i]

  double bx = GetPx()/energy; //fPx, fPy, fPz??? Maybe fMomentum[i]
  double by = GetPy()/energy;
  double bz = GetPz()/energy;

  dau1.Boost(bx,by,bz);
  dau2.Boost(bx,by,bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz)
{

  double energy = GetEnergy();

  //Boost this Lorentz vector
  double b2 = bx*bx + by*by + bz*bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx*GetPx() + by*GetPy() + bz*GetPz();
  double gamma2 = b2 > 0 ? (gamma - 1.0)/b2 : 0.0;

  fMomentum[0] += gamma2*bp*bx + gamma*bx*energy;   //fPx, fPy, fPz??? Maybe fMomentum[i]
  fMomentum[1] += gamma2*bp*by + gamma*by*energy;
  fMomentum[2] += gamma2*bp*bz + gamma*bz*energy;
}