#include "TH3F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TPaveText.h"
#include <regex>

using namespace std;

void draw_decay_effect(bool chargedOnly = false)
{
    string decayed_fname = "~/rootFiles/pt2dscan_ref.root";
    string undecayed_fname = "~/rootFiles/pt2dscan_ref_undecayed.root";
    string charged_decayed_fname = "~/rootFiles/pt2dscan_ref_charged.root";
    string charged_undecayed_fname = "~/rootFiles/charged_partialB_ref.root";    

    TFile *fin_decayed;
    TFile *fin_undecayed;

    // Load ref 3D histograms -- X = rg, Y = kt, Z = pt 


    if (chargedOnly) {
        fin_decayed = new TFile(charged_undecayed_fname.c_str());
        fin_undecayed = new TFile(charged_undecayed_fname.c_str());
    } else {
        fin_decayed = new TFile(decayed_fname.c_str());
        fin_undecayed = new TFile(undecayed_fname.c_str());
    }

    TH3F *h3d_decayed = (TH3F *) fin_decayed->Get("hB_rgkt")->Clone();
    TH3F *h3d_decayed_dynKt = (TH3F *) fin_decayed->Get("hB_rgkt_dynKt")->Clone();

    TH3F *h3d_undecayed = (TH3F *) fin_undecayed->Get("hB_rgkt")->Clone();
    TH3F *h3d_undecayed_dynKt = (TH3F *) fin_undecayed->Get("hB_rgkt_dynKt")->Clone();

    // Create / Draw 2D histograms - Projections
    const int npt = 3;

    Float_t lowpt[2] = {50., 80.};
    Float_t midpt[2] = {80., 120.};
    Float_t highpt[2] = {200., 250.};
    
    Float_t *ptrange[npt] = {lowpt, midpt, highpt};
    
    TCanvas *c = new TCanvas("c", "c", 1800, 1000);
    c->Divide(npt, 2); // 2 for decayed, undecayed

    TCanvas *c_dynKt = new TCanvas("c_dynKt", "c_dynKt", 1800, 1000);
    c_dynKt->Divide(npt, 2); // 2 for decayed, undecayed with dynKt

    TCanvas *c_ratio = new TCanvas("c_ratio", "c_ratio", 1800, 1000);
    c_ratio->Divide(npt, 2); // 2 for decayed/undecayed, decayed/undecayed with dynKt

    for (int i = 0; i < npt; i++) {
        Float_t ptmin = ptrange[i][0];
        Float_t ptmax = ptrange[i][1];

        
    }
    
}