// Script to fill histograms from Jelena's ntuples
// created by the PbPb HiForest 

#include "binning.h"

void plot_substructure_Jelena()
{
    TFile *fin = new TFile("/data_CMS/cms/kalipoliti/fromJelena/dijet_ppref_TMVAaggregation_ak4_SD01_kt0_merged_HiForestMiniAOD.root");
    TTree *t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hiEvtAnalyzer/HiTree");

    // Set branch addresses

    Int_t nref;
    Float_t jtpt[100];
    Float_t jt_rg_SD[100];
    Float_t jt_z_SD[100];
    Float_t jt_ktg_SD[100];
    Float_t weight;

    t->SetBranchAddress("nref", &nref);
    t->SetBranchAddress("jtpt", jtpt);
    t->SetBranchAddress("jt_rg_SD", jt_rg_SD);
    t->SetBranchAddress("jt_z_SD", jt_z_SD);
    t->SetBranchAddress("jt_ktg_SD", jt_ktg_SD);
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
        for (Int_t ijet=0; ijet<nref; ijet++)
        {
            Float_t rg = jt_rg_SD[ijet];
            Float_t kt = jt_ktg_SD[ijet];
            Float_t zg = jt_z_SD[ijet];
            Float_t pt = jtpt[ijet];

            if (kt<=0&&pt>80&&pt<140) {
                nsingle++;
                nsingle_w += weight;
            }

            if (rg<=0) continue; // skip 1-prong jets
            if (kt<=0) continue; // skip 1-prong jets
            if (zg<=0) continue; // skip 1-prong jets
            if (pt<80||pt>140) continue; // skip outside of pt range

            Float_t logrg = std::log(0.4 / jt_rg_SD[ijet]);
            Float_t logkt = std::log(jt_ktg_SD[ijet]);

            if (zg>=0.5) zg = 0.499; // to fit in the last bin

            h_rg_kt_jtpt->Fill(logrg, logkt, pt, weight);
            h_zg_kt_jtpt->Fill(zg, logkt, pt, weight);
        }
    }

    std::cout << "nsingle = " << nsingle << std::endl;
    std::cout << "nsingle_w = " << nsingle_w << std::endl;

    TFile *fout = new TFile("histos/substructure_Jelena_1.root", "RECREATE");
    h_rg_kt_jtpt->Write();
    h_zg_kt_jtpt->Write();
    fout->Close();
}
