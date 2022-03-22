#include "TH3F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include <regex>

using namespace std;

void draw_pt2dscan(char qtype = 'L', bool ktORzgrg = false, bool dynKt = false)
{       
    string histfile_ref = "~/rootFiles/pt2dscan_ref.root";
    string histfile_par = "~/rootFiles/pt2dscan_par.root";

    // Create names / titles / labels based on settings
    string hname = "h";
    string xtitle = "ln(1/R_{g})";
    string ytitle = "";
    string title = "";
    string savename = "~/gitRepos/jetAnalysis/myMacros/pt2dscan/";

    if (qtype == 'B'){
        title += "b-jets";
        if(ktORzgrg) {
            hname += "_bJet_ktB";
            savename += "pt2dscan_bjets_rgkt";
        } else {
            hname += "_bJet_rgzgB";
            savename += "pt2dscan_bjets_rgzg";
        }
    } else if (qtype == 'C'){
        title += "c-jets";
        if(ktORzgrg) {
            hname += "_qcd_ktC";
            savename += "pt2dscan_cjets_rgkt";
        } else {
            hname += "_qcd_rgzgC";
            savename += "pt2dscan_cjets_rgzg";
        }
    } else {
        title += "light jets";
        if(ktORzgrg) {
            hname += "_qcd_ktL";
            savename += "pt2dscan_ljets_rgkt";
        } else {
            hname += "_qcd_rgzgL";
            savename += "pt2dscan_ljets_rgzg";
        }
    }
    if (ktORzgrg) { 
        ytitle += "ln(k_{T}/GeV)";
    } else {
        ytitle += "z_{g}";
    }
    string hname_par = hname;
    if (dynKt) { 
        hname += "_dynKt";
        savename += "_dynKt";
    }

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
    
    TCanvas *c = new TCanvas("c", "c", 1800, 1000);
    c->Divide(3, 2);

    for (int i = 0; i < npt; i ++){
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];
        //cout << "pt range: " << ptmin << ", " << ptmax << endl;

        TH3F *h3d_ref_clone = (TH3F *) h3d_ref->Clone();
        TH3F *h3d_par_clone = (TH3F *) h3d_par->Clone();

        h3d_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_par_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);

        TH2F *h2d_ref = (TH2F *) h3d_ref_clone->Project3D(Form("yx%d_ref", i)); 
        TH2F *h2d_par = (TH2F *) h3d_par_clone->Project3D(Form("yx%d_par", i)); 

        c->cd(i + 1);
        h2d_par->SetYTitle(ytitle.c_str());
        h2d_par->SetXTitle(xtitle.c_str());
        h2d_par->GetXaxis()->SetTitleOffset(2.5);
        h2d_par->GetYaxis()->SetTitleOffset(2.5);
        h2d_par->Draw("colz");

        c->cd(i + 4);
        h2d_ref->SetYTitle(ytitle.c_str());
        h2d_ref->SetXTitle(xtitle.c_str());
        h2d_ref->GetXaxis()->SetTitleOffset(2.5);
        h2d_ref->GetYaxis()->SetTitleOffset(2.5);
        h2d_ref->Draw("colz");
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
    text->AddText(title.c_str());
    if (dynKt) {
        text->AddText("dynKt only");
    } else {
        text->AddText("no dynKt");
    }
    text->SetTextSize(30);
    text->Draw();

    TPaveText *level_ref = new TPaveText(0.01, 0.14, 0.04, 0.31, "ndc");
    TText *l1 = level_ref->AddText("hadron level");
    l1->SetTextAngle(90);
    level_ref->SetTextSize(30);
    level_ref->Draw();

    TPaveText *level_par = new TPaveText(0.01, 0.64, 0.04, 0.81, "ndc");
    TText *l2 = level_par->AddText("parton level");
    l2->SetTextAngle(90);
    level_par->SetTextSize(30);
    level_par->Draw();

    savename += ".png";

    c->Draw();
    c->Print(savename.c_str());

    c->Show();
}
