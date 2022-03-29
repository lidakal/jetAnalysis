#include "TH3F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLine.h"
#include "TSystem.h"
#include <regex>

using namespace std;

void draw_pt2dscan_ratio_charged(char qtype = 'B', bool ktORzg = true, bool dynKt = true)
{       
    string histfile_ref = "~/rootFiles/pt2dscan_ref_charged.root";
    string histfile_par = "~/rootFiles/pt2dscan_par_charged.root";

    // Create names / titles / labels based on settings
    string hname = "h";
    string xtitle = "ln(1/R_{g})";
    string ytitle = "";
    string title = "";
    string savename = "~/gitRepos/jetAnalysis/myMacros/pt2dscan/chargedPlots/ratio/pt2dscan";

   // MC set
    if (qtype == 'B') {
        hname += "_bJet";
    } else {
        hname += "_qcd";
    }
    // Variables
    if (ktORzg) {
        hname += "_rgkt";
        savename += "_rgkt";
        ytitle += "ln(k_{T}/GeV)";
    } else {
        hname += "_rgzg";
        savename += "_rgzg";
        ytitle += "z_{g}";
    }
    // Jet Flavour
    if (qtype == 'B') {
        hname += "B";
        title += "b-jets";
        savename += "_bjets";
    } else if (qtype == 'C') {
        hname += "C";
        title += "c-jets";
        savename += "_cjets";
    } else {
        hname += "L";
        title += "l-jets";
        savename += "_ljets";
    }

    string hname_par = hname;
    title += ", all events";

    if (dynKt) { 
        hname += "_dynKt";
        savename += "_dynKt";
    }
    
    savename += "_ratio";
    savename += "_charged";

    // Load ref 3D histogram -- X = rg, Y = zg / kt, Z = pt 
    TFile *fin_ref = new TFile(histfile_ref.c_str());
    TH3F *h3d_ref = (TH3F *) fin_ref->Get(hname.c_str())->Clone();

    // Load par 3D histogram -- X = rg, Y = zg / kt, Z = pt 
    TFile *fin_par = new TFile(histfile_par.c_str());
    TH3F *h3d_par = (TH3F *) fin_par->Get(hname_par.c_str())->Clone();
    
    // Create / Draw 2D histograms - Projections
    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {80., 120.};
    Float_t highpt[2] = {200., 250.};
    
    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};
    
    TCanvas *c = new TCanvas("c", "c", 1800, 600);
    c->Divide(3, 1);

    for (int i = 0; i < npt; i ++){
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];
        
        Float_t zmin = 0.;
        Float_t zmax = 25.;
        
        if (qtype == 'C') { 
            zmax = 20.;
        } else if (qtype == 'L') {
            zmax = 5.;
        }
        
        if (!ktORzg) {
        }  
        
        TPaveText *text = new TPaveText(0.53, 0.68, 0.81, 0.87, "ndc");
        text->AddText(title.c_str());
        if (dynKt) {
            text->AddText("dynKt only");
        } else {
            text->AddText("no dynKt");
        }
        text->AddText("ratio truth/parton");
        text->AddText("charged only");
        text->SetTextSize(20);

        TH3F *h3d_ref_clone = (TH3F *) h3d_ref->Clone();
        TH3F *h3d_par_clone = (TH3F *) h3d_par->Clone();

        h3d_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_par_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);

        TH2F *h2d_ref = (TH2F *) h3d_ref_clone->Project3D(Form("yx%d_ref", i)); 
        TH2F *h2d_par = (TH2F *) h3d_par_clone->Project3D(Form("yx%d_par", i)); 

        c->cd(i + 1);
        //c->cd(i + 1)->SetLogz();
        // Normalise the histogram
        h2d_par->GetXaxis()->SetRange(0, h2d_par->GetNbinsX()+1);
        h2d_par->GetYaxis()->SetRange(0, h2d_par->GetNbinsY()+1);
        h2d_par->Scale(1/h2d_par->Integral("width"));
        // Hide the under/overflow again
        h2d_par->GetXaxis()->SetRange(1, h2d_par->GetNbinsX());
        h2d_par->GetYaxis()->SetRange(1, h2d_par->GetNbinsY());

        h2d_ref->SetYTitle(ytitle.c_str());
        h2d_ref->SetXTitle(xtitle.c_str());
        h2d_ref->GetXaxis()->SetTitleOffset(1.5);
        h2d_ref->GetYaxis()->SetTitleOffset(1.5);
        // Normalise the histogram
        h2d_ref->GetXaxis()->SetRange(0, h2d_ref->GetNbinsX()+1);
        h2d_ref->GetYaxis()->SetRange(0, h2d_ref->GetNbinsY()+1);
        h2d_ref->Scale(1/h2d_ref->Integral("width"));
        // Hide the under/overflow again
        h2d_ref->GetXaxis()->SetRange(1, h2d_ref->GetNbinsX());
        h2d_ref->GetYaxis()->SetRange(1, h2d_ref->GetNbinsY());
        
        // Make ratio of two histograms
        TH2F *h2d_ratio = (TH2F *) h2d_ref->Clone();
        h2d_ratio->Divide(h2d_par);
        // Fix the colormap (Z) axis
        h2d_ratio->GetZaxis()->SetRangeUser(zmin, zmax);
        
        h2d_ratio->Draw("colz");
        
        TLine *line = new TLine(0.91, 0, 5, 0);
        line->SetLineWidth(2);
        line->Draw();
        text->Draw();
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

    TPaveText *text = new TPaveText(0.3, 0.04, 0.38, 0.14, "ndc");
    text->AddText(title.c_str());
    if (dynKt) {
        text->AddText("dynKt only");
    } else {
        text->AddText("no dynKt");
    }
    text->SetTextSize(20);
    text->Draw();

    TPaveText *level_ref = new TPaveText(0.01, 0.14, 0.03, 0.5, "ndc");
    TText *l1 = level_ref->AddText("hadron / parton ratio");
    l1->SetTextAngle(90);
    level_ref->SetTextSize(20);
    level_ref->Draw();

    savename += ".png";

    c->Draw();
    
    c->Print(savename.c_str());
    
    if (c) { 
        c->Close(); 
        gSystem->ProcessEvents(); 
        delete c; 
        c = 0; 
    }
    
    
    //c->Show();
}
