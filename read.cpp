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

  TH1D *hDC = new TH1D("HDC", "difference of discording and concording charges", 1000, 0, 6);
  // hDC->Sumw2();
  hDC->Add(h[5], h[6], 1, -1);
  h[10] = hDC;

  TH1D *hDPK = new TH1D("HDPK", "difference of discording and concording pions and kaons", 1000, 0, 6);
  // hDPK->Sumw2();
  hDPK->Add(h[7], h[8], 1, -1);
  h[11] = hDPK;

  TH2D *h2D = (TH2D *)file->Get("AH");  // 2D Histogram (Angle dist.)

  //----------------------------------------
  //  Canvas Creation and function fitting
  //----------------------------------------

  TCanvas *c1 = new TCanvas("c1", "c1", 10, 20, 1000, 600);
  c1->Divide(2, 2);

  c1->cd(1);
  h[0]->Draw("H");
  h[0]->Draw("E,P,SAME");

  c1->cd(2);
  TF1 *f1 = new TF1("f1", "expo(0)", 0, 5);
  f1->SetParameters(0, 1);
  h[1]->Fit("f1");
  TF1 *ff1 = h[1]->GetFunction("f1");
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

  for (int i{10}; i < 12; ++i) {
    c4->cd(i - 10 + 1);
    h[i]->Draw("H");
    h[i]->Draw("E,P,SAME");
  }
  c4->Print("canvas/myCanvas4.gif");

  TCanvas *c5 = new TCanvas("c5", "c5", 10, 20, 1000, 600);
  TF1 *fU1 = new TF1("fU1", "pol0(0)", 0, 2 * M_PI);  // phi (0,2pi) is x and theta (0,pi) is y
  fU1->SetParameter(0, M_PI / 2);
  h2D->Fit(fU1);
  TF1 *ffU1 = h2D->GetFunction("fU1");
  h2D->Draw("H");
  h2D->Draw("E,P,SAME");

  /*
  std::cout << ffU1->GetChisquare() / ffU1->GetNDF() << std::endl;
  std::cout << ffU1->GetParameter(0) << " +/- " << ffU1->GetParError(0) << std::endl;
  std::cout << ffU1->GetParameter(1) << " +/- " << ffU1->GetParError(1) << std::endl;
  std::cout << ffU1->GetProb() << std::endl << std::endl;
  */

  c5->Print("canvas/myCanvas5.gif");

  //------------------------------------------------------
  //  Printing Histograms and fit functions information
  //------------------------------------------------------

  std::cout << "Particle Type Histogram information: " << std::endl;
  for (int k{0}; k < 9; ++k) {
    std::cout << h[0]->GetBinContent(k) / 1.E7 << " +/- " << h[0]->GetBinError(k) / 1.E7 << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Impulse Histogram information: " << std::endl;
  std::cout << ff1->GetChisquare() / ff1->GetNDF() << std::endl;
  std::cout << ff1->GetParameter(0) << " +/- " << ff1->GetParError(0) << std::endl;
  std::cout << ff1->GetParameter(1) << " +/- " << ff1->GetParError(1) << std::endl;
  std::cout << ff1->GetProb() << std::endl << std::endl;

  file->Close();
}