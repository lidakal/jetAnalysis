#include <TTree.h>
#include <TFile.h>
#include <TRandom.h>
#include <TApplication.h>

#include "tTree.h"
#include "hist_utils.h"

// need to load cmssw
#include <JetMETCorrections/Modules/interface/JetResolution.h>
#include <CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h>

# define M_PI           3.14159265358979323846  /* pi */

void create_new_tree_btagSF(std::string fin_name, TString fout_name="test_file.root", bool mc = false, bool inclusive = true)
{
    std::cout << "File in : " << fin_name << std::endl;
    std::cout << "mc : " << mc << std::endl;
    std::cout << "inclusive : " << inclusive << std::endl;

    TString rootf_out = "test_file.root";
    tTree t = tTree(fin_name);

    std::vector<TString> activeBranches_t = {"nref",
                                             "jtpt", "jtptCh", "jteta", "jtphi", "jtmB", "jtBpt",
                                             "sjt1Pt", "sjt1Eta", "sjt1Phi", "sjt1Y",
                                             "sjt2Pt", "sjt2Eta", "sjt2Phi", "sjt2Y",
                                             "discr_pfJP",
                                             "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                             "discr_particleNet_b", "discr_particleNet_bb", "discr_particleNet_BvsAll",
                                             "HLT_HIAK4CaloJet30_v1", "HLT_HIAK4CaloJet40_v1", "HLT_HIAK4CaloJet60_v1",
                                             "HLT_HIAK4CaloJet80_v1", "HLT_HIAK4CaloJet100_v1"
                                             };
    t.SetBranchStatus("*", 0);
    t.SetBranchStatus(activeBranches_t, 1);

    if (mc) {
        std::vector<TString> activeBranches_mc = {"weight", 
                                                  "refpt", "refptCh", "refeta", "refmB", "jtNbHad", "jtNcHad", "refBpt",
                                                  "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", "rsjt1Y",
                                                  "rsjt2Pt", "rsjt2Eta", "rsjt2Phi", "rsjt2Y",
                                                  "jer_sf_nom", "jer_sf_up", "jer_sf_down", "jec_unc"
                                                 };
        t.SetBranchStatus(activeBranches_mc, 1);
    }

    // reco 
    Double_t jtpt_tree = -999.;
    Double_t jteta_tree = -999.;
    Double_t jtphi_tree = -999.;
    Double_t jtptCh_tree = -999.;
    Double_t logrg_tree = -999.; 
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
    

    // gen
    Double_t jtpt_gen_tree = -999.; 
    Double_t jteta_gen_tree = -999.; 
    Double_t jtphi_gen_tree = -999.; 
    Double_t jtptCh_gen_tree = -999.; 
    Double_t logrg_gen_tree = -999.; 
    Double_t zg_gen_tree = -999.;
    Double_t mb_gen_tree = -999.;
    Double_t bpt_gen_tree = -999.;
    Int_t nb_gen_tree = -999;
    Int_t nc_gen_tree = -999;

    Double_t weight_tree = -999.;
    Double_t jer_sf_nom_tree = -999.;
    Double_t jer_sf_up_tree = -999.;
    Double_t jer_sf_down_tree = -999.;
    Double_t jec_unc_tree = -999.;

    // ---- Setup 
    double kt_cut = 1.;
    double btag_cut = 0.7;
    double jetR = 0.4;

    // double z1min = 60.;
    // double z1max = 160.;
    // double x1min = -0.6;
    // double x1max = 2.4;
    double logrg_underflow = -0.2;
    double zg_underflow = 0.;

    // Create new tree to fill
    TFile *f_out = new TFile(fout_name, "recreate");
    TTree *tree_all = new TTree("tree_all", "");
    tree_all->Branch("jtpt", &jtpt_tree, "jtpt/D");
    tree_all->Branch("jteta", &jteta_tree, "jteta/D");
    tree_all->Branch("jtphi", &jtphi_tree, "jtphi/D");
    tree_all->Branch("jtptCh", &jtptCh_tree, "jtptCh/D");
    tree_all->Branch("logrg", &logrg_tree, "logrg/D");
    tree_all->Branch("zg", &zg_tree, "zg/D");
    tree_all->Branch("mb", &mb_tree, "mb/D");
    tree_all->Branch("bpt", &bpt_tree, "bpt/D");
    tree_all->Branch("discr_b", &discr_b_tree, "discr_b/D");
    tree_all->Branch("discr_lepb", &discr_lepb_tree, "discr_lepb/D");
    tree_all->Branch("discr_bb", &discr_bb_tree, "discr_bb/D");
    tree_all->Branch("discr_particleNet_b", &discr_particleNet_b_tree, "discr_particleNet_b/D");
    tree_all->Branch("discr_particleNet_BvsAll", &discr_particleNet_BvsAll_tree, "discr_particleNet_BvsAll/D");
    tree_all->Branch("discr_particleNet_bb", &discr_particleNet_bb_tree, "discr_particleNet_bb/D");
    tree_all->Branch("discr_pfJP", &discr_pfJP_tree, "discr_pfJP/D");
    tree_all->Branch("jtpt_gen", &jtpt_gen_tree, "jtpt_gen/D");
    tree_all->Branch("jteta_gen", &jteta_gen_tree, "jteta_gen/D");
    tree_all->Branch("jtphi_gen", &jtphi_gen_tree, "jtphi_gen/D");
    tree_all->Branch("jtptCh_gen", &jtptCh_gen_tree, "jtptCh_gen/D");
    tree_all->Branch("logrg_gen", &logrg_gen_tree, "logrg_gen/D");
    tree_all->Branch("zg_gen", &zg_gen_tree, "zg_gen/D");
    tree_all->Branch("mb_gen", &mb_gen_tree, "mb_gen/D");
    tree_all->Branch("bpt_gen", &bpt_gen_tree, "bpt_gen/D");
    tree_all->Branch("nb_gen", &nb_gen_tree, "nb_gen/I");
    tree_all->Branch("nc_gen", &nc_gen_tree, "nc_gen/I");
    tree_all->Branch("weight", &weight_tree, "weight/D");
    tree_all->Branch("jer_sf_nom", &jer_sf_nom_tree, "jer_sf_nom/D");
    tree_all->Branch("jer_sf_up", &jer_sf_up_tree, "jer_sf_up/D");
    tree_all->Branch("jer_sf_down", &jer_sf_down_tree, "jer_sf_down/D");
    tree_all->Branch("jec_unc", &jec_unc_tree, "jec_unc/D");

    TTree *tree_sig = new TTree("tree_sig", "");
    tree_sig->Branch("jtpt", &jtpt_tree, "jtpt/D");
    tree_sig->Branch("jteta", &jteta_tree, "jteta/D");
    tree_sig->Branch("jtphi", &jtphi_tree, "jtphi/D");
    tree_sig->Branch("jtptCh", &jtptCh_tree, "jtptCh/D");
    tree_sig->Branch("logrg", &logrg_tree, "logrg/D");
    tree_sig->Branch("zg", &zg_tree, "zg/D");
    tree_sig->Branch("mb", &mb_tree, "mb/D");
    tree_sig->Branch("bpt", &bpt_tree, "bpt/D");
    tree_sig->Branch("discr_b", &discr_b_tree, "discr_b/D");
    tree_sig->Branch("discr_lepb", &discr_lepb_tree, "discr_lepb/D");
    tree_sig->Branch("discr_bb", &discr_bb_tree, "discr_bb/D");
    tree_sig->Branch("discr_particleNet_b", &discr_particleNet_b_tree, "discr_particleNet_b/D");
    tree_sig->Branch("discr_particleNet_BvsAll", &discr_particleNet_BvsAll_tree, "discr_particleNet_BvsAll/D");
    tree_sig->Branch("discr_particleNet_bb", &discr_particleNet_bb_tree, "discr_particleNet_bb/D");
    tree_sig->Branch("discr_pfJP", &discr_pfJP_tree, "discr_pfJP/D");
    tree_sig->Branch("jtpt_gen", &jtpt_gen_tree, "jtpt_gen/D");
    tree_sig->Branch("jteta_gen", &jteta_gen_tree, "jteta_gen/D");
    tree_sig->Branch("jtphi_gen", &jtphi_gen_tree, "jtphi_gen/D");
    tree_sig->Branch("jtptCh_gen", &jtptCh_gen_tree, "jtptCh_gen/D");
    tree_sig->Branch("logrg_gen", &logrg_gen_tree, "logrg_gen/D");
    tree_sig->Branch("zg_gen", &zg_gen_tree, "zg_gen/D");
    tree_sig->Branch("mb_gen", &mb_gen_tree, "mb_gen/D");
    tree_sig->Branch("bpt_gen", &bpt_gen_tree, "bpt_gen/D");
    tree_sig->Branch("nb_gen", &nb_gen_tree, "nb_gen/I");
    tree_sig->Branch("nc_gen", &nc_gen_tree, "nc_gen/I");
    tree_sig->Branch("weight", &weight_tree, "weight/D");
    tree_sig->Branch("jer_sf_nom", &jer_sf_nom_tree, "jer_sf_nom/D");
    tree_sig->Branch("jer_sf_up", &jer_sf_up_tree, "jer_sf_up/D");
    tree_sig->Branch("jer_sf_down", &jer_sf_down_tree, "jer_sf_down/D");
    tree_sig->Branch("jec_unc", &jec_unc_tree, "jec_unc/D");

    TTree *tree_bkg_bb = new TTree("tree_bkg_bb", "");
    tree_bkg_bb->Branch("jtpt", &jtpt_tree, "jtpt/D");
    tree_bkg_bb->Branch("jteta", &jteta_tree, "jteta/D");
    tree_bkg_bb->Branch("jtphi", &jtphi_tree, "jtphi/D");
    tree_bkg_bb->Branch("jtptCh", &jtptCh_tree, "jtptCh/D");
    tree_bkg_bb->Branch("logrg", &logrg_tree, "logrg/D");
    tree_bkg_bb->Branch("zg", &zg_tree, "zg/D");
    tree_bkg_bb->Branch("mb", &mb_tree, "mb/D");
    tree_bkg_bb->Branch("bpt", &bpt_tree, "bpt/D");
    tree_bkg_bb->Branch("discr_b", &discr_b_tree, "discr_b/D");
    tree_bkg_bb->Branch("discr_lepb", &discr_lepb_tree, "discr_lepb/D");
    tree_bkg_bb->Branch("discr_bb", &discr_bb_tree, "discr_bb/D");
    tree_bkg_bb->Branch("discr_particleNet_b", &discr_particleNet_b_tree, "discr_particleNet_b/D");
    tree_bkg_bb->Branch("discr_particleNet_BvsAll", &discr_particleNet_BvsAll_tree, "discr_particleNet_BvsAll/D");
    tree_bkg_bb->Branch("discr_particleNet_bb", &discr_particleNet_bb_tree, "discr_particleNet_bb/D");
    tree_bkg_bb->Branch("discr_pfJP", &discr_pfJP_tree, "discr_pfJP/D");
    tree_bkg_bb->Branch("jtpt_gen", &jtpt_gen_tree, "jtpt_gen/D");
    tree_bkg_bb->Branch("jteta_gen", &jteta_gen_tree, "jteta_gen/D");
    tree_bkg_bb->Branch("jtphi_gen", &jtphi_gen_tree, "jtphi_gen/D");
    tree_bkg_bb->Branch("jtptCh_gen", &jtptCh_gen_tree, "jtptCh_gen/D");
    tree_bkg_bb->Branch("logrg_gen", &logrg_gen_tree, "logrg_gen/D");
    tree_bkg_bb->Branch("zg_gen", &zg_gen_tree, "zg_gen/D");
    tree_bkg_bb->Branch("mb_gen", &mb_gen_tree, "mb_gen/D");
    tree_bkg_bb->Branch("bpt_gen", &bpt_gen_tree, "bpt_gen/D");
    tree_bkg_bb->Branch("nb_gen", &nb_gen_tree, "nb_gen/I");
    tree_bkg_bb->Branch("nc_gen", &nc_gen_tree, "nc_gen/I");
    tree_bkg_bb->Branch("weight", &weight_tree, "weight/D");
    tree_bkg_bb->Branch("jer_sf_nom", &jer_sf_nom_tree, "jer_sf_nom/D");
    tree_bkg_bb->Branch("jer_sf_up", &jer_sf_up_tree, "jer_sf_up/D");
    tree_bkg_bb->Branch("jer_sf_down", &jer_sf_down_tree, "jer_sf_down/D");
    tree_bkg_bb->Branch("jec_unc", &jec_unc_tree, "jec_unc/D");

    TTree *tree_bkg_rest = new TTree("tree_bkg_rest", "");
    tree_bkg_rest->Branch("jtpt", &jtpt_tree, "jtpt/D");
    tree_bkg_rest->Branch("jteta", &jteta_tree, "jteta/D");
    tree_bkg_rest->Branch("jtphi", &jtphi_tree, "jtphi/D");
    tree_bkg_rest->Branch("jtptCh", &jtptCh_tree, "jtptCh/D");
    tree_bkg_rest->Branch("logrg", &logrg_tree, "logrg/D");
    tree_bkg_rest->Branch("zg", &zg_tree, "zg/D");
    tree_bkg_rest->Branch("mb", &mb_tree, "mb/D");
    tree_bkg_rest->Branch("bpt", &bpt_tree, "bpt/D");
    tree_bkg_rest->Branch("discr_b", &discr_b_tree, "discr_b/D");
    tree_bkg_rest->Branch("discr_lepb", &discr_lepb_tree, "discr_lepb/D");
    tree_bkg_rest->Branch("discr_bb", &discr_bb_tree, "discr_bb/D");
    tree_bkg_rest->Branch("discr_particleNet_b", &discr_particleNet_b_tree, "discr_particleNet_b/D");
    tree_bkg_rest->Branch("discr_particleNet_BvsAll", &discr_particleNet_BvsAll_tree, "discr_particleNet_BvsAll/D");
    tree_bkg_rest->Branch("discr_particleNet_bb", &discr_particleNet_bb_tree, "discr_particleNet_bb/D");
    tree_bkg_rest->Branch("discr_pfJP", &discr_pfJP_tree, "discr_pfJP/D");
    tree_bkg_rest->Branch("jtpt_gen", &jtpt_gen_tree, "jtpt_gen/D");
    tree_bkg_rest->Branch("jteta_gen", &jteta_gen_tree, "jteta_gen/D");
    tree_bkg_rest->Branch("jtphi_gen", &jtphi_gen_tree, "jtphi_gen/D");
    tree_bkg_rest->Branch("jtptCh_gen", &jtptCh_gen_tree, "jtptCh_gen/D");
    tree_bkg_rest->Branch("logrg_gen", &logrg_gen_tree, "logrg_gen/D");
    tree_bkg_rest->Branch("zg_gen", &zg_gen_tree, "zg_gen/D");
    tree_bkg_rest->Branch("mb_gen", &mb_gen_tree, "mb_gen/D");
    tree_bkg_rest->Branch("bpt_gen", &bpt_gen_tree, "bpt_gen/D");
    tree_bkg_rest->Branch("nb_gen", &nb_gen_tree, "nb_gen/I");
    tree_bkg_rest->Branch("nc_gen", &nc_gen_tree, "nc_gen/I");
    tree_bkg_rest->Branch("weight", &weight_tree, "weight/D");
    tree_bkg_rest->Branch("jer_sf_nom", &jer_sf_nom_tree, "jer_sf_nom/D");
    tree_bkg_rest->Branch("jer_sf_up", &jer_sf_up_tree, "jer_sf_up/D");
    tree_bkg_rest->Branch("jer_sf_down", &jer_sf_down_tree, "jer_sf_down/D");
    tree_bkg_rest->Branch("jec_unc", &jec_unc_tree, "jec_unc/D");

    TTree *tree_bkg_c = new TTree("tree_bkg_c", "");
    tree_bkg_c->Branch("jtpt", &jtpt_tree, "jtpt/D");
    tree_bkg_c->Branch("jteta", &jteta_tree, "jteta/D");
    tree_bkg_c->Branch("jtphi", &jtphi_tree, "jtphi/D");
    tree_bkg_c->Branch("jtptCh", &jtptCh_tree, "jtptCh/D");
    tree_bkg_c->Branch("logrg", &logrg_tree, "logrg/D");
    tree_bkg_c->Branch("zg", &zg_tree, "zg/D");
    tree_bkg_c->Branch("mb", &mb_tree, "mb/D");
    tree_bkg_c->Branch("bpt", &bpt_tree, "bpt/D");
    tree_bkg_c->Branch("discr_b", &discr_b_tree, "discr_b/D");
    tree_bkg_c->Branch("discr_lepb", &discr_lepb_tree, "discr_lepb/D");
    tree_bkg_c->Branch("discr_bb", &discr_bb_tree, "discr_bb/D");
    tree_bkg_c->Branch("discr_particleNet_b", &discr_particleNet_b_tree, "discr_particleNet_b/D");
    tree_bkg_c->Branch("discr_particleNet_BvsAll", &discr_particleNet_BvsAll_tree, "discr_particleNet_BvsAll/D");
    tree_bkg_c->Branch("discr_particleNet_bb", &discr_particleNet_bb_tree, "discr_particleNet_bb/D");
    tree_bkg_c->Branch("discr_pfJP", &discr_pfJP_tree, "discr_pfJP/D");
    tree_bkg_c->Branch("jtpt_gen", &jtpt_gen_tree, "jtpt_gen/D");
    tree_bkg_c->Branch("jteta_gen", &jteta_gen_tree, "jteta_gen/D");
    tree_bkg_c->Branch("jtphi_gen", &jtphi_gen_tree, "jtphi_gen/D");
    tree_bkg_c->Branch("jtptCh_gen", &jtptCh_gen_tree, "jtptCh_gen/D");
    tree_bkg_c->Branch("logrg_gen", &logrg_gen_tree, "logrg_gen/D");
    tree_bkg_c->Branch("zg_gen", &zg_gen_tree, "zg_gen/D");
    tree_bkg_c->Branch("mb_gen", &mb_gen_tree, "mb_gen/D");
    tree_bkg_c->Branch("bpt_gen", &bpt_gen_tree, "bpt_gen/D");
    tree_bkg_c->Branch("nb_gen", &nb_gen_tree, "nb_gen/I");
    tree_bkg_c->Branch("nc_gen", &nc_gen_tree, "nc_gen/I");
    tree_bkg_c->Branch("weight", &weight_tree, "weight/D");
    tree_bkg_c->Branch("jer_sf_nom", &jer_sf_nom_tree, "jer_sf_nom/D");
    tree_bkg_c->Branch("jer_sf_up", &jer_sf_up_tree, "jer_sf_up/D");
    tree_bkg_c->Branch("jer_sf_down", &jer_sf_down_tree, "jer_sf_down/D");
    tree_bkg_c->Branch("jec_unc", &jec_unc_tree, "jec_unc/D");

    TTree *tree_bkg_l = new TTree("tree_bkg_l", "");
    tree_bkg_l->Branch("jtpt", &jtpt_tree, "jtpt/D");
    tree_bkg_l->Branch("jteta", &jteta_tree, "jteta/D");
    tree_bkg_l->Branch("jtphi", &jtphi_tree, "jtphi/D");
    tree_bkg_l->Branch("jtptCh", &jtptCh_tree, "jtptCh/D");
    tree_bkg_l->Branch("logrg", &logrg_tree, "logrg/D");
    tree_bkg_l->Branch("zg", &zg_tree, "zg/D");
    tree_bkg_l->Branch("mb", &mb_tree, "mb/D");
    tree_bkg_l->Branch("bpt", &bpt_tree, "bpt/D");
    tree_bkg_l->Branch("discr_b", &discr_b_tree, "discr_b/D");
    tree_bkg_l->Branch("discr_lepb", &discr_lepb_tree, "discr_lepb/D");
    tree_bkg_l->Branch("discr_bb", &discr_bb_tree, "discr_bb/D");
    tree_bkg_l->Branch("discr_particleNet_b", &discr_particleNet_b_tree, "discr_particleNet_b/D");
    tree_bkg_l->Branch("discr_particleNet_BvsAll", &discr_particleNet_BvsAll_tree, "discr_particleNet_BvsAll/D");
    tree_bkg_l->Branch("discr_particleNet_bb", &discr_particleNet_bb_tree, "discr_particleNet_bb/D");
    tree_bkg_l->Branch("discr_pfJP", &discr_pfJP_tree, "discr_pfJP/D");
    tree_bkg_l->Branch("jtpt_gen", &jtpt_gen_tree, "jtpt_gen/D");
    tree_bkg_l->Branch("jteta_gen", &jteta_gen_tree, "jteta_gen/D");
    tree_bkg_l->Branch("jtphi_gen", &jtphi_gen_tree, "jtphi_gen/D");
    tree_bkg_l->Branch("jtptCh_gen", &jtptCh_gen_tree, "jtptCh_gen/D");
    tree_bkg_l->Branch("logrg_gen", &logrg_gen_tree, "logrg_gen/D");
    tree_bkg_l->Branch("zg_gen", &zg_gen_tree, "zg_gen/D");
    tree_bkg_l->Branch("mb_gen", &mb_gen_tree, "mb_gen/D");
    tree_bkg_l->Branch("bpt_gen", &bpt_gen_tree, "bpt_gen/D");
    tree_bkg_l->Branch("nb_gen", &nb_gen_tree, "nb_gen/I");
    tree_bkg_l->Branch("nc_gen", &nc_gen_tree, "nc_gen/I");
    tree_bkg_l->Branch("weight", &weight_tree, "weight/D");
    tree_bkg_l->Branch("jer_sf_nom", &jer_sf_nom_tree, "jer_sf_nom/D");
    tree_bkg_l->Branch("jer_sf_up", &jer_sf_up_tree, "jer_sf_up/D");
    tree_bkg_l->Branch("jer_sf_down", &jer_sf_down_tree, "jer_sf_down/D");
    tree_bkg_l->Branch("jec_unc", &jec_unc_tree, "jec_unc/D");

    Long64_t total_events = 20000000000;
    Long64_t nentries = t.GetEntries();
    if (total_events > nentries) total_events = nentries;

    for (Long64_t ient = 0; ient < total_events; ient++) {
        // Print progress
        if (ient % 10000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        t.GetEntry(ient);
        weight_tree = 1.;
        const double weight_prescale = 6.32142; // From HighEGJet, to be applied to LowEGJet

        // event selection
        // if 80 weight = 1
        // if 60 and not 80 weight = prescale 
        // else continue 

        if (t.HLT_HIAK4CaloJet80_v1 == 1 && TString(fin_name).Contains("HighEGJet")) {
            weight_tree = 1.;
        } else if (t.HLT_HIAK4CaloJet60_v1 == 1 && t.HLT_HIAK4CaloJet80_v1 != 1 && TString(fin_name).Contains("LowEGJet")) {
            weight_tree = weight_prescale; 
        } else if (!mc) {
            continue;
        }
        // if it's MC, the weight will be fixed later

        // JET SELECTION FOR BTAG SF 
            
        // CASE 1 - recoil off of b tagged jets
        std::vector<int> btagged_jets = {};
        for (int ijet = 0; ijet < t.nref; ijet++) {
            if (t.discr_particleNet_BvsAll[ijet]<0.2 || t.jtpt[ijet] < 80) continue;
            btagged_jets.push_back(ijet);
        }
            
        int nbtagged_jets = btagged_jets.size();
        if (nbtagged_jets <= 0) continue; // no b tagged jets, next event

        // Select a b tagged jet at random
        const double range_from = 0;
        const double range_to = 1;
        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());
        std::uniform_real_distribution<double> distr(range_from, range_to);
        double rand_uniform = distr(generator); // random number between 0 and 1

        int ijet_btag = 0; // which jet from the vector
        for (; ijet_btag < nbtagged_jets; ijet_btag++) {
            if (rand_uniform < double(ijet_btag+1)/nbtagged_jets) break;
        }
        
        ijet_btag = btagged_jets[ijet_btag]; // which jet from the event

        // Add to new tree all recoil jets
        for (Int_t ijet = 0; ijet < t.nref; ijet++) {
            if (ijet==ijet_btag) continue;
            if (std::abs(t.jteta[ijet]) > 2) continue;

            // Skip dphi<2pi/3 (not recoil)
            ROOT::Math::PtEtaPhiMVector v1;
            v1.SetPhi(t.jtphi[ijet_btag]);

            ROOT::Math::PtEtaPhiMVector v2;
            v2.SetPhi(t.jtphi[ijet]);

            double dphi = ROOT::Math::VectorUtil::DeltaPhi(v1, v2);
            if (dphi<2*M_PI/3) continue;
            
            // ---- Calculate everything for reco 

            jtpt_tree = t.jtpt[ijet];
            jteta_tree = t.jteta[ijet];
            jtphi_tree = t.jtphi[ijet];
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

                if (logrg < 0.) logrg = -999.;

                // kt cut 
                if (kt < kt_cut) {
                    logrg = logrg_underflow;
                    zg = zg_underflow;
                }
            } else {
                // 1-prong jets
                logrg = logrg_underflow; 
                zg = zg_underflow;
            }

            logrg_tree = logrg;
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

                jer_sf_nom_tree = t.jer_sf_nom[ijet];
                jer_sf_up_tree = t.jer_sf_up[ijet];
                jer_sf_down_tree = t.jer_sf_down[ijet];
                jec_unc_tree = t.jec_unc[ijet];

                if (t.rsjt2Pt[ijet] > 0.) {
                    rg_gen = calc_rg(t.rsjt1Y[ijet], t.rsjt1Phi[ijet], t.rsjt2Y[ijet], t.rsjt2Phi[ijet]);
                    kt_gen = t.rsjt2Pt[ijet] * rg_gen;
                    zg_gen = t.rsjt2Pt[ijet] / (t.rsjt1Pt[ijet] + t.rsjt2Pt[ijet]);
                    
                    // calculate logs
                    logrg_gen = std::log(jetR/rg_gen);
                    logkt_gen = std::log(kt_gen);

                    if (logrg_gen < 0.) logrg_gen = -999.;

                    // kt cut 
                    if (kt_gen < kt_cut) {
                        logrg_gen = logrg_underflow;
                        zg_gen = zg_underflow;
                    }
                } else {
                    // 1-prong jets
                    logrg_gen = logrg_underflow; 
                    zg_gen = zg_underflow;
                }

                logrg_gen_tree = logrg_gen;
                zg_gen_tree = zg_gen;
                weight_tree = t.weight;
            }

            // ---- Fill histograms

            tree_all->Fill();

            if (!mc) continue;

            if (nb_gen_tree == 1)
                tree_sig->Fill();
            else if (nb_gen_tree > 1)
                tree_bkg_bb->Fill();
            else {
                tree_bkg_rest->Fill();
                if (nc_gen_tree >= 1) tree_bkg_c->Fill();
                else tree_bkg_l->Fill();
            }
        } // jet loop
    } // end entries loop
    tree_all->Write("", TObject::kOverwrite);

    if (mc) {
        tree_sig->Write("", TObject::kOverwrite);
        tree_bkg_bb->Write("", TObject::kOverwrite);
        tree_bkg_rest->Write("", TObject::kOverwrite);
        tree_bkg_c->Write("", TObject::kOverwrite);
        tree_bkg_l->Write("", TObject::kOverwrite);
    }
    
    f_out->Close();
    gApplication->Terminate();
}