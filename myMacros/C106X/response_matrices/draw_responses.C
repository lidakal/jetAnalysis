#include <TH3D.h>
#include <TCanvas.h>
#include <THStack.h>
#include <TLegend.h>
#include <TAxis.h>

#include "draw_utils.h"

void draw_responses()
{
    // Setup 
    gStyle->SetLegendTextSize(15);

    double ptMin = 50.;
    double ptMax = 80.;
    TString suffix = Form("_%.0f_pt_%.0f", ptMin, ptMax);

    // double ptMin = 30.;
    // double ptMax = 700.;

    // ---- Grab histos ----
    TString indir = "./histos_ttbar_highPU/";

    TString label_aggrTMVA = "aggrTMVA";
    TString fname_aggrTMVA = indir + label_aggrTMVA + suffix + "_responses.root";
    TFile *fin_aggrTMVA = new TFile(fname_aggrTMVA);
    TH2F *h_response_rg_bjet_aggrTMVA = (TH2F *) fin_aggrTMVA->Get("h_response_rg_bjet");

    TString label_aggrGenNoReco = "aggrGenNoReco";
    TString fname_aggrGenNoReco = indir + label_aggrGenNoReco + suffix + "_responses.root";
    TFile *fin_aggrGenNoReco = new TFile(fname_aggrGenNoReco);
    TH2F *h_response_rg_bjet_aggrGenNoReco = (TH2F *) fin_aggrGenNoReco->Get("h_response_rg_bjet");

    // ---- Format the histos ----
    format_response(h_response_rg_bjet_aggrTMVA);
    format_response(h_response_rg_bjet_aggrGenNoReco);

    // ---- Draw the histos ----
    TString odir = "./plots_ttbar_highPU/";
    gStyle->SetPalette(57);

    TCanvas *c_response_rg_bjet_aggrTMVA = new TCanvas("c_response_rg_bjet_aggrTMVA", "", 1000, 1000);
    h_response_rg_bjet_aggrTMVA->Draw("colz");
    c_response_rg_bjet_aggrTMVA->Draw();
    c_response_rg_bjet_aggrTMVA->SetLogz();
    SetRealAspectRatio(c_response_rg_bjet_aggrTMVA);

    TString cname_aggrTMVA = odir + label_aggrTMVA + suffix + "response_rg.png";
    c_response_rg_bjet_aggrTMVA->Print(cname_aggrTMVA);

    TCanvas *c_response_rg_bjet_aggrGenNoReco = new TCanvas("c_response_rg_bjet_aggrGenNoReco", "", 1000, 1000);
    h_response_rg_bjet_aggrGenNoReco->Draw("colz");
    c_response_rg_bjet_aggrGenNoReco->Draw();
    c_response_rg_bjet_aggrGenNoReco->SetLogz();
    SetRealAspectRatio(c_response_rg_bjet_aggrGenNoReco);

    TString cname_aggrGenNoReco = odir + label_aggrGenNoReco + suffix + "response_rg.png";
    c_response_rg_bjet_aggrGenNoReco->Print(cname_aggrGenNoReco);
}