#include <TH3D.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
#include <TAxis.h>

#include "hist_utils.h"
// #include "myPalette.h"

void draw_zg()
{
    // ---- Setup ---- 
    double ptMin = 100.;
    double ptMax = 120.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos ----
    TString sample = "bjet";
    TString label = "lateKt";
    TString indir = "./histos/qcd_" + sample + "_";
    TString label_aggrGenNoReco = "aggrGenNoReco_lateKt";
    TString fname_aggrGenNoReco = indir + label_aggrGenNoReco + "_rgzgkt.root";
    TFile *fin_aggrGenNoReco = new TFile(fname_aggrGenNoReco);
    TH3D *hBtag_zgkt_aggrGen = (TH3D *) fin_aggrGenNoReco->Get("hBtag_zgkt_gen");
    TH3D *hBtag_zgkt_noAggrReco = (TH3D *) fin_aggrGenNoReco->Get("hBtag_zgkt");

    TString label_aggrTMVA = "aggrTMVA_lateKt";
    TString fname_aggrTMVA = indir + label_aggrTMVA + "_rgzgkt.root";
    TFile *fin_aggrTMVA = new TFile(fname_aggrTMVA);
    TH3D *hBtag_zgkt_aggrTMVAReco = (TH3D *) fin_aggrTMVA->Get("hBtag_zgkt");

    // ---- Prepare hStack and legend ----
    THStack *hStack_zg = new THStack();
    hStack_zg->SetTitle("; z_{g}=p_{T,2}/(p_{T,1}+p_{T,2}); 1/N_{2-prong jets} dN/dz_{g}");

    TLegend *leg_zg = new TLegend(0.4, 0.7, 0.8, 0.85);
    leg_zg->SetFillStyle(0);
    leg_zg->SetBorderSize(0);
    leg_zg->SetMargin(0.15);
    leg_zg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), b tagged jets", ptMin, ptMax));

    // ---- Get the bins of the pt range ----

    TAxis *zaxis = (TAxis *) hBtag_zgkt_aggrGen->GetZaxis();
    Int_t izmin = zaxis->FindBin(ptMin);
    Int_t izmax = zaxis->FindBin(ptMax) - 1;

    // ---- Get the bins of the ln(kt) range

    TAxis *yaxis = (TAxis *) hBtag_zgkt_aggrGen->GetYaxis();
    // Float_t lnktMin = 0.;
    Int_t iymin = 0;
    Int_t iymax = -1;
    // Int_t iymin = yaxis->FindBin(lnktMin);

    // ---- Make the projections ----

    // gen
    TH1D *hBtag_zg_aggrGen = (TH1D *) hBtag_zgkt_aggrGen->ProjectionX("hBtag_zg_aggrGen", iymin, iymax, izmin, izmax);
    normalize_histo(hBtag_zg_aggrGen, 1, -1);
    format_histo(hBtag_zg_aggrGen, mykRed);
    leg_zg->AddEntry(hBtag_zg_aggrGen, "Particle level", "l");
    hStack_zg->Add(hBtag_zg_aggrGen);

    // reco noAggr
    TH1D *hBtag_zg_noAggrReco = (TH1D *) hBtag_zgkt_noAggrReco->ProjectionX("hBtag_zg_noAggrReco", iymin, iymax, izmin, izmax);
    normalize_histo(hBtag_zg_noAggrReco, 1, -1);  
    format_histo(hBtag_zg_noAggrReco, mykPink);
    leg_zg->AddEntry(hBtag_zg_noAggrReco, "Detector level, non-aggregated", "l");
    hStack_zg->Add(hBtag_zg_noAggrReco);

    // reco aggrTMVA
    TH1D *hBtag_zg_aggrTMVAReco = (TH1D *) hBtag_zgkt_aggrTMVAReco->ProjectionX("hBtag_zg_aggrTMVAReco", iymin, iymax, izmin, izmax);
    normalize_histo(hBtag_zg_aggrTMVAReco, 1, -1);   
    format_histo(hBtag_zg_aggrTMVAReco, mykBlue);
    leg_zg->AddEntry(hBtag_zg_aggrTMVAReco, "Detector level, aggregated with TMVA", "l");
    hStack_zg->Add(hBtag_zg_aggrTMVAReco);

    // ---- Draw the histos ----

    TCanvas *c = new TCanvas("c", "", 1200, 1000);
    hStack_zg->Draw("histo nostack");
    hStack_zg->SetMaximum(6);
    leg_zg->Draw();
    c->Draw();

    TString cname = "./plots/qcd_" + sample + "_" + label + "_zg.png";
    c->Print(cname);
}