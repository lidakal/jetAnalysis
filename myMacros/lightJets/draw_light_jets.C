#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"

using namespace std;

void draw_light_jets(bool ktCut = false) 
{
    // Load histograms from file
    string path_in = "~/rootFiles/lightJetHistos";
    if (ktCut) {
        path_in += "_ktCut.root";
    } else { 
        path_in += ".root";
    }
    
    TFile *fin = new TFile(path_in.c_str()); 
    
    TH2F *hL_zg_par = (TH2F *) fin->Get("hL_zg_par");
    TH2F *hL_zg_ref = (TH2F *) fin->Get("hL_zg_ref");
    TH2F *hL_zg_ref_dynKt = (TH2F *) fin->Get("hL_zg_ref_dynKt");
    
    TH2F *hL_rg_par = (TH2F *) fin->Get("hL_rg_par");
    TH2F *hL_rg_ref = (TH2F *) fin->Get("hL_rg_ref");
    TH2F *hL_rg_ref_dynKt = (TH2F *) fin->Get("hL_rg_ref_dynKt");
    
    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {80., 120.};
    Float_t highpt[2] = {200., 250.};
    
    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};
    
    
    // Draw 1D histograms - Projections
       
    TCanvas *c = new TCanvas("c", "c", 1800, 1000);
    c->Divide(3, 2);
    
    for (int i = 0; i < npt; i ++) {        
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];
        
        Int_t firstbin = hL_zg_par->GetYaxis()->FindBin(ptmin);
        Int_t lastbin = hL_zg_par->GetYaxis()->FindBin(ptmax);
        
        TH1F *hpar_zg = (TH1F *) hL_zg_par->ProjectionX("hpar_zg", firstbin, lastbin, "e")->Clone();
        TH1F *href_zg = (TH1F *) hL_zg_ref->ProjectionX("href_zg", firstbin, lastbin, "e")->Clone();
        TH1F *href_dynKt_zg = (TH1F *) hL_zg_ref_dynKt->ProjectionX("href_dynKt_zg", firstbin, lastbin, "e")->Clone();
        
        TH1F *hpar_rg = (TH1F *) hL_rg_par->ProjectionX("hpar_rg", firstbin, lastbin, "e")->Clone();
        TH1F *href_rg = (TH1F *) hL_rg_ref->ProjectionX("href_rg", firstbin, lastbin, "e")->Clone();
        TH1F *href_dynKt_rg = (TH1F *) hL_rg_ref_dynKt->ProjectionX("href_dynKt_rg", firstbin, lastbin, "e")->Clone();
        
        const int nh = 3;
        TH1F *hs_zg[nh] = {hpar_zg, href_zg, href_dynKt_zg};
        TH1F *hs_rg[nh] = {hpar_rg, href_rg, href_dynKt_rg};
        
        Float_t ymin_zg = 1e10;
        Float_t ymin_rg = 1e10;
        
        Float_t ymax_zg = 0;
        Float_t ymax_rg = 0;
        
        for (int j = 0; j < nh; j++) {            
            c->cd(i + 1);
            
            TH1F *h = hs_zg[j];
            
            // Extend to include underflow bin in normalisation
            h->GetXaxis()->SetCanExtend(true);
            h->GetXaxis()->SetRange(0, h->GetXaxis()->GetNbins());

            h->Scale(1/h->Integral("width"));

            // Fix the range drawn (underflow too large to show)
            h->GetXaxis()->SetRange(1, h->GetNbinsX());

            h->SetMarkerColor(j + 2);
            h->SetMarkerSize(0.8);
            h->SetLineWidth(2);
            h->SetXTitle("z_{g}");
            h->GetXaxis()->SetTitleOffset(2.5);
            h->SetYTitle("1/N dN/dz_{g}");
            h->GetYaxis()->SetTitleOffset(2.5);

            Float_t hymin_zg = h->GetMinimum();
            Float_t hymax_zg = h->GetMaximum();
            
            if (hymin_zg < ymin_zg) {
                ymin_zg = hymin_zg;
            }
            
            if (hymax_zg > ymax_zg) {
                ymax_zg = hymax_zg;
            }

            // To have a line outlining the bins
            h->SetFillColor(0);
            h->SetLineColor(j + 2);
            h->Draw("hist same");
            h->Draw("e same");
            
            c->cd(i + 4);
            h = hs_rg[j];
            
            // Extend to include underflow bin in normalisation
            h->GetXaxis()->SetCanExtend(true);
            h->GetXaxis()->SetRange(0, h->GetXaxis()->GetNbins());

            h->Scale(1/h->Integral("width"));

            // Fix the range drawn (underflow too large to show)
            h->GetXaxis()->SetRange(1, h->GetNbinsX());

            h->SetMarkerColor(j + 2);
            h->SetMarkerSize(0.8);
            h->SetLineWidth(2);
            h->SetXTitle("ln(1/R_{g})");
            h->GetXaxis()->SetTitleOffset(2.5);
            h->SetYTitle("1/N dN/dln(1/R_{g})");
            h->GetYaxis()->SetTitleOffset(2.5);

            Float_t hymin_rg = h->GetMinimum();
            Float_t hymax_rg = h->GetMaximum();
            
            if (hymin_rg < ymin_rg) {
                ymin_rg = hymin_rg;
            }
            
            if (hymax_rg > ymax_rg) {
                ymax_rg = hymax_rg;
            }

            // To have a line outlining the bins
            h->SetFillColor(0);
            h->SetLineColor(j + 2);
            h->Draw("hist same");
            h->Draw("e same");
        }
        
        if (!(ymin_rg > 0.)) {
            ymin_rg = 1e-4;
        }
        
        hpar_zg->GetYaxis()->SetRangeUser(ymin_zg, ymax_zg + 0.5);
        hpar_rg->GetYaxis()->SetRangeUser(ymin_rg, ymax_rg + 1.);
        //c->cd(i + 1)->SetLogy();
        c->cd(i + 4)->SetLogy();
        
        // Build legends
        c->cd(i + 1);
        TLegend *leg = new TLegend(0.35, 0.7, 0.85, 0.9);
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);
        leg->AddEntry(hpar_zg, Form("#kern[-0.2]{ } parton"), "pl");
        leg->AddEntry(href_zg, Form("#kern[-0.2]{ } hadron"), "pl");
        leg->AddEntry(href_dynKt_zg, Form("#kern[-0.2]{ } hadron, dynKt only"), "pl");
        leg->Draw();
        
        c->cd(i + 4);
        
        leg->SetBorderSize(0);
        leg->SetFillStyle(0);
        leg->AddEntry(hpar_zg, Form("#kern[-0.2]{ } parton"), "pl");
        leg->AddEntry(href_zg, Form("#kern[-0.2]{ } hadron"), "pl");
        leg->AddEntry(href_dynKt_zg, Form("#kern[-0.2]{ } hadron, dynKt only"), "pl");
        leg->Draw();
    }
    
    // Add pt ranges text on canvas
    c->cd(0);
    TPad *pad = new TPad("all", "all", 0, 0, 1, 1);
    pad->SetFillStyle(4000);  // transparent
    pad->Draw();
    pad->cd();
    
    TLatex *lefttitle = new TLatex();
    lefttitle->SetTextSize(30);
    lefttitle->DrawLatexNDC(0.1, 0.96, Form("p_{T} in [%.0f, %.0f] GeV", lowpt[0], lowpt[1]));
    
    TLatex *midtitle = new TLatex();
    midtitle->SetTextSize(30);
    midtitle->DrawLatexNDC(0.43, 0.96, Form("p_{T} in [%.0f, %.0f] GeV", midpt[0], midpt[1]));
    
    TLatex *righttitle = new TLatex();
    righttitle->SetTextSize(30);
    righttitle->DrawLatexNDC(0.76, 0.96, Form("p_{T} in [%.0f, %.0f] GeV", highpt[0], highpt[1]));

    TPaveText *text = new TPaveText(0.44, 0.46, 0.56, 0.54, "ndc");
    text->AddText("Light jets only.");
    if (ktCut) {
        text->AddText("kt 1 GeV cut.");
    } else {
        text->AddText("No kt cut.");
    }
    text->SetTextSize(30);
    text->Draw();
    
    c->Draw();
    
    string savename = "~/gitRepos/jetAnalysis/myMacros/lightJets/light_jets";
    if (ktCut) {
        savename += "_ktCut.png";
    } else {
        savename += ".png";
    }
    c->Print(savename.c_str());
    c->Show();
}
    
    
        
    
