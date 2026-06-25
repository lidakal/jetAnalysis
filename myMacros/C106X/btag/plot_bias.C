#include "../binning.h"

void plot_bias(TString sample = "herwig_official_PF40", TString jer_opt="nom", TString jec_opt="nom")
{
    bool sfDown = false;
    bool sfUp = false;

    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;
    suffix += "_withSF";
    if (sfUp) suffix += "Up";
    if (sfDown) suffix += "Down";
    
    TString label = "aggrTMVA_inclusive"; // don't change
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";

    std::cout << "fin: " << fin_name << std::endl;
    std::cout << "sfUp: " << sfUp << std::endl;
    std::cout << "sfDown: " << sfDown << std::endl;
    
    TString fout_name = "histos/" + sample + "_" + label + "_histograms"+suffix+".root";

    TFile *fin = new TFile(fin_name);

    Double_t jtpt;
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

    TH2D *h_rgpt = new TH2D("h_rgpt", "x=logrg, y=jtpt", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zgpt = new TH2D("h_zgpt", "x=zg, y=jtpt", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zptpt = new TH2D("h_zptpt", "x=zpt, y=jtpt", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

    TH2D *h_rgpt_tagged = new TH2D("h_rgpt_tagged", "x=logrg, y=jtpt", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zgpt_tagged = new TH2D("h_zgpt_tagged", "x=zg, y=jtpt", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zptpt_tagged = new TH2D("h_zptpt_tagged", "x=zpt, y=jtpt", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

    TH2D *h_rgpt_tagged_withSF = new TH2D("h_rgpt_tagged_withSF", "x=logrg, y=jtpt", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zgpt_tagged_withSF = new TH2D("h_zgpt_tagged_withSF", "x=zg, y=jtpt", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zptpt_tagged_withSF = new TH2D("h_zptpt_tagged_withSF", "x=zpt, y=jtpt", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

    TH2D *h_rgpt_gen = new TH2D("h_rgpt_gen", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zgpt_gen = new TH2D("h_zgpt_gen", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zptpt_gen = new TH2D("h_zptpt_gen", "x=zpt_gen, y=jtpt_gen", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

    TH2D *h_rgpt_gen_tagged = new TH2D("h_rgpt_gen_tagged", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zgpt_gen_tagged = new TH2D("h_zgpt_gen_tagged", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zptpt_gen_tagged = new TH2D("h_zptpt_gen_tagged", "x=zpt_gen, y=jtpt_gen", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

    TH2D *h_rgpt_gen_tagged_withSF = new TH2D("h_rgpt_gen_tagged_withSF", "x=logrg_gen, y=jtpt_gen", logrg_bins, logrg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zgpt_gen_tagged_withSF = new TH2D("h_zgpt_gen_tagged_withSF", "x=zg_gen, y=jtpt_gen", zg_bins, zg_binsVector, jtpt_bins, jtpt_binsVector);
    TH2D *h_zptpt_gen_tagged_withSF = new TH2D("h_zptpt_gen_tagged_withSF", "x=zpt_gen, y=jtpt_gen", zpt_bins, zpt_binsVector, jtpt_bins, jtpt_binsVector);

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

    int nbins_pt = h_eff_sf_rg->GetNbinsY();
    int nbins_rg = h_eff_sf_rg->GetNbinsX();
    int nbins_zg = h_eff_sf_zg->GetNbinsX();
    int nbins_zpt = h_eff_sf_zpt->GetNbinsX();

    std::cout << "nbins_pt=" << nbins_pt << std::endl;

    std::cout << "filling histograms" << std::endl;
    for (Long64_t ient = 0; ient < tree_sig->GetEntries(); ient++) {
        // if (ient > 1) continue;
        tree_sig->GetEntry(ient);
        if (skipMC(jtpt, jtpt_gen, pthat)) continue;

        if (jer_opt=="nom") jtpt = jtpt * jer_sf_nom;
        else if (jer_opt=="up") jtpt = jtpt * jer_sf_up;
        else if (jer_opt=="down") jtpt = jtpt * jer_sf_down;

        double jec_fact = 0; // nominal
        if (jec_opt=="up") jec_fact = 1;
        if (jec_opt=="down") jec_fact = -1;
        double newCorrection = 1 + (jec_fact * jec_unc);
        jtpt = jtpt * newCorrection;

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
        h_rgpt->Fill(logrg, jtpt, weight);
        h_zgpt->Fill(zg, jtpt, weight);
        h_zptpt->Fill(zpt, jtpt, weight);

        h_rgpt_gen->Fill(logrg_gen, jtpt_gen, weight);
        h_zgpt_gen->Fill(zg_gen, jtpt_gen, weight);
        h_zptpt_gen->Fill(zpt_gen, jtpt_gen, weight);

        // Fill tagged histograms 
        if (discr_particleNet_BvsAll>0.99) {
            h_rgpt_tagged->Fill(logrg, jtpt, weight);
            h_zgpt_tagged->Fill(zg, jtpt, weight);
            h_zptpt_tagged->Fill(zpt, jtpt, weight);

            h_rgpt_gen_tagged->Fill(logrg_gen, jtpt_gen, weight);
            h_zgpt_gen_tagged->Fill(zg_gen, jtpt_gen, weight);
            h_zptpt_gen_tagged->Fill(zpt_gen, jtpt_gen, weight);

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
            
            h_rgpt_tagged_withSF->Fill(logrg, jtpt, weight_rg);
            h_zgpt_tagged_withSF->Fill(zg, jtpt, weight_zg);
            h_zptpt_tagged_withSF->Fill(zpt, jtpt, weight_zpt);

            h_rgpt_gen_tagged_withSF->Fill(logrg_gen, jtpt_gen, weight_rg);
            h_zgpt_gen_tagged_withSF->Fill(zg_gen, jtpt_gen, weight_zg);
            h_zptpt_gen_tagged_withSF->Fill(zpt_gen, jtpt_gen, weight_zpt);
        }
    }

    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {
        h_rgpt, h_zgpt, h_zptpt,
        h_rgpt_tagged, h_zgpt_tagged, h_zptpt_tagged,
        h_rgpt_tagged_withSF, h_zgpt_tagged_withSF, h_zptpt_tagged_withSF,
        h_rgpt_gen, h_zgpt_gen, h_zptpt_gen,
        h_rgpt_gen_tagged, h_zgpt_gen_tagged, h_zptpt_gen_tagged,
        h_rgpt_gen_tagged_withSF, h_zgpt_gen_tagged_withSF, h_zptpt_gen_tagged_withSF
        }) {
        h->Write("", TObject::kOverwrite);
    }

    fout->Close();
    delete fout;
}
