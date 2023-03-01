#include <TH3D.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
#include <TAxis.h>

#include "hist_utils.h"
#include "myPalette.h"

void draw_rg()
{
    // Setup 
    gStyle->SetLegendTextSize(15);

    double ptMin = 50.;
    double ptMax = 80.;

    // ---- Grab histos ----
    TString indir = "./histos/";
    TString label_aggrGenNoReco = "aggrGenNoReco_withY";
    TString fname_aggrGenNoReco = indir + label_aggrGenNoReco + "_rgzgkt.root";
    TFile *fin_aggrGenNoReco = new TFile(fname_aggrGenNoReco);
    TH3D *hBtag_rgkt_aggrGen = (TH3D *) fin_aggrGenNoReco->Get("hBtag_rgkt_gen");
    TH3D *hBtag_rgkt_noAggrReco = (TH3D *) fin_aggrGenNoReco->Get("hBtag_rgkt");

    TString label_aggrTMVA = "aggrTMVA_withY";
    TString fname_aggrTMVA = indir + label_aggrTMVA + "_rgzgkt.root";
    TFile *fin_aggrTMVA = new TFile(fname_aggrTMVA);
    TH3D *hBtag_rgkt_aggrTMVAReco = (TH3D *) fin_aggrTMVA->Get("hBtag_rgkt");

    // ---- Prepare hStack and legend ----
    THStack *hStack_rg = new THStack();
    hStack_rg->SetTitle("; ln(R/R_{g}); 1/N_{2-prong jets} dN/dln(R/R_{g})");

    TLegend *leg_rg = new TLegend(0.5, 0.7, 0.85, 0.9);
    leg_rg->SetFillStyle(0);
    leg_rg->SetBorderSize(1);
    leg_rg->SetMargin(0.15);

    // ---- Get the bins of the pt range ----

    TAxis *zaxis = (TAxis *) hBtag_rgkt_aggrGen->GetZaxis();
    Int_t izmin = zaxis->FindBin(ptMin);
    Int_t izmax = zaxis->FindBin(ptMax) - 1;

    // ---- Get the bins of the ln(kt) range

    TAxis *yaxis = (TAxis *) hBtag_rgkt_aggrGen->GetYaxis();
    Float_t lnktMin = 0.;
    // Int_t iymin = 0;
    Int_t iymax = -1;
    Int_t iymin = yaxis->FindBin(lnktMin);

    // ---- Make the projections ----

    // gen
    TH1D *hBtag_rg_aggrGen = (TH1D *) hBtag_rgkt_aggrGen->ProjectionX("hBtag_rg_aggrGen", iymin, iymax, izmin, izmax);
    normalize_histo(hBtag_rg_aggrGen, 1, -1);
    format_histo(hBtag_rg_aggrGen, mykRed);
    leg_rg->AddEntry(hBtag_rg_aggrGen, "Particle level", "l");
    hStack_rg->Add(hBtag_rg_aggrGen);

    // reco noAggr
    TH1D *hBtag_rg_noAggrReco = (TH1D *) hBtag_rgkt_noAggrReco->ProjectionX("hBtag_rg_noAggrReco", iymin, iymax, izmin, izmax);
    normalize_histo(hBtag_rg_noAggrReco, 1, -1);  
    format_histo(hBtag_rg_noAggrReco, mykPink);
    leg_rg->AddEntry(hBtag_rg_noAggrReco, "Detector level, non-aggregated", "l");
    hStack_rg->Add(hBtag_rg_noAggrReco);

    // reco aggrTMVA
    TH1D *hBtag_rg_aggrTMVAReco = (TH1D *) hBtag_rgkt_aggrTMVAReco->ProjectionX("hBtag_rg_aggrTMVAReco", iymin, iymax, izmin, izmax);
    normalize_histo(hBtag_rg_aggrTMVAReco, 1, -1);   
    format_histo(hBtag_rg_aggrTMVAReco, mykBlue);
    leg_rg->AddEntry(hBtag_rg_aggrTMVAReco, "Detector level, aggregated with TMVA", "l");
    hStack_rg->Add(hBtag_rg_aggrTMVAReco);

    // ---- Draw the histos ----

    TCanvas *c = new TCanvas("c", "", 1000, 600);
    hStack_rg->Draw("histo nostack");
    leg_rg->Draw();
    c->Draw();

    TString cname = "./plots/combined_rg.png";
    c->Print(cname);
}