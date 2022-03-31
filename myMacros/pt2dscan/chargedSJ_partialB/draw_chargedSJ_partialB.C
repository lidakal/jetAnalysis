#include "TH3F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include <regex>

using namespace std;

void draw_chargedSJ_partialB(char qtype = 'B')
{      
    string histfile_ref = "~/rootFiles/pt2dscan_ref.root";
    string histfile_par = "~/rootFiles/pt2dscan_par.root";

    string xtitle = "ln(1/R_{g})";
    string ytitle = "ln(kt/GeV)";

    // Load ref 3D histogram -- X = rg, Y = zg / kt, Z = pt 
    TFile *fin_ref = new TFile(histfile_ref.c_str());
    TH3F *h3d_ref = (TH3F *) fin_ref->Get("hB_rgkt")->Clone();
    TH3F *h3d_ref_dynKt = (TH3F *) fin_ref->Get("hB_rgkt_dynKt")->Clone();

    // Load par 3D histogram -- X = rg, Y = zg / kt, Z = pt 
    TFile *fin_par = new TFile(histfile_par.c_str());
    TH3F *h3d_par = (TH3F *) fin_par->Get("hB_rgkt")->Clone();

    // Create / Draw 2D histograms - Projections
    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {80., 120.};
    Float_t highpt[2] = {200., 250.};
    
    const int npt = 3;
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};
    
    TCanvas *c = new TCanvas("c", "c", 1800, 1000);
    c->Divide(npt, 2); // 2 for parton, hadron

    TCanvas *c_dynKt = new TCanvas("c_dynKt", "c_dynKt", 1800, 1000);
    c_dynKt->Divide(npt, 2); // 2 for parton, hadron dynKt

    TCanvas *c_ratio = new TCanvas("c_ratio", "c_ratio", 1800, 1000);
    c_ratio->Divide(npt, 2); // 2 for hadron/parton, hadron dynKt/parton

    for (int i = 0; i < npt, i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        TH3F *h3d_ref_clone = (TH3F *) h3d_ref->Clone();
        TH3F *h3d_ref_dynKt_clone = (TH3F *) h3d_ref_dynKt->Clone();
        TH3F *h3d_par_clone = (TH3F *) h3d_par->Clone();

        h3d_ref_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_ref_dynKt_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);
        h3d_par_clone->GetZaxis()->SetRangeUser(ptmin, ptmax);

        TH2F *h2d_ref = (TH2F *) h3d_ref_clone->Project3D(Form("yx%d_ref", i)); 
        TH2F *h2d_ref_dynKt = (TH2F *) h3d_ref_dynKt_clone->Project3D(Form("yx%d_ref", i)); 
        TH2F *h2d_par = (TH2F *) h3d_par_clone->Project3D(Form("yx%d_par", i));

        TLine *line = new TLine(0.91, 0, 5, 0);
        line->SetLineWidth(2);

        TLatex *txt = new TLatex();
        txt->SetTextSize(15);

        // c: par, ref
        Float_t zmin = 0.;
        Float_t zmax = 0.4;

        c->cd(i + 1);
        h2d_par->SetYTitle(ytitle.c_str());
        h2d_par->SetXTitle(xtitle.c_str());
        h2d_par->GetXaxis()->SetTitleOffset(2.5);
        h2d_par->GetYaxis()->SetTitleOffset(2.5);
        // Normalise the histogram
        h2d_par->GetXaxis()->SetRange(0, h2d_par->GetNbinsX()+1);
        h2d_par->GetYaxis()->SetRange(0, h2d_par->GetNbinsY()+1);
        h2d_par->Scale(1/h2d_par->Integral("width"));
        // Hide the under/overflow again
        h2d_par->GetXaxis()->SetRange(1, h2d_par->GetNbinsX());
        h2d_par->GetYaxis()->SetRange(1, h2d_par->GetNbinsY());
        // Fix colormap (Z) range
        h2d_par->GetZaxis()->SetRangeUser(zmin, zmax);

        h2d_par->Draw("colz");
        line->Draw();
        txt->DrawLatexNDC(0.7, 0.7, Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));

        c->cd(i + 1 + npt);
        h2d_ref->SetYTitle(ytitle.c_str());
        h2d_ref->SetXTitle(xtitle.c_str());
        h2d_ref->GetXaxis()->SetTitleOffset(2.5);
        h2d_ref->GetYaxis()->SetTitleOffset(2.5);
        // Normalise the histogram
        h2d_ref->GetXaxis()->SetRange(0, h2d_ref->GetNbinsX()+1);
        h2d_ref->GetYaxis()->SetRange(0, h2d_ref->GetNbinsY()+1);
        h2d_ref->Scale(1/h2d_ref->Integral("width"));
        // Hide the under/overflow again
        h2d_ref->GetXaxis()->SetRange(1, h2d_ref->GetNbinsX());
        h2d_ref->GetYaxis()->SetRange(1, h2d_ref->GetNbinsY());
        // Fix colormap (Z) range
        h2d_ref->GetZaxis()->SetRangeUser(zmin, zmax);
        
        h2d_ref->Draw("colz");
        line->Draw();
        txt->DrawLatexNDC(0.7, 0.7, Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));

        // c_dynKt: par, ref dynKt
        Float_t zmin_dynKt = 0.;
        Float_t zmax_dynKt = 0.4;

        c_dynKt->cd(i + 1);
        h2d_par->GetZaxis()->SetRangeUser(zmin_dynKt, zmax_dynKt);

        h2d_par->Draw("colz");
        line->Draw();
        txt->DrawLatexNDC(0.7, 0.7, Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));

        c_dynKt->cd(i + 1 + npt);
        h2d_ref_dynKt->GetZaxis()->SetRangeUser(zmin_dynKt, zmax_dynKt);
        
        h2d_ref_dynKt->Draw("colz");
        line->Draw();
        txt->DrawLatexNDC(0.7, 0.7, Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));

        // c_ratio: ref/par, ref dynKt/par
        Float_t zmin_ratio = 0.;
        Float_t zmax_ratio = 0.4;

        c_ratio->cd(i + 1);
        c_ratio->cd(i + 1)->SetLogz();
        TH2F *h2d_ratio = (TH2F *) h2d_ref->Clone();
        h2d_ratio->Divide(h2d_par);
        // Fix the colormap (Z) axis
        h2d_ratio->GetZaxis()->SetRangeUser(zmin_ratio, zmax_ratio);

        h2d_ratio->Draw("colz");
        line->Draw();
        txt->DrawLatexNDC(0.7, 0.7, Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));

        c_ratio->cd(i + 1 + npt);
        c_ratio->cd(i + 1 + npt)->SetLogz();
        TH2F *h2d_ratio_dynKt = (TH2F *) h2d_ref_dynKt->Clone();
        h2d_ratio_dynKt->Divide(h2d_par);
        // Fix the colormap (Z) axis
        h2d_ratio_dynKt->GetZaxis()->SetRangeUser(zmin_ratio, zmax_ratio);

        h2d_ratio_dynKt->Draw("colz");
        line->Draw();
        txt->DrawLatexNDC(0.7, 0.7, Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
    }

    c->Draw();
    c_dynKt->Draw();
    c_ratio->Draw();
}