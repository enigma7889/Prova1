#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TMath.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"

void read() {
  //-------------------------------------------
  // Opening root file and reading histograms
  //-------------------------------------------

  TFile *file = new TFile("HistogramsFile.root", "READ");

  TH1D *h[12];

  TString s[10] = {"PTH", "IH", "TIH", "EH", "IMH", "IMBDCH", "IMBCCH", "DPKH", "CPKH", "IMDDH"};

  for (int j{0}; j < 10; ++j) {
    h[j] = (TH1D *)file->Get(s[j]);
  }

  TH1D *DiffDiscConcChargesHisto = new TH1D("DiffDiscConcChargesHisto", "difference of discording and concording charges", 1000, 0, 6);
  DiffDiscConcChargesHisto->Sumw2();
  DiffDiscConcChargesHisto->Add(h[5], h[6], 1, -1);
  h[10] = DiffDiscConcChargesHisto;

  TH1D *DiffDiscConcPKHisto = new TH1D("DiffDiscConcPKHisto", "difference of discording and concording pions and kaons", 1000, 0, 6);
  DiffDiscConcPKHisto->Sumw2();
  DiffDiscConcPKHisto->Add(h[7], h[8], 1, -1);
  h[11] = DiffDiscConcPKHisto;

  TH2D *AngleDistHisto = (TH2D *)file->Get("AH");  // 2D Histogram (Angle dist.)

  //----------------------------------------
  //  Canvas Creation and function fitting
  //----------------------------------------

  TCanvas *c1 = new TCanvas("c1", "c1", 10, 20, 1000, 600);
  c1->Divide(2, 2);

  c1->cd(1);
  h[0]->Draw("H");
  h[0]->Draw("E,P,SAME");

  c1->cd(2);
  TF1 *ExpFunction = new TF1("ExpFunction", "expo(0)", 0, 5);
  ExpFunction->SetParameters(0, -1);
  h[1]->Fit("ExpFunction");
  TF1 *ImpulseFitFunction = h[1]->GetFunction("ExpFunction");
  h[1]->Draw("H");
  h[1]->Draw("E,P,SAME");

  c1->cd(3);
  h[2]->Draw("H");
  h[2]->Draw("E,P,SAME");

  c1->cd(4);
  h[3]->Draw("H");
  h[3]->Draw("E,P,SAME");

  c1->Print("canvas/myCanvas1.gif");

  TCanvas *c2 = new TCanvas("c2", "c2", 10, 20, 1000, 600);
  c2->Divide(2, 2);

  for (int i{4}; i < 8; ++i) {
    c2->cd(i - 4 + 1);
    h[i]->Draw("H");
    h[i]->Draw("E,P,SAME");
  }
  c2->Print("canvas/myCanvas2.gif");

  TCanvas *c3 = new TCanvas("c3", "c3", 10, 20, 1000, 600);
  c3->Divide(2, 1);

  for (int i{8}; i < 10; ++i) {
    c3->cd(i - 8 + 1);
    h[i]->Draw("H");
    h[i]->Draw("E,P,SAME");
  }
  c3->Print("canvas/myCanvas3.gif");

  TCanvas *c4 = new TCanvas("c4", "c4", 10, 20, 1000, 600);
  c4->Divide(2, 1);

  c4->cd(1);
  TF1 *GaussianFunction1 = new TF1("GaussianFunction1", "gaus(0)", 0, 6);
  GaussianFunction1->SetParameters(1, 0.89166, 0.05);
  h[10]->Fit(GaussianFunction1);
  TF1 *InvMassDiffChargesFitFunction = h[10]->GetFunction("GaussianFunction1");
  h[10]->Draw("H");
  h[10]->Draw("E,P,SAME");

  c4->cd(2);
  TF1 *GaussianFunction2 = new TF1("GaussianFunction2", "gaus(0)", 0, 6);
  GaussianFunction2->SetParameters(1, 0.89166, 0.05);
  h[11]->Fit(GaussianFunction2);
  TF1 *InvMassDiffPKFitFunction = h[11]->GetFunction("GaussianFunction2");
  h[11]->Draw("H");
  h[11]->Draw("E,P,SAME");

  c4->Print("canvas/myCanvas4.gif");

  TCanvas *c5 = new TCanvas("c5", "c5", 10, 20, 1000, 600);
  TF1 *UniformFunction = new TF1("UniformFunction", "pol0(0)", 0, 2 * M_PI);  // phi (0,2pi) is x and theta (0,pi) is y
  UniformFunction->SetParameter(0, M_PI / 2);
  AngleDistHisto->Fit(UniformFunction);
  TF1 *AnglesFitFunction = AngleDistHisto->GetFunction("UniformFunction");
  AngleDistHisto->DrawCopy();

  c5->Print("canvas/myCanvas5.gif");

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

  std::cout << "Angles Histogram information: " << std::endl;
  std::cout << "Chisquare / NDF : " << AnglesFitFunction->GetChisquare() / AnglesFitFunction->GetNDF() << std::endl;
  std::cout << "Parameter 0 : " << AnglesFitFunction->GetParameter(0) << " +/- " << AnglesFitFunction->GetParError(0) << std::endl;
  std::cout << "Probability : " << AnglesFitFunction->GetProb() << std::endl << std::endl;

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