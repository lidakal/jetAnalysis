// Script to fill histograms from Lid's ntuples
// created by the pp HiForest 

#include "binning.h"

void plot_substructure_Lida()
{
    TFile *fin = new TFile("/data_CMS/cms/kalipoliti/compact_trees/dijet_PF40_aggrTMVA_inclusive_tree.root");
    TTree *t = (TTree *) fin->Get("tree_all");

    // Set branch addresses

    Double_t jtpt;
    Double_t logrg;
    Double_t zg;
    Double_t logkt;
    Double_t weight;

    t->SetBranchAddress("jtpt", &jtpt);
    t->SetBranchAddress("logrg", &logrg);
    t->SetBranchAddress("zg", &zg);
    t->SetBranchAddress("logkt", &logkt);
    t->SetBranchAddress("weight", &weight);

    // Make histograms 

    // ln(R/Rg) bins
    Int_t x1bins = 16;
    Float_t x1min = 0.;
    Float_t x1max = 4.;

    // ln(kT) bins
    Int_t x2bins = 24;
    Float_t x2min = -3.;
    Float_t x2max = 3.;

    // zg bins
    Int_t x3bins = 16;
    Float_t x3min = 0.1;
    Float_t x3max = 0.5;

    // jtpt bins
    Int_t zbins = 3;
    Float_t zmin = 80.;
    Float_t zmax = 140.;

    TH3D *h_rg_kt_jtpt = new TH3D("h_rg_kt_jtpt", "h_rg_kt_jtpt", x1bins, x1min, x1max, x2bins, x2min, x2max, zbins, zmin, zmax);
    TH3D *h_zg_kt_jtpt = new TH3D("h_zg_kt_jtpt", "h_zg_kt_jtpt", x3bins, x3min, x3max, x2bins, x2min, x2max, zbins, zmin, zmax);

    Long64_t nsingle = 0;
    Double_t nsingle_w = 0;
    for (Long64_t ient=0; ient<t->GetEntries(); ient++)
    {
        // if (ient > 10) break;
        t->GetEntry(ient);

        if (logkt<-900&&jtpt>80&&jtpt<140) {
            nsingle++;
            nsingle_w += weight;
        }

        if (logrg<-900) continue; // skip 1-prong jets
        if (logkt<-900) continue; // skip 1-prong jets
        if (zg<-900) continue; // skip 1-prong jets
        if (jtpt<80||jtpt>140) continue; // skip outside of pt range

        if (zg>=0.5) zg = 0.499; // to fit in the last bin

        h_rg_kt_jtpt->Fill(logrg, logkt, jtpt, weight);
        h_zg_kt_jtpt->Fill(zg, logkt, jtpt, weight);
    }

    std::cout << "nsingle = " << nsingle << std::endl;
    std::cout << "nsingle_w = " << nsingle_w << std::endl;

    TFile *fout = new TFile("histos/substructure_Lida.root", "RECREATE");
    h_rg_kt_jtpt->Write();
    h_zg_kt_jtpt->Write();
    fout->Close();
}
