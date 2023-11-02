#include <cmath>

#include "particle.hpp"

int main() {
  Particle::AddParticleType("π+", 0.13957, +1, 0);
  Particle::AddParticleType("π-", 0.13957, -1, 0);
  Particle::AddParticleType("K+", 0.49367, +1, 0);
  Particle::AddParticleType("K-", 0.49367, -1, 0);
  Particle::AddParticleType("p+", 0.93827, +1, 0);
  Particle::AddParticleType("p-", 0.93827, -1, 0);
  Particle::AddParticleType("K*", 0.89166, 0, 0.05);

  int NResonanceDaughters;
  Particle EventParticles[120];

  TH1D *ParticleTypeHisto =
      new TH1D("PTH", "Particle Types Distribution", 7, 0, 7);
  TH2D *AngleHisto =
      new TH2D("AH", "Angle of momentum Distribution", 1000, 0, 2 * M_PI, 1000,
               0, M_PI);  // phi is x and theta is y
  TH1D *ImpulseHisto =
      new TH1D("IH", "Momentum Module Distribution", 1000, 0, 5);
  TH1D *TrasverseImpulseHisto =
      new TH1D("TIH", "Trasversal Momentum Module Distribution", 1000, 0, 4);
  TH1D *EnergyHisto = new TH1D("EH", "Energy Distribution", 1000, 0, 4);
  TH1D *InvMassHisto = new TH1D("IMH", "InvMass Distribution", 1000, 0,
                                10);  // To adjust later...
  TH1D *InvMassBetweenDiscChargesHisto = new TH1D(
      "IMBDCH", "InvMass Distribution between discording charges", 1000, 0, 10);
  TH1D *InvMassBetweenConcChargesHisto = new TH1D(
      "IMBCCH", "InvMass Distribution between concording charges", 1000, 0, 10);
  TH1D *InvMassBetweenPPNKHisto = new TH1D(
      "PPNK", "InvMass Distribution between Positive Pion and Negative Kaon",
      1000, 0, 10);
  TH1D *InvMassBetweenNPPKHisto = new TH1D(
      "NPPK", "InvMass Distribution between Negative Pion and Positive Kaon",
      1000, 0, 10);
  TH1D *InvMassBetweenPPPKHisto = new TH1D(
      "PPPK", "InvMass Distribution between Positive Pion and Positive Kaon",
      1000, 0, 10);
  TH1D *InvMassBetweenNPNKHisto = new TH1D(
      "NPNK", "InvMass Distribution between Negative Pion and Negative Kaon",
      1000, 0, 10);

  gRandom->SetSeed();

  for (int i{0}; i < 1.E5; ++i) {  // 10^5 Events
    NResonanceDaughters = 0;
    for (int j{0}; j < 100; ++j) {  // 10^2 Particles per event
      Particle P;                   // Particle j out of 100 of the i-th event
      Particle p1, p2;

      double phi = gRandom()->Uniform(0, 2 * M_PI);
      double theta = gRandom()->Uniform(0, M_PI);
      double impulse = gRandom()->Exp(1);  // module of momentum

      P.SetP(impulse * sin(theta) * cos(phi), impulse * sin(theta) * sin(phi),
             impulse * cos(theta));

      double x = gRandom->Rndm();
      if (x < 0.40)
        P.SetIndex("π+");
      else if (x < 0.80)
        P.SetIndex("π-");
      else if (x < 0.85)
        P.SetIndex("K+");
      else if (x < 0.90)
        P.SetIndex("K-");
      else if (x < 0.945)
        P.SetIndex("p+");
      else if (x < 0.99)
        P.SetIndex("p-");
      else {
        P.SetIndex("K*");
        P.Decay2body(p1, p2);
        double y = gRandom->Rndm();
        if (y < 0.5) {
          p1.SetIndex("π+");
          p2.SetIndex("K-");
        } else {
          p1.SetIndex("π-");
          p2.SetIndex("K+");
        }
        ++NResonanceDaughters;
        EventParticles[99 + NResonanceDaughters] = p1;
        ++NResonanceDaughters;
        EventParticles[99 + NResonanceDaughters] = p2;
      }
      EventParticles[j] = P;

      // From here we fill the Histograms

      ParticleTypeHisto->Fill(P.GetIndex());

      AngleHisto->Fill(phi, theta);

      ImpulseHisto->Fill(impulse);

      TrasverseImpulseHisto->Fill(
          sqrt(pow(impulse * sin(theta) * cos(phi), 2) +
               pow(impulse * sin(theta) * sin(phi),
                   2)));  // sqrt of module in x and y of impluse (Not with z)

      EnergyHisto->Fill(P.GetEnergy());

      // Here we are discarding the case P is K*
      if (EventParticles[j].GetIndex() != 6) {
        // InvMass for all the particles of a single event Histogram Filling

        // this goes for the first non-empty part of the array
        for (int k{0}; k < j; ++k) {
          if (EventParticles[k].GetIndex() != 6) {  // Here we are excluding K*

            double InvMass = EventParticles[j].InvMass(EventParticles[k]);

            InvMassHisto->Fill(InvMass);

            if (EventParticles[k].GetCharge() !=
                EventParticles[j].GetCharge()) {
              InvMassBetweenDiscChargesHisto->Fill(InvMass);
            }

            if (EventParticles[k].GetCharge() ==
                EventParticles[j].GetCharge()) {
              InvMassBetweenConcChargesHisto->Fill(InvMass);
            }

            // PION+/PION-/KAON+/KAON-
          }
        }

        // this goes for the resonance daughters
        for (int k{99}; k < (99 + NResonanceDaughters); ++k) {
          double InvMass = EventParticles[j].InvMass(EventParticles[k]);

          InvMassHisto->Fill(InvMass);

          if (EventParticles[k].GetCharge() != EventParticles[j].GetCharge()) {
            InvMassBetweenDiscChargesHisto->Fill(InvMass);
          }

          if (EventParticles[k].GetCharge() == EventParticles[j].GetCharge()) {
            InvMassBetweenConcChargesHisto->Fill(InvMass);
          }

          // PION+/PION-/KAON+/KAON-
        }
      }
    }
  }
}