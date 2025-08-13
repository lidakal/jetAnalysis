#include "tTree.h"
#include "binning.h"
#include "hist_utils.h"

void plot_observables_gen_nominal(TString sample="bjet", TString label="aggrTMVA_fixedGenBug")
{
    TString fin_name = "/data_CMS/cms/kalipoliti/qcdMC/"+sample+"/"+label+"/merged_HiForestMiniAOD.root";
    std::cout << "fin: " << fin_name << std::endl;

    std::vector<TString> activeBranches_t = {"nref", "nvtx",
                                             "refeta", "refpt", "jtHadFlav", "jtNbHad", "jtNcHad",
                                             "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", "rsjt1Y",
                                             "rsjt2Pt", "rsjt2Eta", "rsjt2Phi", "rsjt2Y",
                                             "refmB", "refBpt", "refptCh",
                                             "jtpt",
                                             "sjt1Pt", "sjt1Eta", "sjt1Phi", "sjt1Y",
                                             "sjt2Pt", "sjt2Eta", "sjt2Phi", "sjt2Y",
                                             "jtmB", "jtBpt", "jtptCh",
                                             "weight"
                                             };

    tTree tree(fin_name);

    tree.SetBranchStatus("*", 0);
    tree.SetBranchStatus(activeBranches_t, 1);


    TH2D *h_rg_b = new TH2D("h_rg_b", "x=rg, z=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zg_b = new TH2D("h_zg_b", "x=zg, z=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zpt_b = new TH2D("h_zpt_b", "x=zpt, z=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);

    TH2D *h_rg_all = new TH2D("h_rg_all", "x=rg, z=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zg_all = new TH2D("h_zg_all", "x=zg, z=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zpt_all = new TH2D("h_zpt_all", "x=zpt, z=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);

    TH2D *h_rg_b_reco = new TH2D("h_rg_b_reco", "x=rg, z=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zg_b_reco = new TH2D("h_zg_b_reco", "x=zg, z=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zpt_b_reco = new TH2D("h_zpt_b_reco", "x=zpt, z=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);

    TH2D *h_rg_all_reco = new TH2D("h_rg_all_reco", "x=rg, z=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zg_all_reco = new TH2D("h_zg_all_reco", "x=zg, z=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    TH2D *h_zpt_all_reco = new TH2D("h_zpt_all_reco", "x=zpt, z=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);

    TH2D *h_residuals_rg_all = new TH2D("h_residuals_rg_all", "x=(rgReco-rgPart)/rgPart, y=ptPart", 50, -1, 1, 3, 80, 140);
    TH2D *h_residuals_zg_all = new TH2D("h_residuals_zg_all", "x=(zgReco-zgPart)/zgPart, y=ptPart", 50, -1, 1, 3, 80, 140);
    TH2D *h_residuals_jtpt_all = new TH2D("h_residuals_jtpt_all", "x=(ptReco-ptPart)/ptPart, y=ptPart", 50, -1, 1, 3, 80, 140);

    double kt_cut = 1.;
    double logrg_underflow = -1.1; // SD-untagged
    double zg_underflow = 0.; // SD-untagged
    double zpt_underflow = -0.05; // jtptCh==0
    double jetR = 0.4;

    Long64_t nentries = tree.GetEntries();
    for (Long64_t ient=0; ient<nentries; ient++) {
        tree.GetEntry(ient);

        if (ient%1000000==0) std::cout << "ient=" << ient << std::endl;

        if (ient>5000000) break;

        Float_t weight = tree.weight;

        for (int ijet=0; ijet<tree.nref; ijet++) {
            Float_t refeta = tree.refeta[ijet];
            if (std::abs(refeta)>2) continue;

            Float_t refpt = tree.refpt[ijet];
            Float_t jtpt = tree.jtpt[ijet];

            // Initialize observables
            Float_t rg = -999.;
            Float_t kt = -999.;
            Float_t zg = -999.;

            Float_t logrg = -999.;
            Float_t logkt = -999.;

            Float_t rg_reco = -999.;
            Float_t kt_reco = -999.;
            Float_t zg_reco = -999.;

            Float_t logrg_reco = -999.;
            Float_t logkt_reco = -999.;

            // Calculate rg, kt only for 2 prong jets
            if (tree.rsjt2Pt[ijet] > 0.) {
                rg = calc_rg(tree.rsjt1Y[ijet], tree.rsjt1Phi[ijet], tree.rsjt2Y[ijet], tree.rsjt2Phi[ijet]);
                kt = tree.rsjt2Pt[ijet] * rg;
                zg = tree.rsjt2Pt[ijet] / (tree.rsjt1Pt[ijet] + tree.rsjt2Pt[ijet]);
                
                // calculate logs
                logrg = std::log(jetR/rg);
                logkt = std::log(kt);
            } 

            if (tree.sjt2Pt[ijet] > 0.) {
                rg_reco = calc_rg(tree.sjt1Y[ijet], tree.sjt1Phi[ijet], tree.sjt2Y[ijet], tree.sjt2Phi[ijet]);
                kt_reco = tree.sjt2Pt[ijet] * rg_reco;
                zg_reco = tree.sjt2Pt[ijet] / (tree.sjt1Pt[ijet] + tree.sjt2Pt[ijet]);
                
                // calculate logs
                logrg_reco = std::log(jetR/rg_reco);
                logkt_reco = std::log(kt_reco);
            } 

            // Checks for zpt
            Float_t refBpt = tree.refBpt[ijet];
            Float_t refptCh = tree.refptCh[ijet];
            Float_t zpt = -1.;
            if (refptCh>1e-4) zpt = refBpt / refptCh;
            else zpt = zpt_underflow; // underflow bin range
            if (zpt>=1) zpt = 0.999; // zpt=1. included in last bin

            Float_t jtBpt = tree.jtBpt[ijet];
            Float_t jtptCh = tree.jtptCh[ijet];
            Float_t zpt_reco = -1.;
            if (jtptCh>1e-4) zpt_reco = jtBpt / jtptCh;
            else zpt_reco = zpt_underflow; // underflow bin range
            if (zpt_reco>=1) zpt_reco = 0.999; // zpt=1. included in last bin

            // Check for logrg 
            if (logrg<-900.) logrg=logrg_underflow; // underflow
            if (logrg>=2.5) logrg = 2.499; // overflow
            if (logrg_reco<-900.) logrg_reco=logrg_underflow; // underflow
            if (logrg_reco>=2.5) logrg_reco = 2.499; // overflow

            // Check for zg
            if (zg>=0.5) zg = 0.499; // zg=1. included in last bin
            if (zg_reco>=0.5) zg_reco = 0.499; // zg=1. included in last bin

            // kt cut 
            if (kt < kt_cut) {
                logrg = logrg_underflow;
                zg = zg_underflow;

                logrg_reco = logrg_underflow;
                zg_reco = zg_underflow;
            }

            h_rg_all->Fill(logrg, refpt, weight);
            h_zg_all->Fill(zg, refpt, weight);
            h_zpt_all->Fill(zpt, refpt, weight);

            h_rg_all_reco->Fill(logrg_reco, jtpt, weight);
            h_zg_all_reco->Fill(zg_reco, jtpt, weight);
            h_zpt_all_reco->Fill(zpt_reco, jtpt, weight);

            if (tree.jtNbHad[ijet]==1) {
                h_rg_b->Fill(logrg, refpt, weight);
                h_zg_b->Fill(zg, refpt, weight);
                h_zpt_b->Fill(zpt, refpt, weight);

                h_rg_b_reco->Fill(logrg_reco, jtpt, weight);
                h_zg_b_reco->Fill(zg_reco, jtpt, weight);
                h_zpt_b_reco->Fill(zpt_reco, jtpt, weight);
            }       

            if (logrg>-1&&logrg_reco>-1) h_residuals_rg_all->Fill((logrg_reco-logrg)/logrg, refpt, weight);
            if (zg>0.05&&zg_reco>0.05) h_residuals_zg_all->Fill((zg_reco-zg)/zg, refpt, weight);
            h_residuals_jtpt_all->Fill((jtpt-refpt)/refpt, refpt, weight);     
        } // jet loop
    }

    TString fout_name = "histos/"+sample+"_"+label+"_gen_histograms_5Mevents.root";
    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {
        h_rg_b, h_zg_b, h_zpt_b,
        h_rg_all, h_zg_all, h_zpt_all,
        h_rg_b_reco, h_zg_b_reco, h_zpt_b_reco,
        h_rg_all_reco, h_zg_all_reco, h_zpt_all_reco,
    }) {
        h->Write();
    }

    h_residuals_rg_all->Write();
    h_residuals_zg_all->Write();
    h_residuals_jtpt_all->Write();

    fout->Close();
    delete fout;

    gApplication->Terminate();
}