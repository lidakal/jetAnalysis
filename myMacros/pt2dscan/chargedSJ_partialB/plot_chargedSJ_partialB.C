#include "TreeAnalyzer.h"
#include "TH3.h"
#include <iostream>

using namespace std;

void plot_chargedSJ_partialB(bool parORref)
{
    TreeAnalyzer TAb(true, true);
    TreeAnalyzer TAqcd(false, true);
    
    // Create output file name
    string foutname = "charged_partialB";
    // Set analysis level
    if (parORref) {
        TAb.SetAnalysisLevelParton();
        TAqcd.SetAnalysisLevelParton();
        foutname += "_par.root";
    } else {
        TAb.SetAnalysisLevelTruth();
        TAqcd.SetAnalysisLevelTruth();
        foutname += "_ref.root";
    }

    // ln(1/rg)
    Int_t x1bins = 40;
    Float_t x1min = 0.91;
    Float_t x1max = 5.;

    // ln(kt)
    Int_t y1bins = 40;
    Float_t y1min = -5.;
    Float_t y1max = 5.;

    // jetpt
    Int_t z1bins = 27*2;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    TH3D *hB_rgkt = new TH3D("hB_rgkt", "rg, kt, pt, bjets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hC_rgkt = new TH3D("hC_rgkt", "rg, kt, pt, cjets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hL_rgkt = new TH3D("hL_rgkt", "rg, kt, pt, ljets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hB_rgkt_dynKt = new TH3D("hB_rgkt_dynKt", "rg, kt, pt, bjets, dynKt only", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hC_rgkt_dynKt = new TH3D("hC_rgkt_dynKt", "rg, kt, pt, cjets, dynKt only", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hL_rgkt_dynKt = new TH3D("hL_rgkt_dynKt", "rg, kt, pt, ljets, dynKt only", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    cout << "Creating B histograms..." << endl;

    for (Long64_t i = 0; i < TAb.nentries; i++) {
        TAb.t->GetEntry(i);

        // Print progress
        if (i%1000000 == 0) {
            cout << "i = " << i << endl;
        }

        for (Int_t j = 0; j < TAb.njet; j++) {
            if (abs(TAb.jeteta[j]) > 2.) {
                continue;
            }

            Float_t rg = -1.;
            Float_t kt = -1.;

            Float_t logrg = -1;
            Float_t logkt = -10;

            // Calculate rg, kt only for 2 prong jets
            if (TAb.subjet2pt[j] > 0.) {
                Float_t deta = TAb.subjet1eta[j] - TAb.subjet2eta[j];
                Float_t dphi = acos(cos(TAb.subjet1phi[j] - TAb.subjet2phi[j]));
                rg = sqrt(deta * deta + dphi * dphi);
                
                kt = TAb.subjet2pt[j] * rg;
                
                // calculate logs
                logrg = log(1/rg);
                logkt = log(kt);
            }

            // Fill the histograms
            if (TAb.jetNb[j] > 0) {
                hB_rgkt->Fill(logrg, logkt, TAb.jetpt[j], TAb.weight);
            }

            // Fill dynKt histos with -1 when parORref = true or when !refIsHardest
            if (parORref || (!TAb.refIsHardest[j])) { 
                logrg = -1;
                logkt = -10;
            }

            if (TAb.jetNb[j] > 0) {
                hB_rgkt_dynKt->Fill(logrg, logkt, TAb.jetpt[j], TAb.weight);
            }
        }
    }

    cout << "Creating C, L histograms..." << endl;

    for (Long64_t i = 0; i < TAqcd.nentries; i++) {
        TAqcd.t->GetEntry(i);

        // Print progress
        if (i%1000000 == 0) {
            cout << "i = " << i << endl;
        }

        for (Int_t j = 0; j < TAqcd.njet; j++) {
            if (abs(TAqcd.jeteta[j]) > 2.) {
                continue;
            }

            Float_t rg = -1.;
            Float_t kt = -1.;

            Float_t logrg = -1;
            Float_t logkt = -10;

            // Calculate rg, kt only for 2 prong jets
            if (TAqcd.subjet2pt[j] > 0.) {
                Float_t deta = TAqcd.subjet1eta[j] - TAqcd.subjet2eta[j];
                Float_t dphi = acos(cos(TAqcd.subjet1phi[j] - TAqcd.subjet2phi[j]));
                rg = sqrt(deta * deta + dphi * dphi);
                
                kt = TAqcd.subjet2pt[j] * rg;
                
                // calculate logs
                logrg = log(1/rg);
                logkt = log(kt);
            }

            // Fill the histograms
            if ((TAqcd.jetNc[j] > 0) && (!(TAqcd.jetNb[j] > 0))) {
                hC_rgkt->Fill(logrg, logkt, TAqcd.jetpt[j], TAqcd.weight);
            } else { 
                hL_rgkt->Fill(logrg, logkt, TAqcd.jetpt[j], TAqcd.weight);
            }

            // Fill dynKt histos with -1 when parORref = true or when !refIsHardest
            if (parORref || (!TAqcd.refIsHardest[j])) { 
                logrg = -1;
                logkt = -10;
            }

            if ((TAqcd.jetNc[j] > 0) && (!(TAqcd.jetNb[j] > 0))) {
                hC_rgkt_dynKt->Fill(logrg, logkt, TAqcd.jetpt[j], TAqcd.weight);
            } else { 
                hL_rgkt_dynKt->Fill(logrg, logkt, TAqcd.jetpt[j], TAqcd.weight);
            }
        }
    }

    cout << "\n(Re)creating file " << foutname << endl;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    cout << "Saving histograms..." << endl;
    hB_rgkt->Write();
    hC_rgkt->Write();
    hL_rgkt->Write();

    hB_rgkt_dynKt->Write();
    hC_rgkt_dynKt->Write();
    hL_rgkt_dynKt->Write();

    fout->Close();
}