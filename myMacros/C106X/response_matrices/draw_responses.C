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

    TString label_aggrTMVA = "aggrTMVA_withY";
    TString fname_aggrTMVA = indir + label_aggrTMVA + suffix + "_responses.root";
    TFile *fin_aggrTMVA = new TFile(fname_aggrTMVA);
    TH2F *h_response_dr_bjet_aggrTMVA = (TH2F *) fin_aggrTMVA->Get("h_response_dr_bjet");
    TH2F *h_response_rg_bjet_aggrTMVA = (TH2F *) fin_aggrTMVA->Get("h_response_rg_bjet");
    TH2F *h_response_zg_bjet_aggrTMVA = (TH2F *) fin_aggrTMVA->Get("h_response_zg_bjet");
    TH2F *h_response_mb_bjet_aggrTMVA = (TH2F *) fin_aggrTMVA->Get("h_response_mb_bjet");

    TString label_aggrGenNoReco = "aggrGenNoReco_withY";
    TString fname_aggrGenNoReco = indir + label_aggrGenNoReco + suffix + "_responses.root";
    TFile *fin_aggrGenNoReco = new TFile(fname_aggrGenNoReco);
    TH2F *h_response_dr_bjet_aggrGenNoReco = (TH2F *) fin_aggrGenNoReco->Get("h_response_dr_bjet");
    TH2F *h_response_rg_bjet_aggrGenNoReco = (TH2F *) fin_aggrGenNoReco->Get("h_response_rg_bjet");
    TH2F *h_response_zg_bjet_aggrGenNoReco = (TH2F *) fin_aggrGenNoReco->Get("h_response_zg_bjet");

    // TString label_aggrCuts_ip3dsig_2p5 = "aggrCuts_ip3dsig_2p5";
    // TString fname_aggrCuts_ip3dsig_2p5 = indir + label_aggrCuts_ip3dsig_2p5 + suffix + "_responses.root";
    // TFile *fin_aggrCuts_ip3dsig_2p5 = new TFile(fname_aggrCuts_ip3dsig_2p5);
    // TH2F *h_response_rg_bjet_aggrCuts_ip3dsig_2p5 = (TH2F *) fin_aggrCuts_ip3dsig_2p5->Get("h_response_rg_bjet");
    // TH2F *h_response_zg_bjet_aggrCuts_ip3dsig_2p5 = (TH2F *) fin_aggrCuts_ip3dsig_2p5->Get("h_response_zg_bjet");
    // TH2F *h_response_mb_bjet_aggrCuts_ip3dsig_2p5 = (TH2F *) fin_aggrCuts_ip3dsig_2p5->Get("h_response_mb_bjet");

    // ---- Format the histos ----
    // dr
    format_response(h_response_dr_bjet_aggrTMVA, "ln(R/#DeltaR)");
    format_response(h_response_dr_bjet_aggrGenNoReco, "ln(R/#DeltaR)");

    // rg
    format_response(h_response_rg_bjet_aggrTMVA);
    format_response(h_response_rg_bjet_aggrGenNoReco);
    // format_response(h_response_rg_bjet_aggrCuts_ip3dsig_2p5);

    // zg
    format_response(h_response_zg_bjet_aggrTMVA, "z_{g}");
    format_response(h_response_zg_bjet_aggrGenNoReco, "z_{g}");
    // format_response(h_response_zg_bjet_aggrCuts_ip3dsig_2p5, "z_{g}");

    // mb
    format_response(h_response_mb_bjet_aggrTMVA, "m_{pseudo-B}");
    // format_response(h_response_mb_bjet_aggrCuts_ip3dsig_2p5, "m_{pseudo-B}");

    // ---- Draw the histos ----
    TString odir = "./plots_ttbar_highPU/";
    gStyle->SetPalette(57);

    // dr
    TCanvas *c_response_dr_bjet_aggrTMVA = new TCanvas("c_response_dr_bjet_aggrTMVA", "", 1000, 1000);
    TString cname_dr_aggrTMVA = odir + label_aggrTMVA + suffix + "response_dr.png";
    draw_canvas_response(c_response_dr_bjet_aggrTMVA,h_response_dr_bjet_aggrTMVA, cname_dr_aggrTMVA);

    TCanvas *c_response_dr_bjet_aggrGenNoReco = new TCanvas("c_response_dr_bjet_aggrGenNoReco", "", 1000, 1000);
    TString cname_dr_aggrGenNoReco = odir + label_aggrGenNoReco + suffix + "response_dr.png";
    draw_canvas_response(c_response_dr_bjet_aggrGenNoReco, h_response_dr_bjet_aggrGenNoReco, cname_dr_aggrGenNoReco);

    // rg
    TCanvas *c_response_rg_bjet_aggrTMVA = new TCanvas("c_response_rg_bjet_aggrTMVA", "", 1000, 1000);
    TString cname_rg_aggrTMVA = odir + label_aggrTMVA + suffix + "response_rg.png";
    draw_canvas_response(c_response_rg_bjet_aggrTMVA,h_response_rg_bjet_aggrTMVA, cname_rg_aggrTMVA);

    TCanvas *c_response_rg_bjet_aggrGenNoReco = new TCanvas("c_response_rg_bjet_aggrGenNoReco", "", 1000, 1000);
    TString cname_rg_aggrGenNoReco = odir + label_aggrGenNoReco + suffix + "response_rg.png";
    draw_canvas_response(c_response_rg_bjet_aggrGenNoReco, h_response_rg_bjet_aggrGenNoReco, cname_rg_aggrGenNoReco);

    // TCanvas *c_response_rg_bjet_aggrCuts_ip3dsig_2p5 = new TCanvas("c_response_rg_bjet_aggrCuts_ip3dsig_2p5", "", 1000, 1000);
    // TString cname_rg_aggrCuts_ip3dsig_2p5 = odir + label_aggrCuts_ip3dsig_2p5 + suffix + "response_rg.png";
    // draw_canvas_response(c_response_rg_bjet_aggrCuts_ip3dsig_2p5, h_response_rg_bjet_aggrCuts_ip3dsig_2p5, cname_rg_aggrCuts_ip3dsig_2p5);

    // zg
    TCanvas *c_response_zg_bjet_aggrTMVA = new TCanvas("c_response_zg_bjet_aggrTMVA", "", 1000, 1000);
    TString cname_zg_aggrTMVA = odir + label_aggrTMVA + suffix + "response_zg.png";
    draw_canvas_response(c_response_zg_bjet_aggrTMVA, h_response_zg_bjet_aggrTMVA, cname_zg_aggrTMVA);

    TCanvas *c_response_zg_bjet_aggrGenNoReco = new TCanvas("c_response_zg_bjet_aggrGenNoReco", "", 1000, 1000);
    TString cname_zg_aggrGenNoReco = odir + label_aggrGenNoReco + suffix + "response_zg.png";
    draw_canvas_response(c_response_zg_bjet_aggrGenNoReco, h_response_zg_bjet_aggrGenNoReco, cname_zg_aggrGenNoReco);

    // TCanvas *c_response_zg_bjet_aggrCuts_ip3dsig_2p5 = new TCanvas("c_response_zg_bjet_aggrCuts_ip3dsig_2p5", "", 1000, 1000);
    // TString cname_zg_aggrCuts_ip3dsig_2p5 = odir + label_aggrCuts_ip3dsig_2p5 + suffix + "response_zg.png";
    // draw_canvas_response(c_response_zg_bjet_aggrCuts_ip3dsig_2p5, h_response_zg_bjet_aggrCuts_ip3dsig_2p5, cname_zg_aggrCuts_ip3dsig_2p5);

    // mb
    TCanvas *c_response_mb_bjet_aggrTMVA = new TCanvas("c_response_mb_bjet_aggrTMVA", "", 1000, 1000);
    TString cname_mb_aggrTMVA = odir + label_aggrTMVA + suffix + "response_mb.png";
    draw_canvas_response(c_response_mb_bjet_aggrTMVA, h_response_mb_bjet_aggrTMVA, cname_mb_aggrTMVA);

    // TCanvas *c_response_mb_bjet_aggrCuts_ip3dsig_2p5 = new TCanvas("c_response_mb_bjet_aggrCuts_ip3dsig_2p5", "", 1000, 1000);
    // TString cname_mb_aggrCuts_ip3dsig_2p5 = odir + label_aggrCuts_ip3dsig_2p5 + suffix + "response_mb.png";
    // draw_canvas_response(c_response_mb_bjet_aggrCuts_ip3dsig_2p5, h_response_mb_bjet_aggrCuts_ip3dsig_2p5, cname_mb_aggrCuts_ip3dsig_2p5);
}