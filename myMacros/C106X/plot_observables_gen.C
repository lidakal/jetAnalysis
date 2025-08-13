#include "tTreeGen.h"
#include "binning.h"
#include "hist_utils.h"

void plot_observables_gen(TString sample="inclusive", TString label="FSRup")
{
    TString fin_name = "/data_CMS/cms/kalipoliti/qcdMC/pythia_var/"+sample+"/"+label+"/merged_HiForestMiniAOD.root";
    std::cout << "fin: " << fin_name << std::endl;

    tTreeGen tree(fin_name);

    TH2D *h_rg_b = new TH2D("h_rg_b", "x=rg, z=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zg_b = new TH2D("h_zg_b", "x=zg, z=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zpt_b = new TH2D("h_zpt_b", "x=zpt, z=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);

    TH2D *h_rg_all = new TH2D("h_rg_all", "x=rg, z=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zg_all = new TH2D("h_zg_all", "x=zg, z=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zpt_all = new TH2D("h_zpt_all", "x=zpt, z=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);

    TH2D *h_rg_b_noUO = new TH2D("h_rg_b_noUO", "x=rg, z=jtpt", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zg_b_noUO = new TH2D("h_zg_b_noUO", "x=zg, z=jtpt", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zpt_b_noUO = new TH2D("h_zpt_b_noUO", "x=zpt, z=jtpt", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

    TH2D *h_rg_all_noUO = new TH2D("h_rg_all_noUO", "x=rg, z=jtpt", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zg_all_noUO = new TH2D("h_zg_all_noUO", "x=zg, z=jtpt", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zpt_all_noUO = new TH2D("h_zpt_all_noUO", "x=zpt, z=jtpt", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

    double kt_cut = 1.;
    double logrg_underflow = -1.1; // SD-untagged
    double zg_underflow = 0.; // SD-untagged
    double zpt_underflow = -0.05; // jtptCh==0
    double jetR = 0.4;

    Long64_t nentries = tree.GetEntries();
    for (Long64_t ient=0; ient<nentries; ient++) {
        tree.GetEntry(ient);

        Float_t weight = tree.weight;

        for (int ijet=0; ijet<tree.ngen; ijet++) {
            Float_t geneta = tree.geneta[ijet];
            if (std::abs(geneta)>2) continue;

            Float_t genpt = tree.genpt[ijet];

            // Initialize observables
            Float_t rg = -1.;
            Float_t kt = -1.;
            Float_t zg = -1.;

            Float_t logrg = -999.;
            Float_t logkt = -999.;

            // Calculate rg, kt only for 2 prong jets
            if (tree.gsjt2Pt[ijet] > 0.) {
                rg = calc_rg(tree.gsjt1Y[ijet], tree.gsjt1Phi[ijet], tree.gsjt2Y[ijet], tree.gsjt2Phi[ijet]);
                kt = tree.gsjt2Pt[ijet] * rg;
                zg = tree.gsjt2Pt[ijet] / (tree.gsjt1Pt[ijet] + tree.gsjt2Pt[ijet]);
                
                // calculate logs
                logrg = std::log(jetR/rg);
                logkt = std::log(kt);
            }

            // Checks for zpt
            Float_t genBpt = tree.genBpt[ijet];
            Float_t genptCh = tree.genptCh[ijet];
            Float_t zpt = -1.;
            if (genptCh>1e-4) zpt = genBpt / genptCh;
            else zpt = zpt_underflow; // underflow bin range
            if (zpt>=1) zpt = 0.999; // zpt=1. included in last bin

            // Check for logrg 
            if (logrg<-900.) logrg=logrg_underflow; // underflow
            if (logrg>=2.5) logrg = 2.499; // overflow

            // Check for zg
            if (zg>=0.5) zg = 0.499; // zg=1. included in last bin

            // kt cut 
            if (kt < kt_cut) {
                logrg = logrg_underflow;
                zg = zg_underflow;
            }

            h_rg_all->Fill(logrg, genpt, weight);
            h_zg_all->Fill(zg, genpt, weight);
            h_zpt_all->Fill(zpt, genpt, weight);

            h_rg_all_noUO->Fill(logrg, genpt, weight);
            h_zg_all_noUO->Fill(zg, genpt, weight);
            h_zpt_all_noUO->Fill(zpt, genpt, weight);

            if (tree.genNbHad[ijet]==1) {
                h_rg_b->Fill(logrg, genpt, weight);
                h_zg_b->Fill(zg, genpt, weight);
                h_zpt_b->Fill(zpt, genpt, weight);

                h_rg_b_noUO->Fill(logrg, genpt, weight);
                h_zg_b_noUO->Fill(zg, genpt, weight);
                h_zpt_b_noUO->Fill(zpt, genpt, weight);
            }             
        } // jet loop
    }

    TString fout_name = "histos/"+sample+"_"+label+"_gen_histograms.root";
    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {
        h_rg_b, h_zg_b, h_zpt_b,
        h_rg_all, h_zg_all, h_zpt_all,
        h_rg_b_noUO, h_zg_b_noUO, h_zpt_b_noUO,
        h_rg_all_noUO, h_zg_all_noUO, h_zpt_all_noUO,
    }) {
        h->Write();
    }

    fout->Close();
    delete fout;

    gApplication->Terminate();
}