// This function draws the histograms included in the zg.root file
// and saves the image as a PNG file

#include <iostream>
#include "stdio.h"
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include <vector>

void draw_zg_rg() 
{
    const int n = 2;
    // General 
    gStyle->SetLegendTextSize(0.04);
    
    // Use to hide the title
    //gStyle->SetOptTitle(0);
    
    // Load data
    TFile *fin = new TFile("zg.root");
    TH2F *hs_zg[n] = {(TH2F *) fin->Get("h_incl_zg"), (TH2F *) fin->Get("h_bJet_zg")};
    TH2F *hs_rg[n] = {(TH2F *) fin->Get("h_incl_rg"), (TH2F *) fin->Get("h_bJet_rg")};
    
    // zg histograms    
      
    // 2D histograms
    TCanvas *czg = new TCanvas("czg", "parpt vs zg", 600, 400);  
    //czg->SetCanvasSize(1000, 1000);
    //czg->SetLeftMargin(0.5);
    czg->Divide(2, 1);
    
    czg->cd(1);
    hs_zg[0]->SetTitle("qcdMC");
    hs_zg[0]->SetStats(0);
    hs_zg[0]->SetXTitle("z_{g}");
    hs_zg[0]->SetYTitle("parpt");
    hs_zg[0]->GetYaxis()->SetTitleOffset(2.0);
    hs_zg[0]->SetStats(0);
    
    hs_zg[0]->Draw("COLZ");

    czg->cd(2);
    hs_zg[1]->SetTitle("bJetMC");
    hs_zg[1]->SetXTitle("z_{g}");
    hs_zg[1]->SetYTitle("parpt"); 
    hs_zg[1]->SetStats(0);
    hs_zg[1]->SetTitle("bJet");
    
    hs_zg[1]->Draw("COLZ");

    
    czg->Draw();
    czg->Print("parptVSzg.jpg", "jpg");
    
    // Projection histograms
    TCanvas *cproj_zg = new TCanvas("cproj_zg", "sum parpt vs zg", 600, 400); 
    TH1F *hsproj_zg[n] = {(TH1F *) hs_zg[0]->ProjectionX("hproj_incl", 1, 20), 
                       (TH1F *) hs_zg[1]->ProjectionX("hproj_bJet", 1, 20)};
    
    // Scale by bin width and total counts
    for (int i = 0; i < n; i++) {
        hsproj_zg[i]->Scale(1/(hsproj_zg[i]->Integral())); // it's not total, it's all the counts in the histo
        hsproj_zg[i]->Scale(1/(hsproj_zg[i]->GetXaxis()->GetBinWidth(0)));
    }
       
    hsproj_zg[0]->SetMarkerStyle(kFullCircle);
    hsproj_zg[0]->SetStats(0);
    hsproj_zg[0]->SetTitle("qcdMC");
    hsproj_zg[0]->SetXTitle("z_{g}");
    hsproj_zg[0]->SetYTitle("1/N dN/dz_{g} for parpt in [50, 100] GeV");
    
    hsproj_zg[1]->SetMarkerStyle(kFullSquare);
    hsproj_zg[1]->SetMarkerColor(2);
    hsproj_zg[1]->SetTitle("bJetMC");
    
    hsproj_zg[0]->Draw("E");
    hsproj_zg[1]->Draw("SAME E");
    
    gPad->BuildLegend();
    hsproj_zg[0]->SetTitle("Projection");
    
    cproj_zg->Draw();
    cproj_zg->Print("zg_proj.jpg", "jpg");
    
    // Rg histograms
    
    // 2D histograms
    TCanvas *crg = new TCanvas("crg", "parpt vs rg", 600, 400);   
    //crg->SetLeftMargin(0.2);
    crg->Divide(2, 1, 0.02);
    
    crg->cd(1);
    hs_rg[0]->SetTitle("qcdMC");
    hs_rg[0]->SetStats(0);
    hs_rg[0]->SetXTitle("R_{g}");
    hs_rg[0]->SetYTitle("parpt");
    hs_rg[0]->SetStats(0);
    
    hs_rg[0]->Draw("COLZ");

    crg->cd(2);
    hs_rg[1]->SetTitle("bJetMC");
    hs_rg[1]->SetXTitle("R_{g}");
    hs_rg[1]->SetYTitle("parpt"); 
    hs_rg[1]->SetStats(0);
    hs_rg[1]->SetTitle("bJetMC");
    
    hs_rg[1]->Draw("COLZ");

    crg->Draw();
    crg->Print("parptVSrg.jpg", "jpg");
    
    // Rg Projection
    TCanvas *cproj_rg = new TCanvas("cproj_rg", "sum parpt vs rg", 600, 400); 
    TH1F *hsproj_rg[n] = {(TH1F *) hs_rg[0]->ProjectionX("hproj_incl", 1, 20), 
                       (TH1F *) hs_rg[1]->ProjectionX("hproj_bJet", 1, 20)};
    
    // Scale by bin width and total counts
    for (int i = 0; i < n; i++) {
        hsproj_rg[i]->Scale(1/(hsproj_rg[i]->Integral())); // it's not total, it's all the counts in the histo
        hsproj_rg[i]->Scale(1/(hsproj_rg[i]->GetXaxis()->GetBinWidth(0)));
    }
       
    hsproj_rg[0]->SetMarkerStyle(kFullCircle);
    hsproj_rg[0]->SetStats(0);
    hsproj_rg[0]->SetTitle("qcdMC");
    hsproj_rg[0]->SetXTitle("z_{g}");
    hsproj_rg[0]->SetYTitle("1/N dN/dz_{g} for parpt in [50, 100] GeV");
    
    hsproj_rg[1]->SetMarkerStyle(kFullSquare);
    hsproj_rg[1]->SetMarkerColor(2);
    hsproj_rg[1]->SetTitle("bJetMC");
    
    hsproj_rg[0]->Draw("E");
    hsproj_rg[1]->Draw("SAME E");
    
    gPad->BuildLegend();
    hsproj_rg[0]->SetTitle("Projection");
    
    cproj_rg->Draw();
    cproj_rg->Print("rg_proj.jpg", "jpg");
}