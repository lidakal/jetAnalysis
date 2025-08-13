#include "tTree.h"
#include "binning.h"
#include "hist_utils.h"

double get_weight(TH2D *h, double x, double y) {
    int ibin_x = h->GetXaxis()->FindBin(x);
    int ibin_y = h->GetYaxis()->FindBin(y);
    double weight = h->GetBinContent(ibin_x, ibin_y);

    if (ibin_x<1||ibin_x>h->GetNbinsX()) std::cout << "x = " << x << " out of bounds" << std::endl;
    if (ibin_y<1||ibin_y>h->GetNbinsY()) std::cout << "y = " << y << " out of bounds" << std::endl;

    return weight;
}

void plot_observables_gen_reweighted(TString sample="dijet", TString label="aggrTMVA_fixedGenBug")
{
    std::vector<TString> variations = {
        "FSRup", "FSRdown",
        "ISRup", "ISRdown",
        "Bothup", "Bothdown",
    };

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

    TH2D *h_FSRup_rg_b, 
         *h_FSRdown_rg_b, 
         *h_ISRup_rg_b, 
         *h_ISRdown_rg_b, 
         *h_Bothup_rg_b, 
         *h_Bothdown_rg_b;
    std::vector<TH2D *> histos_reweighted_rg_b = {
        h_FSRup_rg_b, 
        h_FSRdown_rg_b, 
        h_ISRup_rg_b, 
        h_ISRdown_rg_b, 
        h_Bothup_rg_b, 
        h_Bothdown_rg_b
    };

    TH2D *h_FSRup_zg_b, 
         *h_FSRdown_zg_b, 
         *h_ISRup_zg_b, 
         *h_ISRdown_zg_b, 
         *h_Bothup_zg_b, 
         *h_Bothdown_zg_b;
    std::vector<TH2D *> histos_reweighted_zg_b = {
        h_FSRup_zg_b, 
        h_FSRdown_zg_b, 
        h_ISRup_zg_b, 
        h_ISRdown_zg_b, 
        h_Bothup_zg_b, 
        h_Bothdown_zg_b
    };

    TH2D *h_FSRup_zpt_b, 
         *h_FSRdown_zpt_b, 
         *h_ISRup_zpt_b, 
         *h_ISRdown_zpt_b, 
         *h_Bothup_zpt_b, 
         *h_Bothdown_zpt_b;
    std::vector<TH2D *> histos_reweighted_zpt_b = {
        h_FSRup_zpt_b, 
        h_FSRdown_zpt_b, 
        h_ISRup_zpt_b, 
        h_ISRdown_zpt_b, 
        h_Bothup_zpt_b, 
        h_Bothdown_zpt_b
    };

    TH2D *h_FSRup_rg_all, 
         *h_FSRdown_rg_all, 
         *h_ISRup_rg_all, 
         *h_ISRdown_rg_all, 
         *h_Bothup_rg_all, 
         *h_Bothdown_rg_all;
    std::vector<TH2D *> histos_reweighted_rg_all = {
        h_FSRup_rg_all, 
        h_FSRdown_rg_all, 
        h_ISRup_rg_all, 
        h_ISRdown_rg_all, 
        h_Bothup_rg_all, 
        h_Bothdown_rg_all
    };

    TH2D *h_FSRup_zg_all, 
         *h_FSRdown_zg_all, 
         *h_ISRup_zg_all, 
         *h_ISRdown_zg_all, 
         *h_Bothup_zg_all, 
         *h_Bothdown_zg_all;
    std::vector<TH2D *> histos_reweighted_zg_all = {
        h_FSRup_zg_all, 
        h_FSRdown_zg_all, 
        h_ISRup_zg_all, 
        h_ISRdown_zg_all, 
        h_Bothup_zg_all, 
        h_Bothdown_zg_all
    };

    TH2D *h_FSRup_zpt_all, 
         *h_FSRdown_zpt_all, 
         *h_ISRup_zpt_all, 
         *h_ISRdown_zpt_all, 
         *h_Bothup_zpt_all, 
         *h_Bothdown_zpt_all;
    std::vector<TH2D *> histos_reweighted_zpt_all = {
        h_FSRup_zpt_all, 
        h_FSRdown_zpt_all, 
        h_ISRup_zpt_all, 
        h_ISRdown_zpt_all, 
        h_Bothup_zpt_all, 
        h_Bothdown_zpt_all
    };

    TH2D *h_FSRup_rg_b_reco, 
         *h_FSRdown_rg_b_reco, 
         *h_ISRup_rg_b_reco, 
         *h_ISRdown_rg_b_reco, 
         *h_Bothup_rg_b_reco, 
         *h_Bothdown_rg_b_reco;
    std::vector<TH2D *> histos_reweighted_rg_b_reco = {
        h_FSRup_rg_b_reco, 
        h_FSRdown_rg_b_reco, 
        h_ISRup_rg_b_reco, 
        h_ISRdown_rg_b_reco, 
        h_Bothup_rg_b_reco, 
        h_Bothdown_rg_b_reco
    };

    TH2D *h_FSRup_zg_b_reco, 
         *h_FSRdown_zg_b_reco, 
         *h_ISRup_zg_b_reco, 
         *h_ISRdown_zg_b_reco, 
         *h_Bothup_zg_b_reco, 
         *h_Bothdown_zg_b_reco;
    std::vector<TH2D *> histos_reweighted_zg_b_reco = {
        h_FSRup_zg_b_reco, 
        h_FSRdown_zg_b_reco, 
        h_ISRup_zg_b_reco, 
        h_ISRdown_zg_b_reco, 
        h_Bothup_zg_b_reco, 
        h_Bothdown_zg_b_reco
    };

    TH2D *h_FSRup_zpt_b_reco, 
         *h_FSRdown_zpt_b_reco, 
         *h_ISRup_zpt_b_reco, 
         *h_ISRdown_zpt_b_reco, 
         *h_Bothup_zpt_b_reco, 
         *h_Bothdown_zpt_b_reco;
    std::vector<TH2D *> histos_reweighted_zpt_b_reco = {
        h_FSRup_zpt_b_reco, 
        h_FSRdown_zpt_b_reco, 
        h_ISRup_zpt_b_reco, 
        h_ISRdown_zpt_b_reco, 
        h_Bothup_zpt_b_reco, 
        h_Bothdown_zpt_b_reco
    };

    TH2D *h_FSRup_rg_all_reco, 
         *h_FSRdown_rg_all_reco, 
         *h_ISRup_rg_all_reco, 
         *h_ISRdown_rg_all_reco, 
         *h_Bothup_rg_all_reco, 
         *h_Bothdown_rg_all_reco;
    std::vector<TH2D *> histos_reweighted_rg_all_reco = {
        h_FSRup_rg_all_reco, 
        h_FSRdown_rg_all_reco, 
        h_ISRup_rg_all_reco, 
        h_ISRdown_rg_all_reco, 
        h_Bothup_rg_all_reco, 
        h_Bothdown_rg_all_reco
    };

    TH2D *h_FSRup_zg_all_reco, 
         *h_FSRdown_zg_all_reco, 
         *h_ISRup_zg_all_reco, 
         *h_ISRdown_zg_all_reco, 
         *h_Bothup_zg_all_reco, 
         *h_Bothdown_zg_all_reco;
    std::vector<TH2D *> histos_reweighted_zg_all_reco = {
        h_FSRup_zg_all_reco, 
        h_FSRdown_zg_all_reco, 
        h_ISRup_zg_all_reco, 
        h_ISRdown_zg_all_reco, 
        h_Bothup_zg_all_reco, 
        h_Bothdown_zg_all_reco
    };

    TH2D *h_FSRup_zpt_all_reco, 
         *h_FSRdown_zpt_all_reco, 
         *h_ISRup_zpt_all_reco, 
         *h_ISRdown_zpt_all_reco, 
         *h_Bothup_zpt_all_reco, 
         *h_Bothdown_zpt_all_reco;
    std::vector<TH2D *> histos_reweighted_zpt_all_reco = {
        h_FSRup_zpt_all_reco, 
        h_FSRdown_zpt_all_reco, 
        h_ISRup_zpt_all_reco, 
        h_ISRdown_zpt_all_reco, 
        h_Bothup_zpt_all_reco, 
        h_Bothdown_zpt_all_reco
    };

    for (size_t ivar=0; ivar<variations.size(); ivar++) {
        TString var = variations[ivar];

        histos_reweighted_rg_b[ivar] = new TH2D("h_"+var+"_rg_b", "x=rg, y=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
        histos_reweighted_zg_b[ivar] = new TH2D("h_"+var+"_zg_b", "x=zg, y=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
        histos_reweighted_zpt_b[ivar] = new TH2D("h_"+var+"_zpt_b", "x=zpt, y=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);

        histos_reweighted_rg_all[ivar] = new TH2D("h_"+var+"_rg_all", "x=rg, y=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
        histos_reweighted_zg_all[ivar] = new TH2D("h_"+var+"_zg_all", "x=zg, y=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
        histos_reweighted_zpt_all[ivar] = new TH2D("h_"+var+"_zpt_all", "x=zpt, y=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);

        histos_reweighted_rg_b_reco[ivar] = new TH2D("h_"+var+"_rg_b_reco", "x=rg, y=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
        histos_reweighted_zg_b_reco[ivar] = new TH2D("h_"+var+"_zg_b_reco", "x=zg, y=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
        histos_reweighted_zpt_b_reco[ivar] = new TH2D("h_"+var+"_zpt_b_reco", "x=zpt, y=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);

        histos_reweighted_rg_all_reco[ivar] = new TH2D("h_"+var+"_rg_all_reco", "x=rg, y=jtpt", logrg_wU_bins, logrg_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
        histos_reweighted_zg_all_reco[ivar] = new TH2D("h_"+var+"_zg_all_reco", "x=zg, y=jtpt", zg_bins, zg_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
        histos_reweighted_zpt_all_reco[ivar] = new TH2D("h_"+var+"_zpt_all_reco", "x=zpt, y=jtpt", zpt_wU_bins, zpt_wU_binsVector, jtpt_wUO_bins, jtpt_wUO_binsVector);
    }

    TH2D *h_FSRdown_residuals_rg_all = new TH2D("h_FSRdown_residuals_rg_all", "x=(rgReco-rgPart)/rgPart, y=ptPart", 50, -1, 1, 3, 80, 140);
    TH2D *h_FSRdown_residuals_zg_all = new TH2D("h_FSRdown_residuals_zg_all", "x=(zgReco-zgPart)/zgPart, y=ptPart", 50, -1, 1, 3, 80, 140);
    TH2D *h_FSRdown_residuals_jtpt_all = new TH2D("h_FSRdown_residuals_jtpt_all", "x=(ptReco-ptPart)/ptPart, y=ptPart", 50, -1, 1, 3, 80, 140);

    double kt_cut = 1.;
    double logrg_underflow = -1.1; // SD-untagged
    double zg_underflow = 0.; // SD-untagged
    double zpt_underflow = -0.05; // jtptCh==0
    double jetR = 0.4;

    // Get variation weights
    TH2D *h_FSRup_weights_rg,
         *h_FSRdown_weights_rg,
         *h_ISRup_weights_rg,
         *h_ISRdown_weights_rg,
         *h_Bothup_weights_rg,
         *h_Bothdown_weights_rg;
    std::vector<TH2D *> histos_weights_rg = {
        h_FSRup_weights_rg,
        h_FSRdown_weights_rg,
        h_ISRup_weights_rg,
        h_ISRdown_weights_rg,
        h_Bothup_weights_rg,
        h_Bothdown_weights_rg
    };

    TH2D *h_FSRup_weights_zg,
         *h_FSRdown_weights_zg,
         *h_ISRup_weights_zg,
         *h_ISRdown_weights_zg,
         *h_Bothup_weights_zg,
         *h_Bothdown_weights_zg;
    std::vector<TH2D *> histos_weights_zg = {
        h_FSRup_weights_zg,
        h_FSRdown_weights_zg,
        h_ISRup_weights_zg,
        h_ISRdown_weights_zg,
        h_Bothup_weights_zg,
        h_Bothdown_weights_zg
    };

    TH2D *h_FSRup_weights_zpt,
         *h_FSRdown_weights_zpt,
         *h_ISRup_weights_zpt,
         *h_ISRdown_weights_zpt,
         *h_Bothup_weights_zpt,
         *h_Bothdown_weights_zpt;
    std::vector<TH2D *> histos_weights_zpt = {
        h_FSRup_weights_zpt,
        h_FSRdown_weights_zpt,
        h_ISRup_weights_zpt,
        h_ISRdown_weights_zpt,
        h_Bothup_weights_zpt,
        h_Bothdown_weights_zpt
    };

    TString sample_var = sample.Contains("dijet") ? "inclusive" : "bjet";
    for (int ivar=0; ivar<6; ivar++) {
        TString var = variations[ivar];

        TString fin_rg_weights_name = "histos/"+sample_var+"_"+var+"_weights_rg.root";
        std::cout << var << " rg weights: " << fin_rg_weights_name << std::endl;
        TFile *fin_rg_weights = new TFile(fin_rg_weights_name);
        histos_weights_rg[ivar] = (TH2D *) fin_rg_weights->Get("h_weights")->Clone("h_"+var+"_weights_rg");

        TString fin_zg_weights_name = "histos/"+sample_var+"_"+var+"_weights_zg.root";
        std::cout << var << " zg weights: " << fin_zg_weights_name << std::endl;
        TFile *fin_zg_weights = new TFile(fin_zg_weights_name);
        histos_weights_zg[ivar] = (TH2D *) fin_zg_weights->Get("h_weights")->Clone("h_"+var+"_weights_zg");

        TString fin_zpt_weights_name = "histos/"+sample_var+"_"+var+"_weights_zpt.root";
        std::cout << var << " zpt weights: " << fin_zpt_weights_name << std::endl;
        TFile *fin_zpt_weights = new TFile(fin_zpt_weights_name);
        histos_weights_zpt[ivar] = (TH2D *) fin_zpt_weights->Get("h_weights")->Clone("h_"+var+"_weights_zpt");
    }

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

            // Calculate rg, kt only for 2 prong jets
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

            // Checks for zpt
            Float_t jtBpt = tree.jtBpt[ijet];
            Float_t jtptCh = tree.jtptCh[ijet];
            Float_t zpt_reco = -1.;
            if (jtptCh>1e-4) zpt_reco = jtBpt / jtptCh;
            else zpt_reco = zpt_underflow; // underflow bin range
            if (zpt_reco>=1) zpt = 0.999; // zpt=1. included in last bin

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
            }

            if (kt_reco < kt_cut) {
                logrg_reco = logrg_underflow;
                zg_reco = zg_underflow;
            }

            for (int ivar=0; ivar<6; ivar++) {
                double weight_rg = get_weight(histos_weights_rg[ivar], logrg, refpt) * weight;
                double weight_zg = get_weight(histos_weights_zg[ivar], zg, refpt) * weight;
                double weight_zpt = get_weight(histos_weights_zpt[ivar], zpt, refpt) * weight;

                histos_reweighted_rg_all[ivar]->Fill(logrg, refpt, weight_rg);
                histos_reweighted_zg_all[ivar]->Fill(zg, refpt, weight_zg);
                histos_reweighted_zpt_all[ivar]->Fill(zpt, refpt, weight_zpt);

                histos_reweighted_rg_all_reco[ivar]->Fill(logrg_reco, jtpt, weight_rg);
                histos_reweighted_zg_all_reco[ivar]->Fill(zg_reco, jtpt, weight_zg);
                histos_reweighted_zpt_all_reco[ivar]->Fill(zpt_reco, jtpt, weight_zpt);

                if (tree.jtNbHad[ijet]==1) {
                    histos_reweighted_rg_b[ivar]->Fill(logrg, refpt, weight_rg);
                    histos_reweighted_zg_b[ivar]->Fill(zg, refpt, weight_zg);
                    histos_reweighted_zpt_b[ivar]->Fill(zpt, refpt, weight_zpt);

                    histos_reweighted_rg_b_reco[ivar]->Fill(logrg_reco, jtpt, weight_rg);
                    histos_reweighted_zg_b_reco[ivar]->Fill(zg_reco, jtpt, weight_zg);
                    histos_reweighted_zpt_b_reco[ivar]->Fill(zpt_reco, jtpt, weight_zpt);
                }

                if (ivar==1&&logrg>-1&&logrg_reco>-1) h_FSRdown_residuals_rg_all->Fill((logrg_reco-logrg)/logrg, refpt, weight_rg);
                if (ivar==1&&zg>0.05&&zg_reco>0.05) h_FSRdown_residuals_zg_all->Fill((zg_reco-zg)/zg, refpt, weight_zg);
                if (ivar==1) h_FSRdown_residuals_jtpt_all->Fill((jtpt-refpt)/refpt, refpt, weight_rg);

            } // var loop 
        } // jet loop
    }

    TString fout_name = "histos/"+sample+"_"+label+"_gen_reweighted_histograms.root";
    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (int ivar=0; ivar<6; ivar++) {
        histos_reweighted_rg_b[ivar]->Write();
        histos_reweighted_zg_b[ivar]->Write();
        histos_reweighted_zpt_b[ivar]->Write();

        histos_reweighted_rg_all[ivar]->Write();
        histos_reweighted_zg_all[ivar]->Write();
        histos_reweighted_zpt_all[ivar]->Write();

        histos_reweighted_rg_b_reco[ivar]->Write();
        histos_reweighted_zg_b_reco[ivar]->Write();
        histos_reweighted_zpt_b_reco[ivar]->Write();

        histos_reweighted_rg_all_reco[ivar]->Write();
        histos_reweighted_zg_all_reco[ivar]->Write();
        histos_reweighted_zpt_all_reco[ivar]->Write();
    } // var loop 

    h_FSRdown_residuals_rg_all->Write();
    h_FSRdown_residuals_zg_all->Write();
    h_FSRdown_residuals_jtpt_all->Write();

    fout->Close();
    delete fout;

    gApplication->Terminate();
}