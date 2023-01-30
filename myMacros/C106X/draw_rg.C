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
    TString suffix_bdt = "_bdt";
    TString fname_bdt = "./histos/rgzgkt" + suffix_bdt + ".root";
    TFile *fin_bdt = new TFile(fname_bdt);
    TH3D *hB_rgkt_bdt = (TH3D *) fin_bdt->Get("hB_rgkt");

    TString fname_noAggr = "./histos/rgzgkt_noAggr.root";
    TFile *fin_noAggr = new TFile(fname_noAggr);
    TH3D *hB_rgkt_noAggr = (TH3D *) fin_noAggr->Get("hB_rgkt");

    TString fname_truth = "./histos/rgzgkt_truth.root";
    TFile *fin_truth = new TFile(fname_truth);
    TH3D *hB_rgkt_truth = (TH3D *) fin_truth->Get("hB_rgkt");
    TH3D *hB_rgkt_gen_truth = (TH3D *) fin_truth->Get("hB_rgkt_gen");

    // ---- Prepare hStack and legend ----

    THStack *hStack_rg = new THStack();
    hStack_rg->SetTitle("; ln(1/R_{g}); 1/N_{2-prong jets} dN/dln(1/R_{g})");

    TLegend *leg_rg = new TLegend(0.65, 0.7, 0.85, 0.9);
    leg_rg->SetFillStyle(0);
    leg_rg->SetBorderSize(1);
    leg_rg->SetMargin(0.15);

    // ---- Get the bins of the pt range ----
    TAxis *zaxis = (TAxis *) hB_rgkt_truth->GetZaxis();
    Int_t izmin = zaxis->FindBin(ptMin);
    Int_t izmax = zaxis->FindBin(ptMax) - 1;

    Int_t iymin = 0;
    Int_t iymax = -1;

    // ---- Make the projections ----

    // gen
    TH1D *hB_rg_gen_truth = (TH1D *) hB_rgkt_gen_truth->ProjectionX("hB_rgkt_gen_truth", iymin, iymax, izmin, izmax);
    normalize_histo(hB_rg_gen_truth, 1, -1);
    format_histo(hB_rg_gen_truth, mykRed);
    leg_rg->AddEntry(hB_rg_gen_truth, "gen truthAggr", "l");
    hStack_rg->Add(hB_rg_gen_truth);

    // reco noAggr
    TH1D *hB_rg_noAggr = (TH1D *) hB_rgkt_noAggr->ProjectionX("hB_rg_noAggr", iymin, iymax, izmin, izmax);
    normalize_histo(hB_rg_noAggr, 1, -1);  
    format_histo(hB_rg_noAggr, mykPink);
    leg_rg->AddEntry(hB_rg_noAggr, "reco noAggr", "l");
    hStack_rg->Add(hB_rg_noAggr);

    // reco truth
    TH1D *hB_rg_truth = (TH1D *) hB_rgkt_truth->ProjectionX("hB_rg_truth", iymin, iymax, izmin, izmax);
    normalize_histo(hB_rg_truth, 1, -1); 
    format_histo(hB_rg_truth, mykBlue);
    leg_rg->AddEntry(hB_rg_truth, "reco truthAggr", "l");
    hStack_rg->Add(hB_rg_truth);

    // reco bdt
    TH1D *hB_rg_bdt = (TH1D *) hB_rgkt_bdt->ProjectionX("hB_rg_bdt", iymin, iymax, izmin, izmax);
    normalize_histo(hB_rg_bdt, 1, -1);   
    format_histo(hB_rg_bdt, mykGreen);
    leg_rg->AddEntry(hB_rg_bdt, "reco bdtAggr", "l");
    hStack_rg->Add(hB_rg_bdt);

    // ---- Draw the histos ----

    TCanvas *c = new TCanvas("c", "", 1000, 600);
    hStack_rg->Draw("histo nostack");
    leg_rg->Draw();
    c->Draw();

    TString cname = "./plots/combined_rg" + suffix_bdt + ".png";
    c->Print(cname);
}