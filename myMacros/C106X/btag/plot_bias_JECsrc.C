#include "../binning.h"

const std::vector<TString> srcnames_bjet = {
    "AbsoluteStat", "AbsoluteScale", "AbsoluteFlavMap", "AbsoluteMPFBias", 
    "Fragmentation", "SinglePionECAL", "SinglePionHCAL", "TimePtEta", 
    "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF", "RelativePtBB", 
    "RelativePtEC1", "RelativePtEC2", "RelativePtHF", "RelativeBal", 
    "RelativeSample", "RelativeFSR", "RelativeStatFSR", "RelativeStatEC", 
    "PileUpDataMC", "PileUpPtRef", "PileUpPtBB", "PileUpPtEC1", 
    "PileUpPtEC2", "PileUpPtHF", "FlavorPureBottom", "TimeRunF"
};

const TString jec_unc_mc_src_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/condor/correction_files/Summer19UL17_V6_MC_UncertaintySources_AK4PFchs.txt";

void plot_bias_JECsrc(TString jer_opt="nom", TString jec_opt="down")
{
    // We calculate the efficiency as: Nbtag/Nb
    // where N are both at gen level 
    // while the JEC is applied at reco
    // so we only need to change the histograms with SFs (which read from reco)

    bool sfDown = false;
    bool sfUp = false;

    int nbins_pt = jtpt_bins;
    int nbins_rg = logrg_bins;
    int nbins_zg = zg_bins;
    int nbins_zpt = zpt_bins;

    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;
    suffix += "_JECsrc";
    suffix += "_withSF";
    if (sfUp) suffix += "Up";
    if (sfDown) suffix += "Down";

    TString sample = "pythia_PF40";
    TString label = "aggrTMVA_inclusive";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";

    std::cout << "fin: " << fin_name << std::endl;
    std::cout << "sfUp: " << sfUp << std::endl;
    std::cout << "sfDown: " << sfDown << std::endl;
    
    TString fout_name = "histos/" + sample + "_" + label + "_histograms"+suffix+".root";

    // JEC unc source decomposition
    std::vector<JetCorrectionUncertainty*> jecUncPerSrc;
    for (TString name : srcnames_bjet) {
        JetCorrectorParameters param(jec_unc_mc_src_fname.Data(), name.Data());
        jecUncPerSrc.push_back(new JetCorrectionUncertainty(param));
    }

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

    TH2D *h_rgpt_gen = new TH2D("h_rgpt_gen", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zgpt_gen = new TH2D("h_zgpt_gen", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zptpt_gen = new TH2D("h_zptpt_gen", "x=zpt_gen, y=jtpt_gen", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

    TH2D *h_rgpt_gen_tagged = new TH2D("h_rgpt_gen_tagged", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zgpt_gen_tagged = new TH2D("h_zgpt_gen_tagged", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zptpt_gen_tagged = new TH2D("h_zptpt_gen_tagged", "x=zpt_gen, y=jtpt_gen", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);
    
    TH2D *h0_rgpt_gen_tagged_withSF,
         *h1_rgpt_gen_tagged_withSF,
         *h2_rgpt_gen_tagged_withSF,
         *h3_rgpt_gen_tagged_withSF,
         *h4_rgpt_gen_tagged_withSF,
         *h5_rgpt_gen_tagged_withSF,
         *h6_rgpt_gen_tagged_withSF,
         *h7_rgpt_gen_tagged_withSF,
         *h8_rgpt_gen_tagged_withSF,
         *h9_rgpt_gen_tagged_withSF,
         *h10_rgpt_gen_tagged_withSF,
         *h11_rgpt_gen_tagged_withSF,
         *h12_rgpt_gen_tagged_withSF,
         *h13_rgpt_gen_tagged_withSF,
         *h14_rgpt_gen_tagged_withSF,
         *h15_rgpt_gen_tagged_withSF,
         *h16_rgpt_gen_tagged_withSF,
         *h17_rgpt_gen_tagged_withSF,
         *h18_rgpt_gen_tagged_withSF,
         *h19_rgpt_gen_tagged_withSF,
         *h20_rgpt_gen_tagged_withSF,
         *h21_rgpt_gen_tagged_withSF,
         *h22_rgpt_gen_tagged_withSF,
         *h23_rgpt_gen_tagged_withSF,
         *h24_rgpt_gen_tagged_withSF,
         *h25_rgpt_gen_tagged_withSF,
         *h26_rgpt_gen_tagged_withSF,
         *h27_rgpt_gen_tagged_withSF;

    std::vector<TH2D *> histos_rgpt_gen_tagged_withSF = {
        h0_rgpt_gen_tagged_withSF,
        h1_rgpt_gen_tagged_withSF,
        h2_rgpt_gen_tagged_withSF,
        h3_rgpt_gen_tagged_withSF,
        h4_rgpt_gen_tagged_withSF,
        h5_rgpt_gen_tagged_withSF,
        h6_rgpt_gen_tagged_withSF,
        h7_rgpt_gen_tagged_withSF,
        h8_rgpt_gen_tagged_withSF,
        h9_rgpt_gen_tagged_withSF,
        h10_rgpt_gen_tagged_withSF,
        h11_rgpt_gen_tagged_withSF,
        h12_rgpt_gen_tagged_withSF,
        h13_rgpt_gen_tagged_withSF,
        h14_rgpt_gen_tagged_withSF,
        h15_rgpt_gen_tagged_withSF,
        h16_rgpt_gen_tagged_withSF,
        h17_rgpt_gen_tagged_withSF,
        h18_rgpt_gen_tagged_withSF,
        h19_rgpt_gen_tagged_withSF,
        h20_rgpt_gen_tagged_withSF,
        h21_rgpt_gen_tagged_withSF,
        h22_rgpt_gen_tagged_withSF,
        h23_rgpt_gen_tagged_withSF,
        h24_rgpt_gen_tagged_withSF,
        h25_rgpt_gen_tagged_withSF,
        h26_rgpt_gen_tagged_withSF,
        h27_rgpt_gen_tagged_withSF
    };
    
    TH2D *h0_zgpt_gen_tagged_withSF,
         *h1_zgpt_gen_tagged_withSF,
         *h2_zgpt_gen_tagged_withSF,
         *h3_zgpt_gen_tagged_withSF,
         *h4_zgpt_gen_tagged_withSF,
         *h5_zgpt_gen_tagged_withSF,
         *h6_zgpt_gen_tagged_withSF,
         *h7_zgpt_gen_tagged_withSF,
         *h8_zgpt_gen_tagged_withSF,
         *h9_zgpt_gen_tagged_withSF,
         *h10_zgpt_gen_tagged_withSF,
         *h11_zgpt_gen_tagged_withSF,
         *h12_zgpt_gen_tagged_withSF,
         *h13_zgpt_gen_tagged_withSF,
         *h14_zgpt_gen_tagged_withSF,
         *h15_zgpt_gen_tagged_withSF,
         *h16_zgpt_gen_tagged_withSF,
         *h17_zgpt_gen_tagged_withSF,
         *h18_zgpt_gen_tagged_withSF,
         *h19_zgpt_gen_tagged_withSF,
         *h20_zgpt_gen_tagged_withSF,
         *h21_zgpt_gen_tagged_withSF,
         *h22_zgpt_gen_tagged_withSF,
         *h23_zgpt_gen_tagged_withSF,
         *h24_zgpt_gen_tagged_withSF,
         *h25_zgpt_gen_tagged_withSF,
         *h26_zgpt_gen_tagged_withSF,
         *h27_zgpt_gen_tagged_withSF;

    std::vector<TH2D *> histos_zgpt_gen_tagged_withSF = {
        h0_zgpt_gen_tagged_withSF,
        h1_zgpt_gen_tagged_withSF,
        h2_zgpt_gen_tagged_withSF,
        h3_zgpt_gen_tagged_withSF,
        h4_zgpt_gen_tagged_withSF,
        h5_zgpt_gen_tagged_withSF,
        h6_zgpt_gen_tagged_withSF,
        h7_zgpt_gen_tagged_withSF,
        h8_zgpt_gen_tagged_withSF,
        h9_zgpt_gen_tagged_withSF,
        h10_zgpt_gen_tagged_withSF,
        h11_zgpt_gen_tagged_withSF,
        h12_zgpt_gen_tagged_withSF,
        h13_zgpt_gen_tagged_withSF,
        h14_zgpt_gen_tagged_withSF,
        h15_zgpt_gen_tagged_withSF,
        h16_zgpt_gen_tagged_withSF,
        h17_zgpt_gen_tagged_withSF,
        h18_zgpt_gen_tagged_withSF,
        h19_zgpt_gen_tagged_withSF,
        h20_zgpt_gen_tagged_withSF,
        h21_zgpt_gen_tagged_withSF,
        h22_zgpt_gen_tagged_withSF,
        h23_zgpt_gen_tagged_withSF,
        h24_zgpt_gen_tagged_withSF,
        h25_zgpt_gen_tagged_withSF,
        h26_zgpt_gen_tagged_withSF,
        h27_zgpt_gen_tagged_withSF
    };
    
    TH2D *h0_zptpt_gen_tagged_withSF,
         *h1_zptpt_gen_tagged_withSF,
         *h2_zptpt_gen_tagged_withSF,
         *h3_zptpt_gen_tagged_withSF,
         *h4_zptpt_gen_tagged_withSF,
         *h5_zptpt_gen_tagged_withSF,
         *h6_zptpt_gen_tagged_withSF,
         *h7_zptpt_gen_tagged_withSF,
         *h8_zptpt_gen_tagged_withSF,
         *h9_zptpt_gen_tagged_withSF,
         *h10_zptpt_gen_tagged_withSF,
         *h11_zptpt_gen_tagged_withSF,
         *h12_zptpt_gen_tagged_withSF,
         *h13_zptpt_gen_tagged_withSF,
         *h14_zptpt_gen_tagged_withSF,
         *h15_zptpt_gen_tagged_withSF,
         *h16_zptpt_gen_tagged_withSF,
         *h17_zptpt_gen_tagged_withSF,
         *h18_zptpt_gen_tagged_withSF,
         *h19_zptpt_gen_tagged_withSF,
         *h20_zptpt_gen_tagged_withSF,
         *h21_zptpt_gen_tagged_withSF,
         *h22_zptpt_gen_tagged_withSF,
         *h23_zptpt_gen_tagged_withSF,
         *h24_zptpt_gen_tagged_withSF,
         *h25_zptpt_gen_tagged_withSF,
         *h26_zptpt_gen_tagged_withSF,
         *h27_zptpt_gen_tagged_withSF;

    std::vector<TH2D *> histos_zptpt_gen_tagged_withSF = {
        h0_zptpt_gen_tagged_withSF,
        h1_zptpt_gen_tagged_withSF,
        h2_zptpt_gen_tagged_withSF,
        h3_zptpt_gen_tagged_withSF,
        h4_zptpt_gen_tagged_withSF,
        h5_zptpt_gen_tagged_withSF,
        h6_zptpt_gen_tagged_withSF,
        h7_zptpt_gen_tagged_withSF,
        h8_zptpt_gen_tagged_withSF,
        h9_zptpt_gen_tagged_withSF,
        h10_zptpt_gen_tagged_withSF,
        h11_zptpt_gen_tagged_withSF,
        h12_zptpt_gen_tagged_withSF,
        h13_zptpt_gen_tagged_withSF,
        h14_zptpt_gen_tagged_withSF,
        h15_zptpt_gen_tagged_withSF,
        h16_zptpt_gen_tagged_withSF,
        h17_zptpt_gen_tagged_withSF,
        h18_zptpt_gen_tagged_withSF,
        h19_zptpt_gen_tagged_withSF,
        h20_zptpt_gen_tagged_withSF,
        h21_zptpt_gen_tagged_withSF,
        h22_zptpt_gen_tagged_withSF,
        h23_zptpt_gen_tagged_withSF,
        h24_zptpt_gen_tagged_withSF,
        h25_zptpt_gen_tagged_withSF,
        h26_zptpt_gen_tagged_withSF,
        h27_zptpt_gen_tagged_withSF
    };

    for (int i=0; i<28; i++) {
        histos_rgpt_gen_tagged_withSF[i] = new TH2D(Form("h%d_rgpt_gen_tagged_withSF",i), "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
        histos_zgpt_gen_tagged_withSF[i] = new TH2D(Form("h%d_zgpt_gen_tagged_withSF",i), "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
        histos_zptpt_gen_tagged_withSF[i] = new TH2D(Form("h%d_zptpt_gen_tagged_withSF",i), "x=zpt_gen, y=jtpt_gen", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);
    }

    // find correct SF based on reco 
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

    std::cout << "filling histograms" << std::endl;
    for (Long64_t ient = 0; ient < tree_sig->GetEntries(); ient++) {
        // if (ient > 1) continue;
        tree_sig->GetEntry(ient);
        if (skipMC(jtpt, jtpt_gen, pthat)) continue;

        std::vector<double> jecSFs = {};
        for (int i=0; i<jecUncPerSrc.size(); i++) {
            JetCorrectionUncertainty *jecUnc = jecUncPerSrc[i];
            jecUnc->setJetPt(jtpt);
            jecUnc->setJetEta(jteta);
            double unc = jecUnc->getUncertainty(true);
            double fact = 0.;
            if (jec_opt.Contains("up")) fact = 1.;
            else if (jec_opt.Contains("down")) fact = -1.;
            unc *= fact;
            jecSFs.push_back(1+unc);
        }

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


        // Fill inclusive histograms 
        h_rgpt_gen->Fill(logrg_gen, jtpt_gen, weight);
        h_zgpt_gen->Fill(zg_gen, jtpt_gen, weight);
        h_zptpt_gen->Fill(zpt_gen, jtpt_gen, weight);

        // Fill tagged histograms
        if (discr_particleNet_BvsAll>0.99) {
            h_rgpt_gen_tagged->Fill(logrg_gen, jtpt_gen, weight);
            h_zgpt_gen_tagged->Fill(zg_gen, jtpt_gen, weight);
            h_zptpt_gen_tagged->Fill(zpt_gen, jtpt_gen, weight);

            // Fill tagged histograms with SF
            for (int i=0; i<28; i++) {
                jtpt *= jecSFs[i]; // jer SF already applied

                double weight_rg = weight;
                double weight_zg = weight;
                double weight_mb = weight;
                double weight_zpt = weight;

                // Find reco SF 
                int ibin_pt = h_eff_sf_rg->GetYaxis()->FindBin(jtpt); // should be the same for all observables
                if (ibin_pt==0) ibin_pt = 1;
                if (ibin_pt==nbins_pt+1) ibin_pt = nbins_pt;

                int ibin_rg = h_eff_sf_rg->GetXaxis()->FindBin(logrg);
                int ibin_zg = h_eff_sf_zg->GetXaxis()->FindBin(zg);
                int ibin_zpt = h_eff_sf_zpt->GetXaxis()->FindBin(zpt);
                
                if (ibin_zg==0||ibin_zg>nbins_zg) std::cout << "YELL" << std::endl;
                double sf_rg = h_eff_sf_rg->GetBinContent(ibin_rg, ibin_pt);
                double sfUnc_rg = std::abs(sf_rg - 1.); 
                if (sfUp) sf_rg += sfUnc_rg;
                if (sfDown) sf_rg -= sfUnc_rg;
                weight_rg *= sf_rg;

                double sf_zg = h_eff_sf_zg->GetBinContent(ibin_zg, ibin_pt);
                double sfUnc_zg = std::abs(sf_zg - 1.); 
                if (sfUp) sf_zg += sfUnc_zg;
                if (sfDown) sf_zg -= sfUnc_zg;
                weight_zg *= sf_zg;

                double sf_zpt = h_eff_sf_zpt->GetBinContent(ibin_zpt, ibin_pt);
                double sfUnc_zpt = std::abs(sf_zpt - 1.); 
                if (sfUp) sf_zpt += sfUnc_zpt;
                if (sfDown) sf_zpt -= sfUnc_zpt;
                weight_zpt *= sf_zpt;
                
                histos_rgpt_gen_tagged_withSF[i]->Fill(logrg_gen, jtpt_gen, weight_rg);
                histos_zgpt_gen_tagged_withSF[i]->Fill(zg_gen, jtpt_gen, weight_zg);
                histos_zptpt_gen_tagged_withSF[i]->Fill(zpt_gen, jtpt_gen, weight_zpt);
                
                jtpt /= jecSFs[i];
            } // jer src
        } // if tagged
    }

    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {
        h_rgpt_gen, h_zgpt_gen, h_zptpt_gen,
        h_rgpt_gen_tagged, h_zgpt_gen_tagged, h_zptpt_gen_tagged,
        }) {
        h->Write("", TObject::kOverwrite);
    }

    for (int i=0; i<28; i++) {
        histos_rgpt_gen_tagged_withSF[i]->Write("", TObject::kOverwrite);
        histos_zgpt_gen_tagged_withSF[i]->Write("", TObject::kOverwrite);
        histos_zptpt_gen_tagged_withSF[i]->Write("", TObject::kOverwrite);
    }

    fout->Close();
    delete fout;
}
