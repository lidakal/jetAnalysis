#include <TH3D.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
#include <TAxis.h>

#include "hist_utils.h"
#include "myPalette.h"

void draw_dr()
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
    TH3D *hBtag_drkt_aggrGen = (TH3D *) fin_aggrGenNoReco->Get("hBtag_drkt_gen");
    TH3D *hBtag_drkt_noAggrReco = (TH3D *) fin_aggrGenNoReco->Get("hBtag_drkt");

    TString label_aggrTMVA = "aggrTMVA_withY";
    TString fname_aggrTMVA = indir + label_aggrTMVA + "_rgzgkt.root";
    TFile *fin_aggrTMVA = new TFile(fname_aggrTMVA);
    TH3D *hBtag_drkt_aggrTMVAReco = (TH3D *) fin_aggrTMVA->Get("hBtag_drkt");

    // ---- Prepare hStack and legend ----
    THStack *hStack_dr = new THStack();
    hStack_dr->SetTitle("; ln(R/#DeltaR); 1/N_{2-prong jets} dN/dln(R/#DeltaR)");

    TLegend *leg_dr = new TLegend(0.5, 0.7, 0.85, 0.9);
    leg_dr->SetFillStyle(0);
    leg_dr->SetBorderSize(1);
    leg_dr->SetMargin(0.15);

    // ---- Get the bins of the pt range ----

    TAxis *zaxis = (TAxis *) hBtag_drkt_aggrGen->GetZaxis();
    Int_t izmin = zaxis->FindBin(ptMin);
    Int_t izmax = zaxis->FindBin(ptMax) - 1;

    // ---- Get the bins of the ln(kt) range

    TAxis *yaxis = (TAxis *) hBtag_drkt_aggrGen->GetYaxis();
    Float_t lnktMin = 0.;
    // Int_t iymin = 0;
    Int_t iymax = -1;
    Int_t iymin = yaxis->FindBin(lnktMin);

    // ---- Make the projections ----

    // gen
    TH1D *hBtag_dr_aggrGen = (TH1D *) hBtag_drkt_aggrGen->ProjectionX("hBtag_dr_aggrGen", iymin, iymax, izmin, izmax);
    normalize_histo(hBtag_dr_aggrGen, 1, -1);
    format_histo(hBtag_dr_aggrGen, mykRed);
    leg_dr->AddEntry(hBtag_dr_aggrGen, "Particle level", "l");
    hStack_dr->Add(hBtag_dr_aggrGen);

    // reco noAggr
    TH1D *hBtag_dr_noAggrReco = (TH1D *) hBtag_drkt_noAggrReco->ProjectionX("hBtag_dr_noAggrReco", iymin, iymax, izmin, izmax);
    normalize_histo(hBtag_dr_noAggrReco, 1, -1);  
    format_histo(hBtag_dr_noAggrReco, mykPink);
    leg_dr->AddEntry(hBtag_dr_noAggrReco, "Detector level, non-aggregated", "l");
    hStack_dr->Add(hBtag_dr_noAggrReco);

    // reco aggrTMVA
    TH1D *hBtag_dr_aggrTMVAReco = (TH1D *) hBtag_drkt_aggrTMVAReco->ProjectionX("hBtag_dr_aggrTMVAReco", iymin, iymax, izmin, izmax);
    normalize_histo(hBtag_dr_aggrTMVAReco, 1, -1);   
    format_histo(hBtag_dr_aggrTMVAReco, mykBlue);
    leg_dr->AddEntry(hBtag_dr_aggrTMVAReco, "Detector level, aggregated with TMVA", "l");
    hStack_dr->Add(hBtag_dr_aggrTMVAReco);

    // ---- Draw the histos ----

    TCanvas *c = new TCanvas("c", "", 1000, 600);
    hStack_dr->Draw("histo nostack");
    leg_dr->Draw();
    c->Draw();

    TString cname = "./plots/combined_dr.png";
    c->Print(cname);
}