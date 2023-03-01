#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"

#include "../myPalette.h"
#include "hist_utils.h"

void draw_dz() 
{
    // Setup 
    gStyle->SetLegendTextSize(15);

    // float ptMin = 50.;
    // float ptMax = 80.;

    float ptMin = 30.;
    float ptMax = 700.;

    TString indir = "./histos/";
    // TString label = "HiForestMiniAOD_HighPU_50000events_conmatch_newVars_truth";
    TString label = "HiForestMiniAOD_LowPU_10000events_conmatch_newVars_truth";
    TString fin = indir + label + "_" + ptMin + "_pt_" + ptMax + "_dz.root";

    TFile *finPtr = new TFile(fin);

    TH2F *h_dz = (TH2F *) finPtr->Get("h_dz");
    TH2F *h_dz_bjet = (TH2F *) finPtr->Get("h_dz_bjet");

    // Prepare stack histos + legends
    THStack *hStack_dz = new THStack("hStack_dz", "");
    hStack_dz->SetTitle("; #Deltaz_{trk-LV}; 1 / N_{tracks in category} dN / d(#Deltaz_{trk-LV})");

    THStack *hStack_dz_bjet = new THStack("hStack_dz_bjet", "");
    hStack_dz_bjet->SetTitle("; #Deltaz_{trk-LV}; 1 / N_{tracks in category} dN / d(#Deltaz_{trk-LV})");

    TLegend *leg = new TLegend(0.7, 0.7, 0.85, 0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(1);
    leg->SetMargin(0.15);

    // Plot by class
    draw_histos_by_class(h_dz, hStack_dz, false, leg);
    draw_histos_by_class(h_dz_bjet, hStack_dz_bjet, false);

    // Draw histos 
    TString odir = "./plots/";

    TCanvas *c_dz_bjet = new TCanvas("c_dz_bjet", "", 1200, 800);
    draw_canvas(hStack_dz_bjet, leg, ptMin, ptMax, label, "dz_bjet", odir, c_dz_bjet);
}