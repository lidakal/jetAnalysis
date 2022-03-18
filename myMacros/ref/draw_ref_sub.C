// This function draws the histograms included in the ref_sub.root file
// and saves the images as PNG files

#include <iostream>
#include "stdio.h"
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include <vector>

void draw_ref_sub(bool qcd = true) 
{
    const int n = 2;
    
    // General plot options set by .rootrc
    //gStyle->SetPalette(57); //kBird
    
    // Load data
    TFile *fin = new TFile("ref_sub.root");
      
    if (qcd) {
        // 2d histogram
        TH2F *h_zgB = (TH2F *) fin->Get("h_incl_zgB");
        // extend axis to include underflow and overflow bins
        h_zgB->GetXaxis()->SetCanExtend(true);
        h_zgB->GetXaxis()->SetRange(0, h_zgB->GetNbinsX() + 1);
        // not saved / printed
        
        // Projections for qcdMC
        int firstbin;
        int lastbin;
        float minpt;
        float maxpt;
        
        // manipulate projection range
        maxpt = 120;
        firstbin = 0;
        lastbin = h_zgB->GetYaxis()->FindBin(maxpt);
        
        minpt = h_zgB->GetYaxis()->GetBinLowEdge(1);
        maxpt = h_zgB->GetYaxis()->GetBinLowEdge(lastbin);
        
        
        TH1F *h_zgB_proj = (TH1F *) h_zgB->ProjectionX("h_zgB_proj", firstbin, lastbin);
        h_zgB_proj->GetXaxis()->SetCanExtend(true);
        h_zgB_proj->GetXaxis()->SetRange(0, h_zgB_proj->GetNbinsX());
        //h_zgB_proj->Scale(1/h_zgB_proj->Integral("width"));
        //h_zgB_proj->SetXTitle("z_{g}");
        //h_zgB_proj->SetYTitle("1/N dN/dz_{g}");
        h_zgB_proj->SetMarkerStyle(kFullCircle);
        h_zgB_proj->SetMarkerColor(2);
        h_zgB_proj->SetLineColor(2);

        TH2F *h_zgL = (TH2F *) fin->Get("h_incl_zgL");
        TH1F *h_zgL_proj = (TH1F *) h_zgL->ProjectionX("h_zgL_proj", 1, 20);
        h_zgL_proj->Scale(1/h_zgL_proj->Integral("width"));
        h_zgL_proj->SetXTitle("z_{g}");
        h_zgL_proj->SetYTitle("1/N dN/dz_{g}");   
        h_zgL_proj->SetMarkerStyle(kFullSquare);

        TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.9);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);
        leg->AddEntry(h_zgB_proj, "b", "pl");
        leg->AddEntry(h_zgL_proj, "l", "pl");

        // Title
        std::string title = "z_{g} $\\in$ refpt [" + std::to_string((int) minpt) + ", " + std::to_string((int)maxpt) + "]";
        TText *text = new TText(0.5, 0.5, title.c_str());
        
        TCanvas *czg = new TCanvas("incl_zg_BL.png", "qcdMC b, l", 600, 600);
        h_zgB_proj->Draw();
        //h_zgL_proj->Draw("E2 SAME");
        leg->Draw();
        text->Draw();
        czg->Draw();
        czg->Print(czg->GetName(), "png");
    }
    
    /*
    TCanvas *c_zg = new TCanvas("czg", "refpt vs zg", 600, 400);  
    //czg->SetCanvasSize(1000, 1000);
    //czg->SetLeftMargin(0.5);
    czg->Divide(2, 1);
    
    czg->cd(1);
    hs_zg[0]->SetTitle("qcdMC");
    hs_zg[0]->SetStats(0);
    hs_zg[0]->SetXTitle("z_{g}");
    hs_zg[0]->SetYTitle("genpt");
    hs_zg[0]->GetYaxis()->SetTitleOffset(2.0);
    hs_zg[0]->SetStats(0);
    
    hs_zg[0]->Draw("COLZ");

    czg->cd(2);
    hs_zg[1]->SetTitle("bJetMC");
    hs_zg[1]->SetXTitle("z_{g}");
    hs_zg[1]->SetYTitle("genpt"); 
    hs_zg[1]->SetStats(0);
    hs_zg[1]->SetTitle("bJet");
    
    hs_zg[1]->Draw("COLZ");

    
    czg->Draw();
    czg->Print("genptVSzg.jpg", "jpg");
    
    // Projection histograms
    TCanvas *cproj_zg = new TCanvas("cproj_zg", "sum genpt vs zg", 600, 400); 
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
    hsproj_zg[0]->SetYTitle("1/N dN/dz_{g} for genpt in [50, 100] GeV");
    
    hsproj_zg[1]->SetMarkerStyle(kFullSquare);
    hsproj_zg[1]->SetMarkerColor(2);
    hsproj_zg[1]->SetTitle("bJetMC");
    
    hsproj_zg[0]->Draw("E");
    hsproj_zg[1]->Draw("SAME E");
    
    gPad->BuildLegend();
    hsproj_zg[0]->SetTitle("Projection");
    
    cproj_zg->Draw();
    cproj_zg->Print("zg_proj_gen.jpg", "jpg");
    
    // Rg histograms
    
    // 2D histograms
    
    TCanvas *crg = new TCanvas("crg", "genpt vs rg", 600, 400);   
    //crg->SetLeftMargin(0.2);
    crg->Divide(2, 1, 0.02);
    
    crg->cd(1);
    hs_rg[0]->SetTitle("qcdMC");
    hs_rg[0]->SetStats(0);
    hs_rg[0]->SetXTitle("R_{g}");
    hs_rg[0]->SetYTitle("genpt");
    hs_rg[0]->SetStats(0);
    
    hs_rg[0]->Draw("COLZ");

    crg->cd(2);
    hs_rg[1]->SetTitle("bJetMC");
    hs_rg[1]->SetXTitle("R_{g}");
    hs_rg[1]->SetYTitle("genpt"); 
    hs_rg[1]->SetStats(0);
    hs_rg[1]->SetTitle("bJetMC");
    
    hs_rg[1]->Draw("COLZ");

    crg->Draw();
    crg->Print("genptVSrg.jpg", "jpg");
    
    // Rg Projection
    TCanvas *cproj_rg = new TCanvas("cproj_rg", "sum genpt vs rg", 600, 400); 
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
    hsproj_rg[0]->SetXTitle("R_{g}");
    hsproj_rg[0]->SetYTitle("1/N dN/dR_{g} for genpt in [50, 100] GeV");
    
    hsproj_rg[1]->SetMarkerStyle(kFullSquare);
    hsproj_rg[1]->SetMarkerColor(2);
    hsproj_rg[1]->SetTitle("bJetMC");
    
    hsproj_rg[0]->Draw("E");
    hsproj_rg[1]->Draw("SAME E");
    
    gPad->BuildLegend();
    hsproj_rg[0]->SetTitle("Projection");
    
    cproj_rg->Draw();
    cproj_rg->Print("rg_proj_gen.png", "png");
    */
    
}