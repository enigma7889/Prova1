#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <cmath>
#include <iostream>

#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TROOT.h"
#include "TRandom.h"

void read() {
  TFile *file = new TFile("HistogramsFile.root", "READ");

  int NHistograms = 12;
  TH1D *h[NHistograms];

  int j{0};

  h[j] = (TH1D *)file->Get("PTH");

  for (int k{0}; k < 9; ++k) {
    std::cout << h[j]->GetBinContent(k) / 1.E7 << " +/- " << h[j]->GetBinError(k) / 1.E7 << std::endl;
  }
  ++j;

  h[j] = (TH1D *)file->Get("IH");
  /*
  TF1 *f1 = new TF1("f1", myFunction, 0, 5, 1);
  f1->SetParameter(0, 1);

  h[j]->Fit("f1");

  TF1 *ff1 = h[j]->GetFunction("f1");

  std::cout << ff1->GetChisquare() / ff1->GetNDF() << std::endl;
  std::cout << ff1->GetParameter(1) << " +/- " << ff1->GetParError(1) << std::endl;
  std::cout << ff1->GetProb() << std::endl << std::endl;
  */

  ++j;
  h[j] = (TH1D *)file->Get("TIH");
  ++j;
  h[j] = (TH1D *)file->Get("EH");
  ++j;
  h[j] = (TH1D *)file->Get("IMH");
  ++j;
  h[j] = (TH1D *)file->Get("IMBDCH");
  ++j;
  h[j] = (TH1D *)file->Get("IMBCCH");
  ++j;
  h[j] = (TH1D *)file->Get("PPPK");
  ++j;
  h[j] = (TH1D *)file->Get("PPNK");
  ++j;
  h[j] = (TH1D *)file->Get("NPPK");
  ++j;
  h[j] = (TH1D *)file->Get("NPNK");
  ++j;
  h[j] = (TH1D *)file->Get("IMDDH");

  TH2D *h2 = (TH2D *)file->Get("AH");

  TCanvas *c1 = new TCanvas("c1", "c1", 10, 20, 1000, 600);
  c1->Divide(3, 2);

  for (int i{0}; i < NHistograms / 2; ++i) {
    c1->cd(i + 1);
    h[i]->DrawCopy();
  }

  TCanvas *c2 = new TCanvas("c2", "c2", 10, 20, 1000, 600);
  h2->DrawCopy();

  TCanvas *c3 = new TCanvas("c3", "c3", 10, 20, 1000, 600);
  c3->Divide(3, 2);

  for (int i{5}; i < NHistograms; ++i) {
    c3->cd(i - 5 + 1);
    h[i]->DrawCopy();
  }

  file->Close();

  c1->Print("myCanvas1.gif");
  c2->Print("myCanvas2.gif");
  c3->Print("myCanvas3.gif");
}