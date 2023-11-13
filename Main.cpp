#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <cmath>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
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
  TH1D *InvMassDiscChargesHisto = new TH1D("IMBDCH", "InvMass Distribution between discording charges", 1000, 0, 6);
  TH1D *InvMassConcChargesHisto = new TH1D("IMBCCH", "InvMass Distribution between concording charges", 1000, 0, 6);

  TH1D *InvMassPPPKHisto = new TH1D("PPPK", "InvMass Distribution between Positive Pion and Positive Kaon", 1000, 0, 6);
  TH1D *InvMassPPNKHisto = new TH1D("PPNK", "InvMass Distribution between Positive Pion and Negative Kaon", 1000, 0, 6);
  TH1D *InvMassNPPKHisto = new TH1D("NPPK", "InvMass Distribution between Negative Pion and Positive Kaon", 1000, 0, 6);
  TH1D *InvMassNPNKHisto = new TH1D("NPNK", "InvMass Distribution between Negative Pion and Negative Kaon", 1000, 0, 6);

  TH1D *InvMassDecayDaughtersHisto = new TH1D("IMDDH", "InvMass Distribution between daughters of Decay", 300, 0.6, 1.2);

  InvMassHisto->Sumw2();
  InvMassDiscChargesHisto->Sumw2();
  InvMassConcChargesHisto->Sumw2();
  InvMassPPPKHisto->Sumw2();
  InvMassPPNKHisto->Sumw2();
  InvMassNPPKHisto->Sumw2();
  InvMassNPNKHisto->Sumw2();
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
        EventParticles[100 + NResonanceDaughters] = p1;
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

      if (EventParticles[j].GetIndex() != 6) {  // Here we are discarding the case P is K*

        //--------------------------------------------------------------
        // InvMass for all the cases of a single event Histogram Filling
        //--------------------------------------------------------------
        for (int k{0}; k < j; ++k) {                // this goes for the first non-empty part of the array
          if (EventParticles[k].GetIndex() != 6) {  // Here we are excluding K* particles

            double InvMass = EventParticles[j].InvMass(EventParticles[k]);

            InvMassHisto->Fill(InvMass);

            if (EventParticles[k].GetCharge() != EventParticles[j].GetCharge())  // different charge
              InvMassDiscChargesHisto->Fill(InvMass);
            else  // same charge
              InvMassConcChargesHisto->Fill(InvMass);

            switch (EventParticles[k].GetIndex()) {
              case 0:                                     // pion+
                if (EventParticles[j].GetIndex() == 2) {  // kaon+
                  InvMassPPPKHisto->Fill(InvMass);
                } else if (EventParticles[j].GetIndex() == 3) {  // kaon-
                  InvMassPPNKHisto->Fill(InvMass);
                }
                break;
              case 1:                                     // pion-
                if (EventParticles[j].GetIndex() == 2) {  // kaon+
                  InvMassNPPKHisto->Fill(InvMass);
                } else if (EventParticles[j].GetIndex() == 3) {  // kaon-
                  InvMassNPNKHisto->Fill(InvMass);
                }
                break;
              case 2:                                     // kaon+
                if (EventParticles[j].GetIndex() == 0) {  // pion+
                  InvMassPPPKHisto->Fill(InvMass);
                } else if (EventParticles[j].GetIndex() == 1) {  // pion-
                  InvMassNPPKHisto->Fill(InvMass);
                }
                break;
              case 3:                                     // kaon-
                if (EventParticles[j].GetIndex() == 0) {  // pion+
                  InvMassPPNKHisto->Fill(InvMass);
                } else if (EventParticles[j].GetIndex() == 1) {  // pion-
                  InvMassNPPKHisto->Fill(InvMass);
                }
                break;
            }
          }
        }

        for (int k{100}; k < (100 + NResonanceDaughters); ++k) {  // this goes for the resonance daughters
          double InvMass = EventParticles[j].InvMass(EventParticles[k]);

          InvMassHisto->Fill(InvMass);

          if (EventParticles[k].GetCharge() != EventParticles[j].GetCharge())  // different charge
            InvMassDiscChargesHisto->Fill(InvMass);
          else  // same charge
            InvMassConcChargesHisto->Fill(InvMass);

          switch (EventParticles[k].GetIndex()) {
            case 0:                                     // pion+
              if (EventParticles[j].GetIndex() == 2) {  // kaon+
                InvMassPPPKHisto->Fill(InvMass);
              } else if (EventParticles[j].GetIndex() == 3) {  // kaon-
                InvMassPPNKHisto->Fill(InvMass);
              }
              break;
            case 1:                                     // pion-
              if (EventParticles[j].GetIndex() == 2) {  // kaon+
                InvMassNPPKHisto->Fill(InvMass);
              } else if (EventParticles[j].GetIndex() == 3) {  // kaon-
                InvMassNPNKHisto->Fill(InvMass);
              }
              break;
            case 2:                                     // kaon+
              if (EventParticles[j].GetIndex() == 0) {  // pion+
                InvMassPPPKHisto->Fill(InvMass);
              } else if (EventParticles[j].GetIndex() == 1) {  // pion-
                InvMassNPPKHisto->Fill(InvMass);
              }
              break;
            case 3:                                     // kaon-
              if (EventParticles[j].GetIndex() == 0) {  // pion+
                InvMassPPNKHisto->Fill(InvMass);
              } else if (EventParticles[j].GetIndex() == 1) {  // pion-
                InvMassNPPKHisto->Fill(InvMass);
              }
              break;
          }
        }
        // end of pion+/-/kaon+/- combinations
      }
    }

    //--------------------------------------------------------------------
    // Now we combine daughters of decay, ONLY AFTER THE EVENT IS FINISHED
    //--------------------------------------------------------------------

    for (int k{100}; k < (100 + NResonanceDaughters); k += 2) {
      double InvMass = EventParticles[k].InvMass(EventParticles[k + 1]);

      InvMassDecayDaughtersHisto->Fill(InvMass);
    }
  }
  //---------------------------------------------------
  // All histograms filled, particle generation finished
  //---------------------------------------------------
  TFile *file = new TFile("HistogramsFile.root", "ReCreate");

  ParticleTypeHisto->Write();
  AngleHisto->Write();
  ImpulseHisto->Write();
  TrasverseImpulseHisto->Write();
  EnergyHisto->Write();
  InvMassHisto->Write();
  InvMassDiscChargesHisto->Write();
  InvMassConcChargesHisto->Write();
  InvMassPPPKHisto->Write();
  InvMassNPPKHisto->Write();
  InvMassPPNKHisto->Write();
  InvMassNPNKHisto->Write();
  InvMassDecayDaughtersHisto->Write();

  file->Close();

  return 0;
}