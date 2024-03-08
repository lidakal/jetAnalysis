#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"

// #include "myPalette.h"
#include "hist_utils.h"
#include "draw_utils.h"

void draw_sv_tracks_out_of_jet() 
{
    // Setup 
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TString indir = "./histos/";
    // TString label = "HiForestMiniAOD_HighPU_10000events_conmatch_truth_ExtraTracks";
    // TString label = "aggrGenNoReco";
    // TString fin = indir + label + "_" + ptMin + "_pt_" + ptMax + "_dz.root";
    TString fin = indir + "sv_tracks_out_of_jet.root";

    TFile *finPtr = new TFile(fin);

    TH2F *h_pt = (TH2F *) finPtr->Get("h_pt");

    // Prepare stack histos + legends
    THStack *hStack_pt = new THStack("hStack_pt", "");
    hStack_pt->SetTitle("; p_{T}^{trk}; number of tracks");

    TLegend *leg = new TLegend(0.7, 0.7, 0.85, 0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(1);
    leg->SetMargin(0.15);

    // Plot by class
    draw_histos_by_class(h_pt, hStack_pt, false, leg);

    // Draw histos 
    TString odir = "./plots/";

    TCanvas *c_pt_bjet = new TCanvas("c_pt_bjet", "", 800, 600);
    hStack_pt->Draw("hist");
    leg->Draw();
    drawHeaderSimulation();

    // draw_canvas(hStack_dz_bjet, leg, ptMin, ptMax, label, "dz_bjet", odir, c_dz_bjet);
}