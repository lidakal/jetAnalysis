#include <iostream>
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include "TCanvas.h"

void draw2dhist(){
  TFile *f = new TFile("myFile.root");
  TH2F *h = (TH2F*)f->Get("nrefVSngen");
  TCanvas *c = new TCanvas("c", "c", 500, 500);
  h->Draw("COLZ");
  h->SetStats(0);
}
