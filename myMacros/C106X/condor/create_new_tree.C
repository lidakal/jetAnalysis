#include <TTree.h>
#include <TFile.h>
#include <TRandom.h>
#include <TApplication.h>

#include "tTree.h"
#include "hist_utils.h"

// need to load cmssw
#include <JetMETCorrections/Modules/interface/JetResolution.h>
#include <CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h>
#include <CondFormats/JetMETObjects/interface/JetCorrectorParameters.h>


void create_new_tree(std::string fin_name, TString fout_name="test_file.root", bool mc = true)
{
    std::cout << "File in : " << fin_name << std::endl;
    std::cout << "mc : " << mc << std::endl;

    bool reapplyJEC = false;

    TString l1Corr_data_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/condor/correction_files/Summer19UL17_RunF_V6_DATA_L1FastJet_AK4PFchs.txt";
    TString l2Corr_data_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/condor/correction_files/Summer19UL17_RunF_V6_DATA_L2Relative_AK4PFchs.txt";
    TString l3Corr_data_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/condor/correction_files/Summer19UL17_RunF_V6_DATA_L3Absolute_AK4PFchs.txt";
    TString resCorr_data_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/condor/correction_files/Summer19UL17_RunF_V6_DATA_L2L3Residual_AK4PFchs.txt";
    
    TString l1Corr_mc_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/condor/correction_files/Summer19UL17_V6_MC_L1FastJet_AK4PFchs.txt";
    TString l2Corr_mc_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/condor/correction_files/Summer19UL17_V6_MC_L2Relative_AK4PFchs.txt";
    TString l3Corr_mc_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/condor/correction_files/Summer19UL17_V6_MC_L3Absolute_AK4PFchs.txt";
    TString resCorr_mc_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/condor/correction_files/Summer19UL17_V6_MC_L2L3Residual_AK4PFchs.txt";
    
    TString jec_unc_mc_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/uncertanties/data/Summer19UL17_V5_MC_Uncertainty_AK4PFchs.txt";

    TString jer_mc_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/uncertanties/data/Summer19UL17_JRV3_MC_PtResolution_AK4PFchs.txt";
    TString jer_sf_mc_fname = "/home/llr/cms/kalipoliti/gitRepos/jetAnalysis/myMacros/C106X/uncertanties/data/Summer19UL17_JRV3_MC_SF_AK4PFchs.txt";

    // JER corrections 
    JME::JetResolution resolution = JME::JetResolution(jer_mc_fname.Data());
    JME::JetResolutionScaleFactor resolution_sf = JME::JetResolutionScaleFactor(jer_sf_mc_fname.Data());
    JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty(jec_unc_mc_fname.Data());  
    // ------------------------

    // JEC corrections
    JetCorrectorParameters *l1Par;
    JetCorrectorParameters *l2Par;
    JetCorrectorParameters *l3Par;
    JetCorrectorParameters *resPar;
    if (mc) {
        l1Par = new JetCorrectorParameters(l1Corr_mc_fname.Data());
        l2Par = new JetCorrectorParameters(l2Corr_mc_fname.Data());
        l3Par = new JetCorrectorParameters(l3Corr_mc_fname.Data());
        resPar = new JetCorrectorParameters(resCorr_mc_fname.Data());
    } else {
        l1Par = new JetCorrectorParameters(l1Corr_data_fname.Data());
        l2Par = new JetCorrectorParameters(l2Corr_data_fname.Data());
        l3Par = new JetCorrectorParameters(l3Corr_data_fname.Data());
        resPar = new JetCorrectorParameters(resCorr_data_fname.Data());
    }
    vector<JetCorrectorParameters> vPar;
    vPar.push_back(*l1Par);
    vPar.push_back(*l2Par);
    vPar.push_back(*l3Par);
    vPar.push_back(*resPar);

    FactorizedJetCorrector *JetCorrector = new FactorizedJetCorrector(vPar);
    // -------------------------

    TString rootf_out = "test_file.root";
    tTree t = tTree(fin_name);

    std::vector<TString> activeBranches_t = {"nref", "rho",
                                             "jtpt", "jtptCh", "jteta", "jtphi", "jtmB", "jtBpt", "rawpt", "jtarea",
                                             "sjt1Pt", "sjt1Eta", "sjt1Phi", "sjt1Y",
                                             "sjt2Pt", "sjt2Eta", "sjt2Phi", "sjt2Y",
                                             "discr_pfJP",
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "discr_particleNet_b", "discr_particleNet_bb", "discr_particleNet_BvsAll",
                                             "HLT_HIAK4CaloJet30_v1", "HLT_HIAK4CaloJet40_v1", "HLT_HIAK4CaloJet60_v1",
                                             "HLT_HIAK4CaloJet80_v1", "HLT_HIAK4CaloJet100_v1",
                                             "HLT_HIAK4PFJet30_v1", "HLT_HIAK4PFJet40_v1", "HLT_HIAK4PFJet60_v1",
                                             "HLT_HIAK4PFJet80_v1", "HLT_HIAK4PFJet100_v1",
                                             "HLT_HIL3Mu3_v1", "HLT_HIL3Mu5_v1", "HLT_HIL3Mu7_v1",
                                             "HLT_HIL3Mu5_AK4PFJet30_v1"
                                             };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    if (mc) {
        std::vector<TString> activeBranches_mc = {"weight", "pthat",
                                                  "jtHadFlav", "jtParFlav", "refparton_flavor",
                                                  "refpt", "refptCh", "refeta", "refmB", "jtNbHad", "jtNcHad", "refBpt",
                                                  "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", "rsjt1Y",
                                                  "rsjt2Pt", "rsjt2Eta", "rsjt2Phi", "rsjt2Y",
                                                 };
        t.SetBranchStatus(activeBranches_mc, 1);
    }

    // reco 
    Double_t jtpt_tree = -999.;
    Double_t jteta_tree = -999.;
    Double_t jtphi_tree = -999.;
    Double_t jtptCh_tree = -999.;
    Double_t logrg_tree = -999.; 
    Double_t logkt_tree = -999.; 
    Double_t zg_tree = -999.;
    Double_t mb_tree = -999.;
    Double_t bpt_tree = -999.;
    Double_t discr_b_tree = -999.;
    Double_t discr_lepb_tree = -999.;
    Double_t discr_bb_tree = -999.;
    Double_t discr_particleNet_b_tree = -999.;
    Double_t discr_particleNet_BvsAll_tree = -999.;
    Double_t discr_particleNet_bb_tree = -999.;
    Double_t discr_pfJP_tree = -999.;
    Int_t leading_tree = -1;
    

    // gen
    Double_t jtpt_gen_tree = -999.; 
    Double_t jteta_gen_tree = -999.; 
    Double_t jtphi_gen_tree = -999.; 
    Double_t jtptCh_gen_tree = -999.; 
    Double_t logrg_gen_tree = -999.; 
    Double_t logkt_gen_tree = -999.; 
    Double_t zg_gen_tree = -999.;
    Double_t mb_gen_tree = -999.;
    Double_t bpt_gen_tree = -999.;
    Int_t nb_gen_tree = -999;
    Int_t nc_gen_tree = -999;
    Int_t parton_flavor_tree = -999;
    Int_t refparton_flavor_tree = -999;

    Double_t weight_tree = -999.;
    Double_t pthat_tree = -999.;
    Double_t jer_sf_nom_tree = -999.;
    Double_t jer_sf_up_tree = -999.;
    Double_t jer_sf_down_tree = -999.;
    Double_t jec_unc_tree = -999.;

    // ---- Setup 
    double kt_cut = 1.; 
    // kt_cut = 2.225540928; // FOR CHARGED + NEUTRALS
    double jetR = 0.4;

    // double z1min = 60.;
    // double z1max = 160.;
    // double x1min = -0.6;
    // double x1max = 2.4;
    double logrg_underflow = -999.; // SD-untagged
    double zg_underflow = -999.; // SD-untagged

    // Create new tree to fill
    TFile *f_out = new TFile(fout_name, "recreate");
    TTree *tree_all = new TTree("tree_all", "");
    TTree *tree_sig = new TTree("tree_sig", "");
    TTree *tree_bkg_bb = new TTree("tree_bkg_bb", "");
    TTree *tree_bkg_rest = new TTree("tree_bkg_rest", "");
    TTree *tree_bkg_c = new TTree("tree_bkg_c", "");
    TTree *tree_bkg_l = new TTree("tree_bkg_l", "");
    TTree *tree_bkg_singlec = new TTree("tree_bkg_singlec", "");
    TTree *tree_bkg_cc = new TTree("tree_bkg_cc", "");

    for (TTree *tree : {tree_all, tree_sig, tree_bkg_bb, tree_bkg_rest, tree_bkg_c, tree_bkg_l, tree_bkg_singlec, tree_bkg_cc}) {
        tree->Branch("jtpt", &jtpt_tree, "jtpt/D");
        tree->Branch("jteta", &jteta_tree, "jteta/D");
        tree->Branch("jtphi", &jtphi_tree, "jtphi/D");
        tree->Branch("jtptCh", &jtptCh_tree, "jtptCh/D");
        tree->Branch("logrg", &logrg_tree, "logrg/D");
        tree->Branch("logkt", &logkt_tree, "logkt/D");
        tree->Branch("zg", &zg_tree, "zg/D");
        tree->Branch("mb", &mb_tree, "mb/D");
        tree->Branch("bpt", &bpt_tree, "bpt/D");
        tree->Branch("discr_b", &discr_b_tree, "discr_b/D");
        tree->Branch("discr_lepb", &discr_lepb_tree, "discr_lepb/D");
        tree->Branch("discr_bb", &discr_bb_tree, "discr_bb/D");
        tree->Branch("discr_particleNet_b", &discr_particleNet_b_tree, "discr_particleNet_b/D");
        tree->Branch("discr_particleNet_BvsAll", &discr_particleNet_BvsAll_tree, "discr_particleNet_BvsAll/D");
        tree->Branch("discr_particleNet_bb", &discr_particleNet_bb_tree, "discr_particleNet_bb/D");
        tree->Branch("discr_pfJP", &discr_pfJP_tree, "discr_pfJP/D");
        tree->Branch("jtpt_gen", &jtpt_gen_tree, "jtpt_gen/D");
        tree->Branch("jteta_gen", &jteta_gen_tree, "jteta_gen/D");
        tree->Branch("jtphi_gen", &jtphi_gen_tree, "jtphi_gen/D");
        tree->Branch("jtptCh_gen", &jtptCh_gen_tree, "jtptCh_gen/D");
        tree->Branch("logrg_gen", &logrg_gen_tree, "logrg_gen/D");
        tree->Branch("logkt_gen", &logkt_gen_tree, "logkt_gen/D");
        tree->Branch("zg_gen", &zg_gen_tree, "zg_gen/D");
        tree->Branch("mb_gen", &mb_gen_tree, "mb_gen/D");
        tree->Branch("bpt_gen", &bpt_gen_tree, "bpt_gen/D");
        tree->Branch("nb_gen", &nb_gen_tree, "nb_gen/I");
        tree->Branch("nc_gen", &nc_gen_tree, "nc_gen/I");
        tree->Branch("parton_flavor", &parton_flavor_tree, "parton_flavor/I");
        tree->Branch("refparton_flavor", &refparton_flavor_tree, "refparton_flavor/I");
        tree->Branch("weight", &weight_tree, "weight/D");
        tree->Branch("pthat", &pthat_tree, "pthat/D");
        tree->Branch("jer_sf_nom", &jer_sf_nom_tree, "jer_sf_nom/D");
        tree->Branch("jer_sf_up", &jer_sf_up_tree, "jer_sf_up/D");
        tree->Branch("jer_sf_down", &jer_sf_down_tree, "jer_sf_down/D");
        tree->Branch("jec_unc", &jec_unc_tree, "jec_unc/D");
        tree->Branch("leading", &leading_tree, "leading/I");
    }

    Long64_t total_events = 2000000000;
    // Long64_t total_events = 100000;
    Long64_t nentries = t.GetEntries();
    if (total_events > nentries) total_events = nentries;

    for (Long64_t ient = 0; ient < total_events; ient++) {
        // Print progress
        if (ient % 10000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        t.GetEntry(ient);
        weight_tree = 1.;
        pthat_tree = -1.;

        double prescale_pf40 = 33.917210;
        double prescale_pf30 = 153.12145;
        double prescale_pf60 = 6.3534438;
        double prescale_calo40 = 29.1544;
        double prescale_calo30 = 152.29134;
        double prescale_calo60 = 6.3214193;

        bool pass100 = (t.HLT_HIAK4PFJet100_v1==1);
        bool pass80 = (t.HLT_HIAK4PFJet80_v1==1);
        bool pass60 = (t.HLT_HIAK4PFJet60_v1==1);
        bool pass40 = (t.HLT_HIAK4PFJet40_v1==1);
        bool pass30 = (t.HLT_HIAK4PFJet30_v1==1);

        // bool pass100 = (t.HLT_HIAK4CaloJet100_v1==1);
        // bool pass80 = (t.HLT_HIAK4CaloJet80_v1==1);
        // bool pass60 = (t.HLT_HIAK4CaloJet60_v1==1);
        // bool pass40 = (t.HLT_HIAK4CaloJet40_v1==1);
        // bool pass30 = (t.HLT_HIAK4CaloJet30_v1==1);

        // bool passMu = (t.HLT_HIL3Mu5_AK4PFJet30_v1==1);
        bool passMu = (t.HLT_HIL3Mu5_v1==1);

        if (TString(fin_name).Contains("HighEGJet")) {
            // if (pass80 || pass100) weight_tree = 1.;
            // if (pass80) weight_tree = 1.;
            if (passMu) weight_tree = 1.;
            else continue;
            // std::cout << "in" << std::endl;
        } else if (TString(fin_name).Contains("LowEGJet")) {
            if (pass80 || pass100) continue;
            else if (pass60) weight_tree = 1.;
            // else if (pass60) weight_tree = prescale_pf60;
            else if (pass40) weight_tree = prescale_pf40;
            // else if (pass40) weight_tree = 1.; // FOR TRIGGER TURN-ON
            // else if (pass30) weight_tree = prescale_pf30; // FOR TRIGGER TURN-ON
            // else if (pass30) weight_tree = prescale_calo30; // FOR TRIGGER TURN-ON
            else continue;
        } else if (TString(fin_name).Contains("SingleMuon")) {
            if (passMu) weight_tree = 1.;
            else continue;
        } else if (mc) {
            if (pass40) weight_tree = t.weight;
            // if (pass60) weight_tree = t.weight;
            // if (passMu) weight_tree = t.weight;
            else continue;
            weight_tree = t.weight;
            pthat_tree = t.pthat;
            
        } else continue;

        // double rho = t.rho;
        for (Int_t ijet = 0; ijet < t.nref; ijet++) {
            double jteta = t.jteta[ijet];
            double rawpt = t.rawpt[ijet];
            double jtpt = t.jtpt[ijet];
            double jtphi = t.jtphi[ijet];
            double jtarea = t.jtarea[ijet];

            // jet selection 
            if (std::abs(jteta) > 2) continue;

            // ---- Calculate everything for reco 
            // if (reapplyJEC) {
            //     JetCorrector->setJetPt(rawpt); 
            //     JetCorrector->setJetEta(jteta);
            //     JetCorrector->setJetA(jtarea);
            //     JetCorrector->setRho(rho);
            //     double newCorrection = JetCorrector->getCorrection();
            //     jtpt = rawpt * newCorrection;
            // }

            if (ijet==0) leading_tree = 1;
            else leading_tree = 0;

            jtpt_tree = jtpt;
            jteta_tree = jteta;
            jtphi_tree = jtphi;

            jtptCh_tree = t.jtptCh[ijet];
            mb_tree = t.jtmB[ijet];
            bpt_tree = t.jtBpt[ijet];

            discr_b_tree = t.discr_deepFlavour_b[ijet];
            discr_lepb_tree = t.discr_deepFlavour_lepb[ijet];
            discr_bb_tree = t.discr_deepFlavour_bb[ijet];

            discr_particleNet_b_tree = t.discr_particleNet_b[ijet];
            discr_particleNet_BvsAll_tree = t.discr_particleNet_BvsAll[ijet];
            discr_particleNet_bb_tree = t.discr_particleNet_bb[ijet];

            discr_pfJP_tree = t.discr_pfJP[ijet];

            parton_flavor_tree = t.jtParFlav[ijet];
            refparton_flavor_tree = t.refparton_flavor[ijet];

            // Initialize observables
            Float_t rg = -1.;
            Float_t kt = -1.;
            Float_t zg = -1.;

            Float_t logrg = -999.;
            Float_t logkt = -999.;

            // Calculate rg, kt only for 2 prong jets
            if (t.sjt2Pt[ijet] > 0.) {
                rg = calc_rg(t.sjt1Y[ijet], t.sjt1Phi[ijet], t.sjt2Y[ijet], t.sjt2Phi[ijet]);
                kt = t.sjt2Pt[ijet] * rg;
                zg = t.sjt2Pt[ijet] / (t.sjt1Pt[ijet] + t.sjt2Pt[ijet]);
                
                // calculate logs
                logrg = std::log(jetR/rg);
                logkt = std::log(kt);

                // if (logrg < 0.) logrg = -999.;

                // kt cut -- EDIT: can be applied at the time of filling the histograms
                // if (kt < kt_cut) {
                //     logrg = logrg_underflow;
                //     zg = zg_underflow;
                // }
            } else {
                // 1-prong jets
                logrg = logrg_underflow; 
                zg = zg_underflow;
            }

            logrg_tree = logrg;
            logkt_tree = logkt;
            zg_tree = zg;

            // ---- Calculate everything for gen 

            Float_t rg_gen = -1.;
            Float_t kt_gen = -1.;
            Float_t zg_gen = -1.;

            Float_t logrg_gen = -999.;
            Float_t logkt_gen = -999.;

            if (mc) {
                jtpt_gen_tree = t.refpt[ijet];
                jteta_gen_tree = t.refeta[ijet];
                jtphi_gen_tree = t.refphi[ijet];
                jtptCh_gen_tree = t.refptCh[ijet];
                mb_gen_tree = t.refmB[ijet];
                bpt_gen_tree = t.refBpt[ijet];

                nb_gen_tree = t.jtNbHad[ijet];
                nc_gen_tree = t.jtNcHad[ijet];

                JME::JetParameters parameters_1;
                parameters_1.setJetPt(jtpt_tree);
                parameters_1.setJetEta(jteta_tree);
                parameters_1.setRho(0.);

                double sf;
                double sim_resolution = resolution.getResolution(parameters_1);

                sf = resolution_sf.getScaleFactor(parameters_1, Variation::NOMINAL);
                if (jtpt_gen_tree > 0 ) sf = 1+(sf-1)*(jtpt_tree-jtpt_gen_tree)/(jtpt_tree); // det-level jet is matched to truth-level jet
  	            else sf = 1+gRandom->Gaus(0,sim_resolution)*std::sqrt(sf*sf-1); // “stochastic” smearing, which is what you will probably use for HIN
                jer_sf_nom_tree = sf;

                sf = resolution_sf.getScaleFactor(parameters_1, Variation::UP);
                if (jtpt_gen_tree > 0 ) sf = 1+(sf-1)*(jtpt_tree-jtpt_gen_tree)/(jtpt_tree); // det-level jet is matched to truth-level jet
  	            else sf = 1+gRandom->Gaus(0,sim_resolution)*std::sqrt(sf*sf-1); // “stochastic” smearing, which is what you will probably use for HIN
                jer_sf_up_tree = sf;

                sf = resolution_sf.getScaleFactor(parameters_1, Variation::DOWN);
                if (jtpt_gen_tree > 0 ) sf = 1+(sf-1)*(jtpt_tree-jtpt_gen_tree)/(jtpt_tree); // det-level jet is matched to truth-level jet
  	            else sf = 1+gRandom->Gaus(0,sim_resolution)*std::sqrt(sf*sf-1); // “stochastic” smearing, which is what you will probably use for HIN
                jer_sf_down_tree = sf;

                // // Get JEC uncertainty
                jecUnc->setJetEta(jteta_tree);
                jecUnc->setJetPt(jtpt_tree);
                double unc = jecUnc->getUncertainty(true); //this returns the uncertainty on the correction, it should be of order 1-2%
                // //  Vary correction with newCorrection=1+factor*unc; jtpt=jtpt*newCorrection; where factor=0(nom) or +-1(up,down)
                jec_unc_tree = unc;

                if (t.rsjt2Pt[ijet] > 0.) {
                    rg_gen = calc_rg(t.rsjt1Y[ijet], t.rsjt1Phi[ijet], t.rsjt2Y[ijet], t.rsjt2Phi[ijet]);
                    kt_gen = t.rsjt2Pt[ijet] * rg_gen;
                    zg_gen = t.rsjt2Pt[ijet] / (t.rsjt1Pt[ijet] + t.rsjt2Pt[ijet]);
                    
                    // calculate logs
                    logrg_gen = std::log(jetR/rg_gen);
                    logkt_gen = std::log(kt_gen);

                    // kt cut -- EDIT: can be applied at the time of filling the histograms
                    // if (kt_gen < kt_cut) {
                    //     logrg_gen = logrg_underflow;
                    //     zg_gen = zg_underflow;
                    // }
                } else {
                    // 1-prong jets
                    logrg_gen = logrg_underflow; 
                    zg_gen = zg_underflow;
                }

                logrg_gen_tree = logrg_gen;
                logkt_gen_tree = logkt_gen;
                zg_gen_tree = zg_gen;
            }

            // ---- Fill histograms

            tree_all->Fill();

            if (!mc) continue;

            if (nb_gen_tree == 1)
                tree_sig->Fill();
            else if (nb_gen_tree > 1)
                tree_bkg_bb->Fill();
            else if (nc_gen_tree == 1)
                tree_bkg_singlec->Fill();
            else if (nc_gen_tree > 1)
                tree_bkg_cc->Fill();
            else {
                tree_bkg_l->Fill();
            }

            if (nb_gen_tree==0) tree_bkg_rest->Fill();
                
            
        } // end jet loop       
    } // end entries loop
    tree_all->Write("", TObject::kOverwrite);

    if (mc) {
        tree_sig->Write("", TObject::kOverwrite);
        tree_bkg_bb->Write("", TObject::kOverwrite);
        tree_bkg_rest->Write("", TObject::kOverwrite);
        tree_bkg_c->Write("", TObject::kOverwrite);
        tree_bkg_l->Write("", TObject::kOverwrite);
        tree_bkg_singlec->Write("", TObject::kOverwrite);
        tree_bkg_cc->Write("", TObject::kOverwrite);
    }
    
    f_out->Close();
    gApplication->Terminate();
}