#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"

#include "../myPalette.h"
#include "hist_utils.h"

void draw_zdr() 
{
    // Setup 
    gStyle->SetLegendTextSize(15);

    // float ptMin = 50.;
    // float ptMax = 80.;

    float ptMin = 30.;
    float ptMax = 700.;

    TString indir = "./histos/";
    TString label = "HiForestMiniAOD_HighPU_100000events_conmatch";
    TString fin = indir + label + "_" + ptMin + "_pt_" + ptMax + "_zdr.root";

    TFile *finPtr = new TFile(fin);

    TH2F *hTrkDistToAxis = (TH2F *) finPtr->Get("hTrkDistToAxis");
    TH2F *hTrkDistToAxis_bjet = (TH2F *) finPtr->Get("hTrkDistToAxis_bjet");
    TH2F *hDeltaR = (TH2F *) finPtr->Get("hDeltaR");
    TH2F *hDeltaR_bjet = (TH2F *) finPtr->Get("hDeltaR_bjet");
    TH2F *hRelpt = (TH2F *) finPtr->Get("hRelpt");
    TH2F *hRelpt_bjet = (TH2F *) finPtr->Get("hRelpt_bjet");
    TH2F *hzdr = (TH2F *) finPtr->Get("hzdr");
    TH2F *hzdr_bjet = (TH2F *) finPtr->Get("hzdr_bjet");

    // Prepare stack histos + legends
    THStack *hStack_trkDistToAxis = new THStack("hStack_trkDistToAxis", "");
    hStack_trkDistToAxis->SetTitle("; trkDistToJetAxis; 1 / N_{total tracks} dN / d(trkDistToJetAxis)");

    THStack *hStack_deltaR = new THStack("hStack_deltaR", "");
    hStack_deltaR->SetTitle("; #DeltaR_{jet-trk}/0.4; 1 / N_{total tracks} dN / d(#DeltaR_{jet-trk}/0.4)");

    THStack *hStack_relpt = new THStack("hStack_relpt", "");
    hStack_relpt->SetTitle("; p_{T}^{trk}/p_{T}^{jet}; 1 / N_{total tracks} dN / d(p_{T}^{trk}/p_{T}^{jet})");

    THStack *hStack_zdr = new THStack("hStack_zdr", "");
    hStack_zdr->SetTitle("; z#upoint#DeltaR/R; 1 / N_{total tracks} dN / d(z#upoint#DeltaR/R)");

    THStack *hStack_trkDistToAxis_bjet = new THStack("hStack_trkDistToAxis_bjet", "");
    hStack_trkDistToAxis_bjet->SetTitle("; trkDistToJetAxis; 1 / N_{total tracks in b-jets} dN / d(trkDistToJetAxis)");

    THStack *hStack_deltaR_bjet = new THStack("hStack_deltaR_bjet", "");
    hStack_deltaR_bjet->SetTitle("; #DeltaR_{jet-trk}/0.4; 1 / N_{total tracks in b-jets} dN / d(#DeltaR_{jet-trk}/0.4)");

    THStack *hStack_relpt_bjet = new THStack("hStack_relpt_bjet", "");
    hStack_relpt_bjet->SetTitle("; p_{T}^{trk}/p_{T}^{jet}; 1 / N_{total tracks in b-jets} dN / d(p_{T}^{trk}/p_{T}^{jet})");

    THStack *hStack_zdr_bjet = new THStack("hStack_zdr_bjet", "");
    hStack_zdr_bjet->SetTitle("; z#upoint#DeltaR/R; 1 / N_{total tracks in b-jets} dN / d(z#upoint#DeltaR/R)");

    TLegend *leg = new TLegend(0.7, 0.7, 0.85, 0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(1);
    leg->SetMargin(0.15);

    // Plot by class
    draw_histos_by_class(hTrkDistToAxis, hStack_trkDistToAxis, leg);
    draw_histos_by_class(hTrkDistToAxis_bjet, hStack_trkDistToAxis_bjet);

    draw_histos_by_class(hDeltaR, hStack_deltaR);
    draw_histos_by_class(hDeltaR_bjet, hStack_deltaR_bjet); 

    draw_histos_by_class(hRelpt, hStack_relpt);
    draw_histos_by_class(hRelpt_bjet, hStack_relpt_bjet);

    draw_histos_by_class(hzdr, hStack_zdr);
    draw_histos_by_class(hzdr_bjet, hStack_zdr_bjet);

    // Draw histos 
    TString odir = "./plots/";

    // TCanvas *c_trkDistToAxis = new TCanvas("c_trkDistToAxis", "", 1200, 800);
    // draw_canvas(hStack_trkDistToAxis, leg, ptMin, ptMax, label, "trkDistToAxis", odir, c_trkDistToAxis);

    // TCanvas *c_trkDistToAxis_bjet = new TCanvas("c_trkDistToAxis_bjet", "", 1200, 800);
    // draw_canvas(hStack_trkDistToAxis_bjet, leg, ptMin, ptMax, label, "trkDistToAxis_bjet", odir, c_trkDistToAxis_bjet);

    // TCanvas *c_deltaR = new TCanvas("c_deltaR", "", 1200, 800);
    // draw_canvas(hStack_deltaR, leg, ptMin, ptMax, label, "deltaR", odir, c_deltaR);

    // TCanvas *c_deltaR_bjet = new TCanvas("c_deltaR_bjet", "", 1200, 800);
    // draw_canvas(hStack_deltaR_bjet, leg, ptMin, ptMax, label, "deltaR_bjet", odir, c_deltaR_bjet);

    // TCanvas *c_relpt = new TCanvas("c_relpt", "", 1200, 800);
    // draw_canvas(hStack_relpt, leg, ptMin, ptMax, label, "relpt", odir, c_relpt);

    // TCanvas *c_relpt_bjet = new TCanvas("c_relpt_bjet", "", 1200, 800);
    // draw_canvas(hStack_relpt_bjet, leg, ptMin, ptMax, label, "relpt_bjet", odir, c_relpt_bjet);

    TCanvas *c_zdr = new TCanvas("c_zdr", "", 1200, 800);
    draw_canvas(hStack_zdr, leg, ptMin, ptMax, label, "zdr", odir, c_zdr);

    TCanvas *c_zdr_bjet = new TCanvas("c_zdr_bjet", "", 1200, 800);
    draw_canvas(hStack_zdr_bjet, leg, ptMin, ptMax, label, "zdr_bjet", odir, c_zdr_bjet);
}