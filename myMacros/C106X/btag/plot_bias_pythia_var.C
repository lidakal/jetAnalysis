#include "../binning.h"

std::vector<TString> variations = {
    "FSRup", "FSRdown",
    "ISRup", "ISRdown",
    "Bothup", "Bothdown",
};

double get_weight(TH2D *h, double x, double y) {
    int ibin_x = h->GetXaxis()->FindBin(x);
    int ibin_y = h->GetYaxis()->FindBin(y);
    double weight = h->GetBinContent(ibin_x, ibin_y);

    if (ibin_x<0||ibin_x>h->GetNbinsX()) std::cout << "x = " << x << " out of bounds" << std::endl;
    if (ibin_y<0||ibin_y>h->GetNbinsY()) std::cout << "y = " << y << " out of bounds" << std::endl;

    return weight;
}

void plot_bias_pythia_var()
{
    // We calculate the efficiency as: Nbtag/Nb
    // where N are both at gen level 
    // so in this case we need to change the N as well

    // --- DON'T TOUCH THESE ---
    bool sfDown = false;
    bool sfUp = false;

    TString jer_opt="nom"; 
    TString jec_opt="nom";
    // -------------------------

    int nbins_pt = jtpt_bins;
    int nbins_rg = logrg_bins;
    int nbins_zg = zg_bins;
    int nbins_zpt = zpt_bins;

    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;
    suffix += "_pythia_var";
    suffix += "_withSF";
    if (sfUp) suffix += "Up";
    if (sfDown) suffix += "Down";

    TString sample = "pythia_PF40";
    TString label = "aggrTMVA_inclusive";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";

    std::cout << "fin: " << fin_name << std::endl;
    std::cout << "sfUp: " << sfUp << std::endl;
    std::cout << "sfDown: " << sfDown << std::endl;
    std::cout << "jer_opt: " << jer_opt << std::endl;
    std::cout << "jec_opt: " << jec_opt << std::endl;
    
    TString fout_name = "histos/" + sample + "_" + label + "_histograms"+suffix+".root";

    
    TFile *fin = new TFile(fin_name);

    Double_t jtpt;
    Double_t jteta;
    Double_t jtptCh;
    Double_t logrg;
    Double_t logkt;
    Double_t zg;
    Double_t mb;
    Double_t bpt;

    Double_t jtpt_gen;
    Double_t jtptCh_gen;
    Double_t logrg_gen;
    Double_t logkt_gen;
    Double_t zg_gen;
    Double_t mb_gen;
    Double_t bpt_gen;

    Double_t weight;
    Double_t jer_sf_nom;
    Double_t jer_sf_up;
    Double_t jer_sf_down;
    Double_t jec_unc;
    Double_t pthat;

    Double_t discr_particleNet_BvsAll;

    TTree *tree_sig = (TTree *) fin->Get("tree_sig");
    tree_sig->SetBranchAddress("jtpt", &jtpt);
    tree_sig->SetBranchAddress("jteta", &jteta);
    tree_sig->SetBranchAddress("jtptCh", &jtptCh);
    tree_sig->SetBranchAddress("logrg", &logrg);
    tree_sig->SetBranchAddress("logkt", &logkt);
    tree_sig->SetBranchAddress("zg", &zg);
    tree_sig->SetBranchAddress("mb", &mb);
    tree_sig->SetBranchAddress("bpt", &bpt);

    tree_sig->SetBranchAddress("jtpt_gen", &jtpt_gen);
    tree_sig->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
    tree_sig->SetBranchAddress("logrg_gen", &logrg_gen);
    tree_sig->SetBranchAddress("logkt_gen", &logkt_gen);
    tree_sig->SetBranchAddress("zg_gen", &zg_gen);
    tree_sig->SetBranchAddress("mb_gen", &mb_gen);
    tree_sig->SetBranchAddress("bpt_gen", &bpt_gen);

    tree_sig->SetBranchAddress("weight", &weight);
    tree_sig->SetBranchAddress("jer_sf_nom", &jer_sf_nom);
    tree_sig->SetBranchAddress("jer_sf_up", &jer_sf_up);
    tree_sig->SetBranchAddress("jer_sf_down", &jer_sf_down);
    tree_sig->SetBranchAddress("jec_unc", &jec_unc);
    tree_sig->SetBranchAddress("pthat", &pthat);

    tree_sig->SetBranchAddress("discr_particleNet_BvsAll", &discr_particleNet_BvsAll);

    // Load SFs
    TString fin_sf_rg_name = "histos/aggrTMVA_inclusive_rg_sfs.root";
    std::cout << "fin rg sf: " << fin_sf_rg_name << std::endl;
    TFile *fin_sf_rg = new TFile(fin_sf_rg_name);
    TH2D *h_eff_sf_rg = (TH2D *) fin_sf_rg->Get("h_eff_sf")->Clone("h_eff_sf_rg");

    TString fin_sf_zg_name = "histos/aggrTMVA_inclusive_zg_sfs.root";
    std::cout << "fin zg sf: " << fin_sf_zg_name << std::endl;
    TFile *fin_sf_zg = new TFile(fin_sf_zg_name);
    TH2D *h_eff_sf_zg = (TH2D *) fin_sf_zg->Get("h_eff_sf")->Clone("h_eff_sf_zg");

    TString fin_sf_zpt_name = "histos/aggrTMVA_inclusive_zpt_sfs.root";
    std::cout << "fin zpt sf: " << fin_sf_zpt_name << std::endl;
    TFile *fin_sf_zpt = new TFile(fin_sf_zpt_name);
    TH2D *h_eff_sf_zpt = (TH2D *) fin_sf_zpt->Get("h_eff_sf")->Clone("h_eff_sf_zpt");

    TH2D *h_FSRup_rgpt_gen,
         *h_FSRdown_rgpt_gen,
         *h_ISRup_rgpt_gen,
         *h_ISRdown_rgpt_gen,
         *h_Bothup_rgpt_gen,
         *h_Bothdown_rgpt_gen;
    std::vector<TH2D *> histos_rgpt_gen = {
        h_FSRup_rgpt_gen,
        h_FSRdown_rgpt_gen,
        h_ISRup_rgpt_gen,
        h_ISRdown_rgpt_gen,
        h_Bothup_rgpt_gen,
        h_Bothdown_rgpt_gen,
    };
    
    TH2D *h_FSRup_zgpt_gen,
         *h_FSRdown_zgpt_gen,
         *h_ISRup_zgpt_gen,
         *h_ISRdown_zgpt_gen,
         *h_Bothup_zgpt_gen,
         *h_Bothdown_zgpt_gen;
    std::vector<TH2D *> histos_zgpt_gen = {
        h_FSRup_zgpt_gen,
        h_FSRdown_zgpt_gen,
        h_ISRup_zgpt_gen,
        h_ISRdown_zgpt_gen,
        h_Bothup_zgpt_gen,
        h_Bothdown_zgpt_gen,
    };
    
    TH2D *h_FSRup_zptpt_gen,
         *h_FSRdown_zptpt_gen,
         *h_ISRup_zptpt_gen,
         *h_ISRdown_zptpt_gen,
         *h_Bothup_zptpt_gen,
         *h_Bothdown_zptpt_gen;
    std::vector<TH2D *> histos_zptpt_gen = {
        h_FSRup_zptpt_gen,
        h_FSRdown_zptpt_gen,
        h_ISRup_zptpt_gen,
        h_ISRdown_zptpt_gen,
        h_Bothup_zptpt_gen,
        h_Bothdown_zptpt_gen,
    };

    TH2D *h_FSRup_rgpt_gen_tagged,
         *h_FSRdown_rgpt_gen_tagged,
         *h_ISRup_rgpt_gen_tagged,
         *h_ISRdown_rgpt_gen_tagged,
         *h_Bothup_rgpt_gen_tagged,
         *h_Bothdown_rgpt_gen_tagged;
    std::vector<TH2D *> histos_rgpt_gen_tagged = {
        h_FSRup_rgpt_gen_tagged,
        h_FSRdown_rgpt_gen_tagged,
        h_ISRup_rgpt_gen_tagged,
        h_ISRdown_rgpt_gen_tagged,
        h_Bothup_rgpt_gen_tagged,
        h_Bothdown_rgpt_gen_tagged,
    };
    
    TH2D *h_FSRup_zgpt_gen_tagged,
         *h_FSRdown_zgpt_gen_tagged,
         *h_ISRup_zgpt_gen_tagged,
         *h_ISRdown_zgpt_gen_tagged,
         *h_Bothup_zgpt_gen_tagged,
         *h_Bothdown_zgpt_gen_tagged;
    std::vector<TH2D *> histos_zgpt_gen_tagged = {
        h_FSRup_zgpt_gen_tagged,
        h_FSRdown_zgpt_gen_tagged,
        h_ISRup_zgpt_gen_tagged,
        h_ISRdown_zgpt_gen_tagged,
        h_Bothup_zgpt_gen_tagged,
        h_Bothdown_zgpt_gen_tagged,
    };
    
    TH2D *h_FSRup_zptpt_gen_tagged,
         *h_FSRdown_zptpt_gen_tagged,
         *h_ISRup_zptpt_gen_tagged,
         *h_ISRdown_zptpt_gen_tagged,
         *h_Bothup_zptpt_gen_tagged,
         *h_Bothdown_zptpt_gen_tagged;
    std::vector<TH2D *> histos_zptpt_gen_tagged = {
        h_FSRup_zptpt_gen_tagged,
        h_FSRdown_zptpt_gen_tagged,
        h_ISRup_zptpt_gen_tagged,
        h_ISRdown_zptpt_gen_tagged,
        h_Bothup_zptpt_gen_tagged,
        h_Bothdown_zptpt_gen_tagged,
    };

    TH2D *h_FSRup_rgpt_gen_tagged_withSF,
         *h_FSRdown_rgpt_gen_tagged_withSF,
         *h_ISRup_rgpt_gen_tagged_withSF,
         *h_ISRdown_rgpt_gen_tagged_withSF,
         *h_Bothup_rgpt_gen_tagged_withSF,
         *h_Bothdown_rgpt_gen_tagged_withSF;
    std::vector<TH2D *> histos_rgpt_gen_tagged_withSF = {
        h_FSRup_rgpt_gen_tagged_withSF,
        h_FSRdown_rgpt_gen_tagged_withSF,
        h_ISRup_rgpt_gen_tagged_withSF,
        h_ISRdown_rgpt_gen_tagged_withSF,
        h_Bothup_rgpt_gen_tagged_withSF,
        h_Bothdown_rgpt_gen_tagged_withSF,
    };
    
    TH2D *h_FSRup_zgpt_gen_tagged_withSF,
         *h_FSRdown_zgpt_gen_tagged_withSF,
         *h_ISRup_zgpt_gen_tagged_withSF,
         *h_ISRdown_zgpt_gen_tagged_withSF,
         *h_Bothup_zgpt_gen_tagged_withSF,
         *h_Bothdown_zgpt_gen_tagged_withSF;
    std::vector<TH2D *> histos_zgpt_gen_tagged_withSF = {
        h_FSRup_zgpt_gen_tagged_withSF,
        h_FSRdown_zgpt_gen_tagged_withSF,
        h_ISRup_zgpt_gen_tagged_withSF,
        h_ISRdown_zgpt_gen_tagged_withSF,
        h_Bothup_zgpt_gen_tagged_withSF,
        h_Bothdown_zgpt_gen_tagged_withSF,
    };
    
    TH2D *h_FSRup_zptpt_gen_tagged_withSF,
         *h_FSRdown_zptpt_gen_tagged_withSF,
         *h_ISRup_zptpt_gen_tagged_withSF,
         *h_ISRdown_zptpt_gen_tagged_withSF,
         *h_Bothup_zptpt_gen_tagged_withSF,
         *h_Bothdown_zptpt_gen_tagged_withSF;
    std::vector<TH2D *> histos_zptpt_gen_tagged_withSF = {
        h_FSRup_zptpt_gen_tagged_withSF,
        h_FSRdown_zptpt_gen_tagged_withSF,
        h_ISRup_zptpt_gen_tagged_withSF,
        h_ISRdown_zptpt_gen_tagged_withSF,
        h_Bothup_zptpt_gen_tagged_withSF,
        h_Bothdown_zptpt_gen_tagged_withSF,
    };

    for (int ivar=0; ivar<6; ivar++) {
        TString var = variations[ivar];
        histos_rgpt_gen[ivar] = new TH2D("h_"+var+"_rgpt_gen", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
        histos_zgpt_gen[ivar] = new TH2D("h_"+var+"_zgpt_gen", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
        histos_zptpt_gen[ivar] = new TH2D("h_"+var+"_zptpt_gen", "x=zpt_gen, y=jtpt_gen", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

        histos_rgpt_gen_tagged[ivar] = new TH2D("h_"+var+"_rgpt_gen_tagged", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
        histos_zgpt_gen_tagged[ivar] = new TH2D("h_"+var+"_zgpt_gen_tagged", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
        histos_zptpt_gen_tagged[ivar] = new TH2D("h_"+var+"_zptpt_gen_tagged", "x=zpt_gen, y=jtpt_gen", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

        histos_rgpt_gen_tagged_withSF[ivar] = new TH2D("h_"+var+"_rgpt_gen_tagged_withSF", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
        histos_zgpt_gen_tagged_withSF[ivar] = new TH2D("h_"+var+"_zgpt_gen_tagged_withSF", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
        histos_zptpt_gen_tagged_withSF[ivar] = new TH2D("h_"+var+"_zptpt_gen_tagged_withSF", "x=zpt_gen, y=jtpt_gen", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);
    }

    // Load weights
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

    for (int ivar=0; ivar<6; ivar++) {
        TString var = variations[ivar];

        TString fin_rg_weights_name = "../histos/bjet_"+var+"_weights_rg.root";
        std::cout << var << " rg weights: " << fin_rg_weights_name << std::endl;
        TFile *fin_rg_weights = new TFile(fin_rg_weights_name);
        histos_weights_rg[ivar] = (TH2D *) fin_rg_weights->Get("h_weights")->Clone("h_"+var+"_weights_rg");

        TString fin_zg_weights_name = "../histos/bjet_"+var+"_weights_zg.root";
        std::cout << var << " zg weights: " << fin_zg_weights_name << std::endl;
        TFile *fin_zg_weights = new TFile(fin_zg_weights_name);
        histos_weights_zg[ivar] = (TH2D *) fin_zg_weights->Get("h_weights")->Clone("h_"+var+"_weights_zg");

        TString fin_zpt_weights_name = "../histos/bjet_"+var+"_weights_zpt.root";
        std::cout << var << " zpt weights: " << fin_zpt_weights_name << std::endl;
        TFile *fin_zpt_weights = new TFile(fin_zpt_weights_name);
        histos_weights_zpt[ivar] = (TH2D *) fin_zpt_weights->Get("h_weights")->Clone("h_"+var+"_weights_zpt");
    }

    std::cout << "filling histograms" << std::endl;
    for (Long64_t ient = 0; ient < tree_sig->GetEntries(); ient++) {
        // if (ient > 1) break;
        tree_sig->GetEntry(ient);
        if (skipMC(jtpt, jtpt_gen, pthat)) continue;

        // Apply JER nom 
        if (jer_opt=="nom") jtpt = jtpt * jer_sf_nom;
        else std::cout << "jer_opt=" << jer_opt << " not accepted" << std::endl;

        // ---- Fix observable limits ----

        // Checks for logrg 
        if (logrg < 0. && logrg > -900.) continue; // rg>0.4 -> skip for now
        if (logrg < -900.) logrg = -0.2; // SD-untagged bin range
        if (logkt < 0. && logkt > -900.) logrg = -0.2; // kt<1 -> SD-untagged bin range
        if (logrg >= 2.5) logrg = 2.499; // overflow bin range
        if (logrg_gen < 0. && logrg_gen>-900) logrg_gen = -900; // rg_gen>0.4 -> out of range but not SD-untagged
        if (logrg_gen < -900.) logrg_gen = -0.2; // SD-untagged bin range
        if (logkt_gen < 0. && logkt_gen > -900.) logrg_gen = -0.2; // kt_gen<1 -> SD-untagged bin range
        if (logrg_gen >= 2.5) logrg_gen = 2.499; // overflow bin range

        // Checks for zg
        if (zg < -900.) zg = 0.; // SD-untagged bin range
        if (logkt < 0. && logkt > -900.) zg = 0.; // kt<1 -> SD-untagged bin range
        if (zg >= 0.5) zg = 0.499; // zg=0.5 included in last bin
        if (zg_gen < -900.) zg_gen = 0.; // SD-untagged bin range
        if (logkt_gen < 0. && logkt_gen > -900.) zg_gen = 0.; // kt<1 -> SD-untagged bin range
        if (zg_gen >= 0.5) zg_gen = 0.499; // zg=0.5 included in last bin

        // Checks for zpt
        if (std::abs(jtptCh) < 1e-4) continue;
        double zpt = -1.;
        if (jtptCh > 0) zpt = bpt / jtptCh;
        if (zpt>=1.) zpt = 0.999; // zpt=1 included in last bin
        double zpt_gen = -1.;
        if (jtptCh_gen > 0) zpt_gen = bpt_gen / jtptCh_gen;
        if (zpt_gen>=1.) zpt_gen = 0.999; // zpt_gen=1 included in last bin

        // --------------------------

        for (int ivar=0; ivar<6; ivar++) {
            double weight_rg = get_weight(histos_weights_rg[ivar], logrg_gen, jtpt_gen) * weight;
            double weight_zg = get_weight(histos_weights_zg[ivar], zg_gen, jtpt_gen) * weight;
            double weight_zpt = get_weight(histos_weights_zpt[ivar], zpt_gen, jtpt_gen) * weight;

            // Fill inclusive histograms 
            histos_rgpt_gen[ivar]->Fill(logrg_gen, jtpt_gen, weight);
            histos_zgpt_gen[ivar]->Fill(zg_gen, jtpt_gen, weight);
            histos_zptpt_gen[ivar]->Fill(zpt_gen, jtpt_gen, weight);

            // Fill tagged histograms
            if (discr_particleNet_BvsAll>0.99) {
                histos_rgpt_gen_tagged[ivar]->Fill(logrg_gen, jtpt_gen, weight);
                histos_zgpt_gen_tagged[ivar]->Fill(zg_gen, jtpt_gen, weight);
                histos_zptpt_gen_tagged[ivar]->Fill(zpt_gen, jtpt_gen, weight);

                // Find reco SF 
                int ibin_pt = h_eff_sf_rg->GetYaxis()->FindBin(jtpt); // should be the same for all observables
                if (ibin_pt==0) ibin_pt = 1;
                if (ibin_pt==nbins_pt+1) ibin_pt = h_eff_sf_rg->GetNbinsY();

                int ibin_rg = h_eff_sf_rg->GetXaxis()->FindBin(logrg);
                int ibin_zg = h_eff_sf_zg->GetXaxis()->FindBin(zg);
                int ibin_zpt = h_eff_sf_zpt->GetXaxis()->FindBin(zpt);
                
                double sf_rg = h_eff_sf_rg->GetBinContent(ibin_rg, ibin_pt);
                weight_rg *= sf_rg;

                double sf_zg = h_eff_sf_zg->GetBinContent(ibin_zg, ibin_pt);
                weight_zg *= sf_zg;

                double sf_zpt = h_eff_sf_zpt->GetBinContent(ibin_zpt, ibin_pt);
                weight_zpt *= sf_zpt;
                
                histos_rgpt_gen_tagged_withSF[ivar]->Fill(logrg_gen, jtpt_gen, weight_rg);
                histos_zgpt_gen_tagged_withSF[ivar]->Fill(zg_gen, jtpt_gen, weight_zg);
                histos_zptpt_gen_tagged_withSF[ivar]->Fill(zpt_gen, jtpt_gen, weight_zpt);
            } // end if tagged
        } // end variation loop
    }

    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (int ivar=0; ivar<6; ivar++) {
        histos_rgpt_gen[ivar]->Write("", TObject::kOverwrite);
        histos_zgpt_gen[ivar]->Write("", TObject::kOverwrite);
        histos_zptpt_gen[ivar]->Write("", TObject::kOverwrite);

        histos_rgpt_gen_tagged[ivar]->Write("", TObject::kOverwrite);
        histos_zgpt_gen_tagged[ivar]->Write("", TObject::kOverwrite);
        histos_zptpt_gen_tagged[ivar]->Write("", TObject::kOverwrite);

        histos_rgpt_gen_tagged_withSF[ivar]->Write("", TObject::kOverwrite);
        histos_zgpt_gen_tagged_withSF[ivar]->Write("", TObject::kOverwrite);
        histos_zptpt_gen_tagged_withSF[ivar]->Write("", TObject::kOverwrite);
    }

    fout->Close();
    delete fout;
}
