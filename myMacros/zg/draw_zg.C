// This function draws the histograms included in the zg.root file
// and saves the image as a PNG file

#include <iostream>
#include "stdio.h"
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include <vector>

void draw_zg() {
    // General 
    gStyle->SetLegendTextSize(0.05);
    
    // Use to hide the title
    //gStyle->SetOptTitle(0);
    
    // Load data
    TFile *fin = new TFile("zg.root");
    TH1F *h_incl = (TH1F *) fin->Get("h_incl");
    TH1F *h_bJet = (TH1F *) fin->Get("h_bJet");
    
    TCanvas *c = new TCanvas("c", "c", 600, 400);
    
    h_incl->SetMarkerStyle(kFullCircle);
    h_incl->SetTitle("qcdMC");
    h_incl->SetStats(0);
    h_incl->SetXTitle("Zg");
    h_incl->SetYTitle("N");
    
    h_bJet->SetMarkerStyle(kFullSquare);
    h_bJet->SetTitle("bJetMC");
    h_bJet->SetStats(0);
    h_bJet->SetXTitle("Zg");
    
    h_incl->Draw("PLC PMC");
    h_bJet->Draw("SAME PLC PMC");
    
    gPad->BuildLegend();
    
    // Change the title of the first histogram to change the title of the graph
    h_incl->SetTitle("z_{g} of all jets in every event");
    
    c->Draw();
    c->Print("zg_distr.jpg", "jpg");
}