#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <cmath>

#include "TFile.h"
#include "TROOT.h"
#include "TRandom.h"
#include "particle.hpp"

int Main() {
  Particle::AddParticleType("π+", 0.13957, +1, 0);    // 0
  Particle::AddParticleType("π-", 0.13957, -1, 0);    // 1
  Particle::AddParticleType("K+", 0.49367, +1, 0);    // 2
  Particle::AddParticleType("K-", 0.49367, -1, 0);    // 3
  Particle::AddParticleType("p+", 0.93827, +1, 0);    // 4
  Particle::AddParticleType("p-", 0.93827, -1, 0);    // 5
  Particle::AddParticleType("K*", 0.89166, 0, 0.05);  // 6

  int NResonanceDaughters;
  Particle EventParticles[120];

  TH1D *ParticleTypeHisto = new TH1D("PTH", "Particle Types Distribution", 7, 0, 7);
  TH2D *AngleHisto = new TH2D("AH", "Angle of momentum Distribution", 1000, 0, 2 * M_PI, 1000, 0, M_PI);  // phi is x and theta is y
  TH1D *ImpulseHisto = new TH1D("IH", "Momentum Module Distribution", 1000, 0, 5);
  TH1D *TrasverseImpulseHisto = new TH1D("TIH", "Trasversal Momentum Module Distribution", 1000, 0, 3);
  TH1D *EnergyHisto = new TH1D("EH", "Energy Distribution", 1000, 0, 3);
  TH1D *InvMassHisto = new TH1D("IMH", "InvMass Distribution", 1000, 0, 6);
  InvMassHisto->Sumw2();
  TH1D *InvMassDiscChargesHisto = new TH1D("IMBDCH", "InvMass Distribution between discording charges", 1000, 0, 6);
  InvMassDiscChargesHisto->Sumw2();
  TH1D *InvMassConcChargesHisto = new TH1D("IMBCCH", "InvMass Distribution between concording charges", 1000, 0, 6);
  InvMassConcChargesHisto->Sumw2();
  TH1D *InvMassDPKHisto = new TH1D("DPKH", "InvMass Distribution between discording Pions and Kaons", 1000, 0, 6);
  InvMassDPKHisto->Sumw2();
  TH1D *InvMassCPKHisto = new TH1D("CPKH", "InvMass Distribution between concording Pions and Kaons", 1000, 0, 6);
  InvMassCPKHisto->Sumw2();
  TH1D *InvMassDecayDaughtersHisto = new TH1D("IMDDH", "InvMass Distribution between daughters of Decay", 100, 0.6, 1.2);
  InvMassDecayDaughtersHisto->Sumw2();

  gRandom->SetSeed();
  for (int i{0}; i < 1.E5; ++i) {  // 10^5 Events
    NResonanceDaughters = 0;
    for (int j{0}; j < 1.E2; ++j) {  // 10^2 Particles per event
      Particle P;                    // Particle j out of 100 of the i-th event
      Particle p1, p2;               // Possible daughters of a resonance particle

      double phi = gRandom->Uniform(0, 2 * M_PI);
      double theta = gRandom->Uniform(0, M_PI);
      double impulse = gRandom->Exp(1);  // module of momentum

      P.SetP(impulse * sin(theta) * cos(phi), impulse * sin(theta) * sin(phi), impulse * cos(theta));

      double x = gRandom->Rndm();
      if (x < 0.40)
        P.SetIndex(0);
      else if (x < 0.80)
        P.SetIndex(1);
      else if (x < 0.85)
        P.SetIndex(2);
      else if (x < 0.90)
        P.SetIndex(3);
      else if (x < 0.945)
        P.SetIndex(4);
      else if (x < 0.99)
        P.SetIndex(5);
      else {  // Resonance Case, Must Decay
        P.SetIndex(6);
        double y = gRandom->Rndm();
        if (y < 0.5) {
          p1.SetIndex(0);
          p2.SetIndex(3);
        } else {
          p1.SetIndex(1);
          p2.SetIndex(2);
        }
        P.Decay2body(p1, p2);
        EventParticles[100 + NResonanceDaughters] = p1;  // starts from 99 + 1
        ++NResonanceDaughters;
        EventParticles[100 + NResonanceDaughters] = p2;
        ++NResonanceDaughters;
      }

      EventParticles[j] = P;

      //---------------------------------
      // From here we fill the Histograms
      //---------------------------------

      ParticleTypeHisto->Fill(P.GetIndex());

      AngleHisto->Fill(phi, theta);

      ImpulseHisto->Fill(impulse);

      TrasverseImpulseHisto->Fill(sqrt(pow(impulse * sin(theta) * cos(phi), 2) + pow(impulse * sin(theta) * sin(phi),
                                                                                     2)));  // module in x and y of impluse (Not with z)

      EnergyHisto->Fill(P.GetEnergy());

      if (P.GetIndex() == 6) {  // Here we are discarding the case the Particle is K*
        continue;
      }
      //--------------------------------------------------------------
      // InvMass for all the cases of a single event Histogram Filling
      //--------------------------------------------------------------

      for (int k{0}; k < j; ++k) {                // this goes for the first non-empty part of the array
        if (EventParticles[k].GetIndex() == 6) {  // Here we are excluding K* particles already in the array
          continue;
        }
        double InvMass = P.InvMass(EventParticles[k]);

        InvMassHisto->Fill(InvMass);

        if (EventParticles[k].GetCharge() != P.GetCharge())  // different charge
        {
          InvMassDiscChargesHisto->Fill(InvMass);
        } else  // same charge
        {
          InvMassConcChargesHisto->Fill(InvMass);
        }

        int kIndex = EventParticles[k].GetIndex();  // TO DELETE IN EACH APPEARENCE

        switch (kIndex) {
          case 0:                     // pion+
            if (P.GetIndex() == 2) {  // kaon+
              InvMassCPKHisto->Fill(InvMass);
            } else if (P.GetIndex() == 3) {  // kaon-
              InvMassDPKHisto->Fill(InvMass);
            }
            break;
          case 1:                     // pion-
            if (P.GetIndex() == 2) {  // kaon+
              InvMassDPKHisto->Fill(InvMass);
            } else if (P.GetIndex() == 3) {  // kaon-
              InvMassCPKHisto->Fill(InvMass);
            }
            break;
          case 2:                     // kaon+
            if (P.GetIndex() == 0) {  // pion+
              InvMassCPKHisto->Fill(InvMass);
            } else if (P.GetIndex() == 1) {  // pion-
              InvMassDPKHisto->Fill(InvMass);
            }
            break;
          case 3:                     // kaon-
            if (P.GetIndex() == 0) {  // pion+
              InvMassDPKHisto->Fill(InvMass);
            } else if (P.GetIndex() == 1) {  // pion-
              InvMassCPKHisto->Fill(InvMass);
            }
            break;
          default:
            if (!(kIndex == 5 || kIndex == 4)) throw std::runtime_error("Particle index is not in the expected range 1");
        }
      }

      for (int k{100}; k < (100 + NResonanceDaughters); ++k) {  // this goes for the resonance daughters
        double InvMass = P.InvMass(EventParticles[k]);

        InvMassHisto->Fill(InvMass);

        if (EventParticles[k].GetCharge() != P.GetCharge())  // different charge
        {
          InvMassDiscChargesHisto->Fill(InvMass);
        } else  // same charge
        {
          InvMassConcChargesHisto->Fill(InvMass);
        }

        int kIndex = EventParticles[k].GetIndex();

        switch (kIndex) {
          case 0:                     // pion+
            if (P.GetIndex() == 2) {  // kaon+
              InvMassCPKHisto->Fill(InvMass);
            } else if (P.GetIndex() == 3) {  // kaon-
              InvMassDPKHisto->Fill(InvMass);
            }
            break;
          case 1:                     // pion-
            if (P.GetIndex() == 2) {  // kaon+
              InvMassDPKHisto->Fill(InvMass);
            } else if (P.GetIndex() == 3) {  // kaon-
              InvMassCPKHisto->Fill(InvMass);
            }
            break;
          case 2:                     // kaon+
            if (P.GetIndex() == 0) {  // pion+
              InvMassCPKHisto->Fill(InvMass);
            } else if (P.GetIndex() == 1) {  // pion-
              InvMassDPKHisto->Fill(InvMass);
            }
            break;
          case 3:                     // kaon-
            if (P.GetIndex() == 0) {  // pion+
              InvMassDPKHisto->Fill(InvMass);
            } else if (P.GetIndex() == 1) {  // pion-
              InvMassCPKHisto->Fill(InvMass);
            }
            break;
          default:
            if (!(kIndex == 5 || kIndex == 4)) throw std::runtime_error("Particle index is not in the expected range 2");
        }
      }
      // end of pion+/-/kaon+/- combinations
    }

    //--------------------------
    // i-th EVENT IS FINISHED
    //--------------------------

    for (int k{100}; k < (100 + NResonanceDaughters); ++k) {    // this goes for the each resonance daughter...
      for (int n{100}; n < (100 + NResonanceDaughters); ++n) {  // combined between themselves
        if (n == k) {                                           //  We are not combining a particle with itself
          continue;
        }
        double InvMass = EventParticles[n].InvMass(EventParticles[k]);

        InvMassHisto->Fill(InvMass);

        if (EventParticles[k].GetCharge() != EventParticles[n].GetCharge())  // different charge
        {
          InvMassDiscChargesHisto->Fill(InvMass);
        } else  // same charge
        {
          InvMassConcChargesHisto->Fill(InvMass);
        }

        int kIndex = EventParticles[k].GetIndex();

        switch (kIndex) {
          case 0:                                     // pion+
            if (EventParticles[n].GetIndex() == 2) {  // kaon+
              InvMassCPKHisto->Fill(InvMass);
            } else if (EventParticles[n].GetIndex() == 3) {  // kaon-
              InvMassDPKHisto->Fill(InvMass);
            }
            break;
          case 1:                                     // pion-
            if (EventParticles[n].GetIndex() == 2) {  // kaon+
              InvMassDPKHisto->Fill(InvMass);
            } else if (EventParticles[n].GetIndex() == 3) {  // kaon-
              InvMassCPKHisto->Fill(InvMass);
            }
            break;
          case 2:                                     // kaon+
            if (EventParticles[n].GetIndex() == 0) {  // pion+
              InvMassCPKHisto->Fill(InvMass);
            } else if (EventParticles[n].GetIndex() == 1) {  // pion-
              InvMassDPKHisto->Fill(InvMass);
            }
            break;
          case 3:                                     // kaon-
            if (EventParticles[n].GetIndex() == 0) {  // pion+
              InvMassDPKHisto->Fill(InvMass);
            } else if (EventParticles[n].GetIndex() == 1) {  // pion-
              InvMassCPKHisto->Fill(InvMass);
            }
            break;
          default:
            if (!(kIndex == 5 || kIndex == 4)) throw std::runtime_error("Particle index is not in the expected range 3");
        }
      }
    }

    //------------------------------------
    // Now we combine daughters of decay
    //------------------------------------

    for (int k{100}; k < (100 + NResonanceDaughters); k += 2) {
      double InvMass = EventParticles[k].InvMass(EventParticles[k + 1]);

      InvMassDecayDaughtersHisto->Fill(InvMass);
    }
  }
  //-----------------------------------------------------
  // All histograms filled, particle generation finished
  //-----------------------------------------------------
  TFile *file = new TFile("HistogramsFile.root", "ReCreate");

  ParticleTypeHisto->Write();
  AngleHisto->Write();
  ImpulseHisto->Write();
  TrasverseImpulseHisto->Write();
  EnergyHisto->Write();
  InvMassHisto->Write();
  InvMassDiscChargesHisto->Write();
  InvMassConcChargesHisto->Write();
  InvMassDPKHisto->Write();
  InvMassCPKHisto->Write();
  InvMassDecayDaughtersHisto->Write();

  file->Close();

  return 0;
}