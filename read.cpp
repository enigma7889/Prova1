#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TLegend.h"
#include "TMath.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"
#include "TStyle.h"

void read() {
  //-------------------------------------------
  // Opening root file and reading histograms
  //-------------------------------------------

  TFile *file = new TFile("HistogramsFile.root", "READ");

  TH1D *h[14];

  TString s[12] = {"PTH", "IH", "TIH", "AAH", "PAH", "EH", "IMH", "IMBDCH", "IMBCCH", "DPKH", "CPKH", "IMDDH"};

  for (int j{0}; j < 12; ++j) {
    h[j] = (TH1D *)file->Get(s[j]);
    h[j]->GetYaxis()->SetTitleOffset(1.4);
    h[j]->GetXaxis()->SetTitleSize(0.04);
    h[j]->GetYaxis()->SetTitleSize(0.04);
    h[j]->GetYaxis()->SetTitle("Entries");
    h[j]->SetFillColor(kGreen);
    h[j]->SetFillStyle(3003);
  }

  TH1D *DiffDiscConcChargesHisto = new TH1D("DiffDiscConcChargesHisto", "difference of discording and concording charges", 1000, 0, 6);
  DiffDiscConcChargesHisto->Sumw2();
  DiffDiscConcChargesHisto->Add(h[7], h[8], 1, -1);
  h[12] = DiffDiscConcChargesHisto;

  TH1D *DiffDiscConcPKHisto = new TH1D("DiffDiscConcPKHisto", "difference of discording and concording pions and kaons", 1000, 0, 6);
  DiffDiscConcPKHisto->Sumw2();
  DiffDiscConcPKHisto->Add(h[9], h[10], 1, -1);
  h[13] = DiffDiscConcPKHisto;

  //----------------------------------------
  //  Canvas Creation and function fitting
  //----------------------------------------

  TCanvas *c1 = new TCanvas("c1", "c1", 10, 20, 1300, 900);
  c1->Divide(2, 2);

  c1->cd(1);  // Particle Types
  gStyle->SetOptStat(10);
  h[0]->GetXaxis()->SetTitle("Particle Index");
  h[0]->Draw("H");
  h[0]->Draw("E,P,SAME");

  c1->cd(2);  // Impulse
  gStyle->SetOptFit(1111);
  TF1 *ExpFunction = new TF1("ExpFunction", "expo(0)", 0, 5);
  ExpFunction->SetParameters(0, -1);
  h[1]->Fit("ExpFunction");
  TF1 *ImpulseFitFunction = h[1]->GetFunction("ExpFunction");
  h[1]->GetXaxis()->SetTitle("Impulse (GeV)");
  h[1]->Draw("H");
  h[1]->Draw("E,P,SAME");

  c1->cd(3);  // Azimuthal angle phi
  gStyle->SetOptStat(1110);
  TF1 *UniformFunction1 = new TF1("UniformFunction1", "pol0(0)", 0, 2 * M_PI);
  UniformFunction1->SetParameter(0, 1000);
  h[3]->Fit(UniformFunction1);
  TF1 *AzimuthalAngleFitFunction = h[3]->GetFunction("UniformFunction1");
  h[3]->GetXaxis()->SetTitle("Phi (Rad)");
  h[3]->DrawCopy();

  c1->cd(4);  // Polar angle theta
  TF1 *UniformFunction2 = new TF1("UniformFunction2", "pol0(0)", 0, M_PI);
  UniformFunction2->SetParameter(0, 1000);
  h[4]->Fit(UniformFunction2);
  TF1 *PolarAngleFitFunction = h[4]->GetFunction("UniformFunction2");
  h[4]->GetXaxis()->SetTitle("Theta (Rad)");
  h[4]->DrawCopy();

  c1->Print("canvas/myCanvas1.gif");

  TCanvas *c2 = new TCanvas("c2", "c2", 10, 20, 1600, 500);
  c2->Divide(3, 1);

  c2->cd(1);  // Inv Mass between Decay Pairs
  gStyle->SetOptStat(10);
  TF1 *GaussianFunction1 = new TF1("GaussianFunction1", "gaus(0)", 0, 6);
  GaussianFunction1->SetParameters(1, 0.89166, 0.05);
  h[11]->Fit(GaussianFunction1);
  TF1 *InvMassDecayPairsFitFunction = h[11]->GetFunction("GaussianFunction1");
  h[11]->GetYaxis()->SetTitleOffset(1.4);
  h[11]->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  /*
  TLegend *leg1 = new TLegend(.1,.7,.3,.9,"1");
  leg1->SetFillColor(0);
  leg1->AddEntry(h[11],"2");
  leg1->AddEntry(InvMassDecayPairsFitFunction,"3");
  leg1->Draw();
  */
  h[11]->Draw("H");
  h[11]->Draw("E,P,SAME");

  c2->cd(2);  // Inv Mass between Different Charges
  TF1 *GaussianFunction2 = new TF1("GaussianFunction2", "gaus(0)", 0, 6);
  GaussianFunction2->SetParameters(1, 0.89166, 0.05);
  h[12]->Fit(GaussianFunction2);
  TF1 *InvMassDiffChargesFitFunction = h[12]->GetFunction("GaussianFunction2");
  h[12]->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  h[12]->Draw("H");
  h[12]->Draw("E,P,SAME");

  c2->cd(3);  // Inv Mass between Pions and Kaons
  TF1 *GaussianFunction3 = new TF1("GaussianFunction3", "gaus(0)", 0.6, 1.2);
  GaussianFunction3->SetParameters(1, 0.89166, 0.05);
  h[13]->Fit(GaussianFunction3);
  TF1 *InvMassDiffPKFitFunction = h[13]->GetFunction("GaussianFunction3");
  h[13]->GetXaxis()->SetTitle("Invariant Mass (GeV/c^2)");
  h[13]->Draw("H");
  h[13]->Draw("E,P,SAME");

  c2->Print("canvas/myCanvas2.gif");

  TCanvas *c3 = new TCanvas("c3", "c3", 10, 20, 1200, 500);
  c3->Divide(3, 1);
  gStyle->SetOptStat(1000000010);
  c3->cd(1);
  gStyle->SetOptStat(10);
  h[2]->Draw("E,P,H");
  c3->cd(2);
  h[5]->Draw("E,P,H");
  c3->cd(3);
  h[6]->Draw("E,P,H");

  c3->Print("canvas/myCanvas3.gif");

  //------------------------------------------------------
  //  Printing Histograms and fit functions information
  //------------------------------------------------------

  std::cout << "Particle Type Histogram information: " << std::endl;
  for (int k{0}; k < 9; ++k) {
    std::cout << "The probability of the " << k << " particle is : " << h[0]->GetBinContent(k) / 1.E7 << " +/- " << h[0]->GetBinError(k) / 1.E7 << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Impulse Histogram information: " << std::endl;
  std::cout << "Chisquare / NDF : " << ImpulseFitFunction->GetChisquare() / ImpulseFitFunction->GetNDF() << std::endl;
  std::cout << "Parameter 0 : " << ImpulseFitFunction->GetParameter(0) << " +/- " << ImpulseFitFunction->GetParError(0) << std::endl;
  std::cout << "Parameter 1 : " << ImpulseFitFunction->GetParameter(1) << " +/- " << ImpulseFitFunction->GetParError(1) << std::endl;
  std::cout << "Probability : " << ImpulseFitFunction->GetProb() << std::endl << std::endl;

  std::cout << "Azimuthal Angle Histogram information: " << std::endl;
  std::cout << "Chisquare / NDF : " << AzimuthalAngleFitFunction->GetChisquare() / AzimuthalAngleFitFunction->GetNDF() << std::endl;
  std::cout << "Parameter 0 : " << AzimuthalAngleFitFunction->GetParameter(0) << " +/- " << AzimuthalAngleFitFunction->GetParError(0) << std::endl;
  std::cout << "Probability : " << AzimuthalAngleFitFunction->GetProb() << std::endl << std::endl;

  std::cout << "Polar Angle Histogram information: " << std::endl;
  std::cout << "Chisquare / NDF : " << PolarAngleFitFunction->GetChisquare() / PolarAngleFitFunction->GetNDF() << std::endl;
  std::cout << "Parameter 0 : " << PolarAngleFitFunction->GetParameter(0) << " +/- " << PolarAngleFitFunction->GetParError(0) << std::endl;
  std::cout << "Probability : " << PolarAngleFitFunction->GetProb() << std::endl << std::endl;

  std::cout << "Inv Mass of Decay Pairs Histogram information: " << std::endl;
  std::cout << "Chisquare / NDF : " << InvMassDecayPairsFitFunction->GetChisquare() / InvMassDecayPairsFitFunction->GetNDF() << std::endl;
  std::cout << "Parameter 0 : " << InvMassDecayPairsFitFunction->GetParameter(0) << " +/- " << InvMassDecayPairsFitFunction->GetParError(0) << std::endl;
  std::cout << "Parameter 1 : " << InvMassDecayPairsFitFunction->GetParameter(1) << " +/- " << InvMassDecayPairsFitFunction->GetParError(1) << std::endl;
  std::cout << "Parameter 2 : " << InvMassDecayPairsFitFunction->GetParameter(2) << " +/- " << InvMassDecayPairsFitFunction->GetParError(2) << std::endl;
  std::cout << "Probability : " << InvMassDecayPairsFitFunction->GetProb() << std::endl << std::endl;

  std::cout << "Inv Mass Difference of Charges Histogram information: " << std::endl;
  std::cout << "Chisquare / NDF : " << InvMassDiffChargesFitFunction->GetChisquare() / InvMassDiffChargesFitFunction->GetNDF() << std::endl;
  std::cout << "Parameter 0 : " << InvMassDiffChargesFitFunction->GetParameter(0) << " +/- " << InvMassDiffChargesFitFunction->GetParError(0) << std::endl;
  std::cout << "Parameter 1 : " << InvMassDiffChargesFitFunction->GetParameter(1) << " +/- " << InvMassDiffChargesFitFunction->GetParError(1) << std::endl;
  std::cout << "Parameter 2 : " << InvMassDiffChargesFitFunction->GetParameter(2) << " +/- " << InvMassDiffChargesFitFunction->GetParError(2) << std::endl;
  std::cout << "Probability : " << InvMassDiffChargesFitFunction->GetProb() << std::endl << std::endl;

  std::cout << "Inv Mass Difference of Pion/Kaon Histogram information: " << std::endl;
  std::cout << "Chisquare / NDF : " << InvMassDiffPKFitFunction->GetChisquare() / InvMassDiffPKFitFunction->GetNDF() << std::endl;
  std::cout << "Parameter 0 : " << InvMassDiffPKFitFunction->GetParameter(0) << " +/- " << InvMassDiffPKFitFunction->GetParError(0) << std::endl;
  std::cout << "Parameter 1 : " << InvMassDiffPKFitFunction->GetParameter(1) << " +/- " << InvMassDiffPKFitFunction->GetParError(1) << std::endl;
  std::cout << "Parameter 2 : " << InvMassDiffPKFitFunction->GetParameter(2) << " +/- " << InvMassDiffPKFitFunction->GetParError(2) << std::endl;
  std::cout << "Probability : " << InvMassDiffPKFitFunction->GetProb() << std::endl << std::endl;

  file->Close();
}