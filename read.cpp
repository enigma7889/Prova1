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

  h[j] = (TH1D *)file->Get("PTH");  // 0, particletype histogram
  std::cout << "Particle Type Histogram information: " << std::endl;

  for (int k{0}; k < 9; ++k) {
    std::cout << h[j]->GetBinContent(k) / 1.E7 << " +/- " << h[j]->GetBinError(k) / 1.E7 << std::endl;
  }
  std::cout << std::endl;
  ++j;

  h[j] = (TH1D *)file->Get("IH");  // 1, impulse histogram

  TF1 *f1 = new TF1("f1", "expo(0)", 0, 5);
  f1->SetParameters(0, 1);

  h[j]->Fit("f1");

  TF1 *ff1 = h[j]->GetFunction("f1");

  std::cout << ff1->GetChisquare() / ff1->GetNDF() << std::endl;
  std::cout << ff1->GetParameter(0) << " +/- " << ff1->GetParError(0) << std::endl;
  std::cout << ff1->GetParameter(1) << " +/- " << ff1->GetParError(1) << std::endl;
  std::cout << ff1->GetProb() << std::endl << std::endl;
  ++j;

  h[j] = (TH1D *)file->Get("TIH");  // 2
  ++j;
  h[j] = (TH1D *)file->Get("EH");  // 3
  ++j;
  h[j] = (TH1D *)file->Get("IMH");  // 4
  ++j;
  h[j] = (TH1D *)file->Get("IMBDCH");  // 5
  ++j;
  h[j] = (TH1D *)file->Get("IMBCCH");  // 6
  ++j;
  h[j] = (TH1D *)file->Get("PPPK");  // 7 concording
  ++j;
  h[j] = (TH1D *)file->Get("PPNK");  // 8 disc.
  ++j;
  h[j] = (TH1D *)file->Get("NPPK");  // 9 disc.
  ++j;
  h[j] = (TH1D *)file->Get("NPNK");  // 10 concording
  ++j;
  h[j] = (TH1D *)file->Get("IMDDH");  // 11

  TH1D *hDC = new TH1D("HDC", "difference of discording and concording charges", 1000, 0, 6);
  hDC->Add(h[5], h[6], 1, -1);

  TH1D *hDCPK = new TH1D("HDCPK", "sum of discording pions and kaons", 1000, 0, 6);
  hDCPK->Add(h[7], h[10]);
  TH1D *hCCPK = new TH1D("HCCPK", "sum of concording pions and kaons", 1000, 0, 6);
  hCCPK->Add(h[8], h[9]);

  TH1D *hDPK = new TH1D("HDPK", "difference of discording and concording pions and kaons", 1000, 0, 6);
  hDPK->Add(hDCPK, hCCPK, 1, -1);

  TH2D *h2D = (TH2D *)file->Get("AH");  // 2D Histogram (Angle dist.)

  TF1 *fU1 = new TF1("fU1", "[0]+x*[1]", 0, 2 * M_PI);  // phi is x and theta is y
  fU1->SetParameters(M_PI / 2, 0);
  h2D->Fit(fU1);
  TF1 *ffU1 = h[j]->GetFunction("fU1");
  /*
  std::cout << ffU1->GetChisquare() / ffU1->GetNDF() << std::endl;
  std::cout << ffU1->GetParameter(0) << " +/- " << ffU1->GetParError(0) << std::endl;
  std::cout << ffU1->GetParameter(1) << " +/- " << ffU1->GetParError(1) << std::endl;
  std::cout << ffU1->GetProb() << std::endl << std::endl;
  */

  //-----------------------------------------------------------
  //----------------Canvas-Creation----------------------------
  //-----------------------------------------------------------

  TCanvas *c1 = new TCanvas("c1", "c1", 10, 20, 1000, 600);
  c1->Divide(2, 2);

  for (int i{0}; i < 4; ++i) {
    c1->cd(i + 1);
    h[i]->DrawCopy();
  }

  TCanvas *c2 = new TCanvas("c2", "c2", 10, 20, 1000, 600);
  c2->Divide(2, 2);

  for (int i{4}; i < 8; ++i) {
    c2->cd(i - 4 + 1);
    h[i]->DrawCopy();
  }

  TCanvas *c3 = new TCanvas("c3", "c3", 10, 20, 1000, 600);
  c3->Divide(2, 2);

  for (int i{8}; i < 12; ++i) {
    c3->cd(i - 8 + 1);
    h[i]->DrawCopy();
  }

  TCanvas *c4 = new TCanvas("c4", "c4", 10, 20, 1000, 600);
  c4->Divide(2, 2);
  c4->cd(1);
  h2D->DrawCopy();
  c4->cd(2);
  hDC->DrawCopy();
  c4->cd(3);
  hDPK->DrawCopy();

  c1->Print("canvas/myCanvas1.gif");
  c2->Print("canvas/myCanvas2.gif");
  c3->Print("canvas/myCanvas3.gif");
  c4->Print("canvas/myCanvas4.gif");

  file->Close();
}