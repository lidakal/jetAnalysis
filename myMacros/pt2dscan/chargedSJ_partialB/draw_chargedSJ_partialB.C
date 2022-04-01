#include "TH3F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLine.h"
#include <regex>

using namespace std;

void draw_chargedSJ_partialB()
{      
    string histfile_ref = "charged_partialB_ref.root";
    string histfile_par = "charged_partialB_par.root";

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

    for (int i = 0; i < npt; i++) {
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

        TPaveText *info = new TPaveText(0.45, 0.7, 0.8, 0.85, "ndc");
        info->AddText("chargedSJ partialB b-jets");
        info->AddLine(0., 0.7, 1., 0.7);
        info->AddText(Form("%.0f < p_{T} < %.0f (GeV)", ptmin, ptmax));
        info->SetFillColor(0);
        info->SetBorderSize(1);
        info->SetTextSize(15);
        
        TPaveText *info_par = (TPaveText *) info->Clone();
        info_par->AddText("parton level");
        
        TPaveText *info_ref = (TPaveText *) info->Clone();
        info_ref->AddText("hadron level");
        
        TPaveText *info_ref_dynKt = (TPaveText *) info->Clone();
        info_ref_dynKt->AddText("hadron level, dynKt");
        
        TPaveText *info_ratio = (TPaveText *) info->Clone();
        info_ratio->AddText("hadron / parton ratio");
        
        TPaveText *info_ratio_dynKt = (TPaveText *) info->Clone();
        info_ratio_dynKt->AddText("hadron / parton ratio, dynKt");

        // c : par, ref
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
        info_par->Draw();

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
        info_ref->Draw();

        // c_dynKt : par, ref dynKt
        Float_t zmin_dynKt = 0.;
        Float_t zmax_dynKt = 0.3;

        c_dynKt->cd(i + 1);
        TH2F *h2d_par_clone = (TH2F *) h2d_par->Clone();
        h2d_par_clone->GetZaxis()->SetRangeUser(zmin_dynKt, zmax_dynKt);

        h2d_par_clone->Draw("colz");
        line->Draw();
        info_par->Draw();

        c_dynKt->cd(i + 1 + npt);
        h2d_ref_dynKt->SetYTitle(ytitle.c_str());
        h2d_ref_dynKt->SetXTitle(xtitle.c_str());
        h2d_ref_dynKt->GetXaxis()->SetTitleOffset(2.5);
        h2d_ref_dynKt->GetYaxis()->SetTitleOffset(2.5);
        // Normalise the histogram
        h2d_ref_dynKt->GetXaxis()->SetRange(0, h2d_ref_dynKt->GetNbinsX()+1);
        h2d_ref_dynKt->GetYaxis()->SetRange(0, h2d_ref_dynKt->GetNbinsY()+1);
        h2d_ref_dynKt->Scale(1/h2d_ref_dynKt->Integral("width"));
        // Hide the under/overflow again
        h2d_ref_dynKt->GetXaxis()->SetRange(1, h2d_ref_dynKt->GetNbinsX());
        h2d_ref_dynKt->GetYaxis()->SetRange(1, h2d_ref_dynKt->GetNbinsY());
        // Fix colormap (Z) range
        h2d_ref_dynKt->GetZaxis()->SetRangeUser(zmin_dynKt, zmax_dynKt);
        
        h2d_ref_dynKt->Draw("colz");
        line->Draw();
        info_ref_dynKt->Draw();

        // c_ratio : ref/par, ref dynKt/par
        Float_t zmin_ratio = 0.;
        Float_t zmax_ratio = 50.;

        c_ratio->cd(i + 1);
        c_ratio->cd(i + 1)->SetLogz();
        TH2F *h2d_ratio = (TH2F *) h2d_ref->Clone();
        h2d_ratio->Divide(h2d_par);
        // Fix the colormap (Z) axis
        h2d_ratio->GetZaxis()->SetRangeUser(zmin_ratio, zmax_ratio);

        h2d_ratio->Draw("colz");
        line->Draw();
        info_ratio->Draw();

        c_ratio->cd(i + 1 + npt);
        c_ratio->cd(i + 1 + npt)->SetLogz();
        TH2F *h2d_ratio_dynKt = (TH2F *) h2d_ref_dynKt->Clone();
        h2d_ratio_dynKt->Divide(h2d_par);
        // Fix the colormap (Z) axis
        h2d_ratio_dynKt->GetZaxis()->SetRangeUser(zmin_ratio, zmax_ratio);

        h2d_ratio_dynKt->Draw("colz");
        line->Draw();
        info_ratio_dynKt->Draw();
    }

    c->Draw();
    c_dynKt->Draw();
    c_ratio->Draw();
    
    string savename_c = "chargedSJ_partialB_bjets_par_vs_had";
    string savename_c_dynKt = savename_c + "_dynKt.png";
    string savename_c_ratio = savename_c + "_ratio.png";
    savename_c += ".png";
    
    c->Print(savename_c.c_str());
    c_dynKt->Print(savename_c_dynKt.c_str());
    //c_ratio->Print(savename_c_ratio.c_str());
}