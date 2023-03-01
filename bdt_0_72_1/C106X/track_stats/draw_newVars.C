#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"

#include "../myPalette.h"
#include "hist_utils.h"

void draw_newVars() 
{
    // Setup 
    gStyle->SetLegendTextSize(15);

    float ptMin = 50.;
    float ptMax = 80.;

    // float ptMin = 30.;
    // float ptMax = 700.;

    TString indir = "./histos/";
    TString label = "HiForestMiniAOD_HighPU_10000events_conmatch_newVars_truth";
    TString fin = indir + label + "_" + ptMin + "_pt_" + ptMax + "_newVars.root";

    TFile *finPtr = new TFile(fin);

    TH2F *h_svtxm = (TH2F *) finPtr->Get("h_svtxm");
    TH2F *h_svtxm_bjet = (TH2F *) finPtr->Get("h_svtxm_bjet");
    TH2F *h_svtxmcorr = (TH2F *) finPtr->Get("h_svtxmcorr");
    TH2F *h_svtxmcorr_bjet = (TH2F *) finPtr->Get("h_svtxmcorr_bjet");
    TH2F *h_svtxdls = (TH2F *) finPtr->Get("h_svtxdls");
    TH2F *h_svtxdls_bjet = (TH2F *) finPtr->Get("h_svtxdls_bjet");
    TH2F *h_svtxdls2d = (TH2F *) finPtr->Get("h_svtxdls2d");
    TH2F *h_svtxdls2d_bjet = (TH2F *) finPtr->Get("h_svtxdls2d_bjet");
    TH2F *h_trkIp2dSig = (TH2F *) finPtr->Get("h_trkIp2dSig");
    TH2F *h_trkIp2dSig_bjet = (TH2F *) finPtr->Get("h_trkIp2dSig_bjet");
    TH2F *h_nvtx = (TH2F *) finPtr->Get("h_nvtx");
    TH2F *h_nvtx_bjet = (TH2F *) finPtr->Get("h_nvtx_bjet");
    TH2F *h_ptFrac = (TH2F *) finPtr->Get("h_ptFrac");
    TH2F *h_ptFrac_bjet = (TH2F *) finPtr->Get("h_ptFrac_bjet");

    // Prepare stack histos + legends
    // THStack *hStack_svtxm = new THStack("hStack_svtxm", "");
    // hStack_svtxm->SetTitle("; M_{SV}; 1 / N_{total tracks} dN / d(M_{SV})");

    THStack *hStack_svtxm_bjet = new THStack("hStack_svtxm_bjet", "");
    hStack_svtxm_bjet->SetTitle("; M_{SV}; 1 / N_{total tracks in b-jets} dN / dM_{SV}");

    THStack *hStack_svtxmcorr_bjet = new THStack("hStack_svtxmcorr_bjet", "");
    hStack_svtxmcorr_bjet->SetTitle("; M_{corr, SV}; 1 / N_{total tracks in b-jets} dN / dM_{corr, SV}");

    THStack *hStack_svtxdls_bjet = new THStack("hStack_svtxdls_bjet", "");
    hStack_svtxdls_bjet->SetTitle("; svtxdls; 1 / N_{total tracks in b-jets} dN / d(svtxdls)");

    THStack *hStack_svtxdls2d_bjet = new THStack("hStack_svtxdls2d_bjet", "");
    hStack_svtxdls2d_bjet->SetTitle("; svtxdls2d; 1 / N_{total tracks in b-jets} dN / d(svtxdls2d)");

    THStack *hStack_trkIp2dSig_bjet = new THStack("hStack_trkIp2dSig_bjet", "");
    hStack_trkIp2dSig_bjet->SetTitle("; trkIp2dSig; 1 / N_{total tracks in b-jets} dN / d(trkIp2dSig)");

    THStack *hStack_nvtx_bjet = new THStack("hStack_nvtx_bjet", "");
    hStack_nvtx_bjet->SetTitle("; nvtx; 1 / N_{total tracks in b-jets} dN / d(nvtx)");

    THStack *hStack_ptFrac_bjet = new THStack("hStack_ptFrac_bjet", "");
    hStack_ptFrac_bjet->SetTitle("; p_{T}^{trk} / p_{T}^{SV}; 1 / N_{total tracks in b-jets} dN / d(p_{T}^{trk} / p_{T}^{SV})");

    TLegend *leg = new TLegend(0.7, 0.7, 0.85, 0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(1);
    leg->SetMargin(0.15);

    // Plot by class
    // draw_histos_by_class(h_svtxm, hStack_svtxm, leg);
    draw_histos_by_class(h_svtxm_bjet, hStack_svtxm_bjet, leg);

    // draw_histos_by_class(h_svtxmcorr, hStack_svtxmcorr);
    draw_histos_by_class(h_svtxmcorr_bjet, hStack_svtxmcorr_bjet);

    // draw_histos_by_class(h_svtxdls, hStack_svtxdls);
    draw_histos_by_class(h_svtxdls_bjet, hStack_svtxdls_bjet);

    // draw_histos_by_class(h_svtxdls2d, hStack_svtxdls2d);
    draw_histos_by_class(h_svtxdls2d_bjet, hStack_svtxdls2d_bjet);

    // draw_histos_by_class(h_trkIp2dSig, hStack_trkIp2dSig);
    draw_histos_by_class(h_trkIp2dSig_bjet, hStack_trkIp2dSig_bjet);

    // draw_histos_by_class(h_nvtx, hStack_nvtx);
    draw_histos_by_class(h_nvtx_bjet, hStack_nvtx_bjet);

    draw_histos_by_class(h_ptFrac_bjet, hStack_ptFrac_bjet);

    // Draw histos 
    TString odir = "./plots/";

    // TCanvas *c_svtxm = new TCanvas("c_svtxm", "", 1200, 800);
    // draw_canvas(hStack_svtxm, leg, ptMin, ptMax, label, "svtxm", odir, c_svtxm);

    // TCanvas *c_svtxm_bjet = new TCanvas("c_svtxm_bjet", "", 1200, 800);
    // draw_canvas(hStack_svtxm_bjet, leg, ptMin, ptMax, label, "svtxm_bjet", odir, c_svtxm_bjet);

    // TCanvas *c_svtxmcorr = new TCanvas("c_svtxmcorr", "", 1200, 800);
    // draw_canvas(hStack_svtxmcorr, leg, ptMin, ptMax, label, "svtxmcorr", odir, c_svtxmcorr);

    // TCanvas *c_svtxmcorr_bjet = new TCanvas("c_svtxmcorr_bjet", "", 1200, 800);
    // draw_canvas(hStack_svtxmcorr_bjet, leg, ptMin, ptMax, label, "svtxmcorr_bjet", odir, c_svtxmcorr_bjet);

    // TCanvas *c_svtxdls = new TCanvas("c_svtxdls", "", 1200, 800);
    // draw_canvas(hStack_svtxdls, leg, ptMin, ptMax, label, "svtxdls", odir, c_svtxdls);

    // TCanvas *c_svtxdls_bjet = new TCanvas("c_svtxdls_bjet", "", 1200, 800);
    // draw_canvas(hStack_svtxdls_bjet, leg, ptMin, ptMax, label, "svtxdls_bjet", odir, c_svtxdls_bjet);

    // TCanvas *c_svtxdls2d = new TCanvas("c_svtxdls2d", "", 1200, 800);
    // draw_canvas(hStack_svtxdls2d, leg, ptMin, ptMax, label, "svtxdls2d", odir, c_svtxdls2d);

    // TCanvas *c_svtxdls2d_bjet = new TCanvas("c_svtxdls2d_bjet", "", 1200, 800);
    // draw_canvas(hStack_svtxdls2d_bjet, leg, ptMin, ptMax, label, "svtxdls2d_bjet", odir, c_svtxdls2d_bjet);

    // TCanvas *c_trkIp2dSig = new TCanvas("c_trkIp2dSig", "", 1200, 800);
    // draw_canvas(hStack_trkIp2dSig, leg, ptMin, ptMax, label, "trkIp2dSig", odir, c_trkIp2dSig);

    // TCanvas *c_trkIp2dSig_bjet = new TCanvas("c_trkIp2dSig_bjet", "", 1200, 800);
    // draw_canvas(hStack_trkIp2dSig_bjet, leg, ptMin, ptMax, label, "trkIp2dSig_bjet", odir, c_trkIp2dSig_bjet);

    // TCanvas *c_nvtx = new TCanvas("c_nvtx", "", 1200, 800);
    // draw_canvas(hStack_nvtx, leg, ptMin, ptMax, label, "nvtx", odir, c_nvtx);

    // TCanvas *c_nvtx_bjet = new TCanvas("c_nvtx_bjet", "", 1200, 800);
    // draw_canvas(hStack_nvtx_bjet, leg, ptMin, ptMax, label, "nvtx_bjet", odir, c_nvtx_bjet);

    // TCanvas *c_ptFrac_bjet = new TCanvas("c_ptFrac_bjet", "", 1200, 800);
    // draw_canvas(hStack_ptFrac_bjet, leg, ptMin, ptMax, label, "ptFrac_bjet", odir, c_ptFrac_bjet);

    // TCanvas *c_ptFrac_bjet = new TCanvas("c_ptFrac_bjet", "", 1200, 800);
    // draw_canvas(hStack_ptFrac_bjet, leg, ptMin, ptMax, label, "ptFrac_bjet", odir, c_ptFrac_bjet);

}