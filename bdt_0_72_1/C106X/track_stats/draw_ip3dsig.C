#include "TFile.h"
#include "TString.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TStyle.h"

#include "../myPalette.h"
#include "hist_utils.h"

void draw_ip3dsig() 
{
    // Setup 
    gStyle->SetLegendTextSize(15);

    // float ptMin = 50.;
    // float ptMax = 80.;

    float ptMin = 30.;
    float ptMax = 700.;

    TString indir = "./histos/";
    TString label = "HiForestMiniAOD_HighPU_50000events_conmatch_newVars_truth";
    // TString label = "HiForestMiniAOD_LowPU_10000events_conmatch_newVars_truth";
    TString fin = indir + label + "_" + ptMin + "_pt_" + ptMax + "_ip3dsig.root";

    TFile *finPtr = new TFile(fin);

    TH2F *h_ip3dsig = (TH2F *) finPtr->Get("h_ip3dsig");
    TH2F *h_ip3dsig_bjet = (TH2F *) finPtr->Get("h_ip3dsig_bjet");

    TH2F *h_ip3d = (TH2F *) finPtr->Get("h_ip3d");
    TH2F *h_ip3d_bjet = (TH2F *) finPtr->Get("h_ip3d_bjet");

    TH2F *h_ip2dsig = (TH2F *) finPtr->Get("h_ip2dsig");
    TH2F *h_ip2dsig_bjet = (TH2F *) finPtr->Get("h_ip2dsig_bjet");

    TH2F *h_ip2d = (TH2F *) finPtr->Get("h_ip2d");
    TH2F *h_ip2d_bjet = (TH2F *) finPtr->Get("h_ip2d_bjet");

    // Prepare stack histos + legends
    THStack *hStack_ip3dsig = new THStack("hStack_ip3dsig", "");
    hStack_ip3dsig->SetTitle("; 3D IP / #sigma; 1 / N_{tracks in category} dN / d(3D IP / #sigma)");

    THStack *hStack_ip3dsig_bjet = new THStack("hStack_ip3dsig_bjet", "");
    hStack_ip3dsig_bjet->SetTitle("; 3D IP / #sigma; 1 / N_{tracks in category} dN / d(3D IP / #sigma)");

    THStack *hStack_ip3d_bjet = new THStack("hStack_ip3d_bjet", "");
    hStack_ip3d_bjet->SetTitle("; 3D IP; 1 / N_{tracks in category} dN / d(3D IP)");

    THStack *hStack_ip2dsig_bjet = new THStack("hStack_ip2dsig_bjet", "");
    hStack_ip2dsig_bjet->SetTitle("; 3D IP / #sigma; 1 / N_{tracks in category} dN / d(3D IP / #sigma)");

    THStack *hStack_ip2d_bjet = new THStack("hStack_ip2d_bjet", "");
    hStack_ip2d_bjet->SetTitle("; 3D IP; 1 / N_{tracks in category} dN / d(3D IP)");

    TLegend *leg = new TLegend(0.7, 0.7, 0.85, 0.9);
    leg->SetFillStyle(0);
    leg->SetBorderSize(1);
    leg->SetMargin(0.15);

    // Plot by class
    draw_histos_by_class(h_ip3dsig, hStack_ip3dsig, false, leg);
    draw_histos_by_class(h_ip3dsig_bjet, hStack_ip3dsig_bjet, false);
    draw_histos_by_class(h_ip3d_bjet, hStack_ip3d_bjet, false);
    draw_histos_by_class(h_ip2dsig_bjet, hStack_ip2dsig_bjet, false);
    draw_histos_by_class(h_ip2d_bjet, hStack_ip2d_bjet, false);

    // Draw histos 
    TString odir = "./plots/";

    TCanvas *c_ip3dsig_bjet = new TCanvas("c_ip3dsig_bjet", "", 1200, 800);
    draw_canvas(hStack_ip3dsig_bjet, leg, ptMin, ptMax, label, "ip3dsig_bjet", odir, c_ip3dsig_bjet);

    TCanvas *c_ip3d_bjet = new TCanvas("c_ip3d_bjet", "", 1200, 800);
    draw_canvas(hStack_ip3d_bjet, leg, ptMin, ptMax, label, "ip3d_bjet", odir, c_ip3d_bjet);

}