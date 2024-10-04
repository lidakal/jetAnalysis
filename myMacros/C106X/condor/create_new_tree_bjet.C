#include "tTree.h"
#include "hist_utils.h"

void create_new_tree_bjet(std::string filename, bool mc = true)
{
    std::cout << "File in : " << filename << std::endl;
    std::cout << "mc : " << mc << std::endl;

    TString rootf_out = "bjet_file.root";

    std::vector<TString> tree_names;

    if (mc) {
        tree_names = {"tree_all", 
                      "tree_sig", 
                      "tree_bkg_bb", "tree_bkg_rest",
                      "tree_bkg_c", "tree_bkg_l",
                      "tree_bkg_singlec", "tree_bkg_cc"
        };
    } else {
        tree_names = {"tree_all"};
    }

    TFile *fin = new TFile(filename.c_str());
    TFile *fout = new TFile(rootf_out, "recreate");

    for (TString tree_name : tree_names) {
        TTree *tree_old = (TTree *) fin->Get(tree_name);
        tree_old->SetName("tree_old");

        // reco 
        Double_t jtpt_old_tree = -999.;
        Double_t jteta_old_tree = -999.;
        Double_t jtphi_old_tree = -999.;
        Double_t jtptCh_old_tree = -999.;
        Double_t logrg_old_tree = -999.; 
        Double_t logkt_old_tree = -999.; 
        Double_t zg_old_tree = -999.; 
        Double_t mb_old_tree = -999.;
        Double_t bpt_old_tree = -999.;
        Double_t discr_b_old_tree = -999.;
        Double_t discr_lepb_old_tree = -999.;
        Double_t discr_bb_old_tree = -999.;
        Double_t discr_particleNet_b_old_tree = -999.;
        Double_t discr_particleNet_BvsAll_old_tree = -999.;
        Double_t discr_particleNet_bb_old_tree = -999.;
        Double_t discr_pfJP_old_tree = -999.;

        // gen
        Double_t jtpt_gen_old_tree = -999.; 
        Double_t jteta_gen_old_tree = -999.; 
        Double_t jtphi_gen_old_tree = -999.; 
        Double_t jtptCh_gen_old_tree = -999.; 
        Double_t logrg_gen_old_tree = -999.; 
        Double_t logkt_gen_old_tree = -999.; 
        Double_t zg_gen_old_tree = -999.; 
        Double_t mb_gen_old_tree = -999.;
        Double_t bpt_gen_old_tree = -999.;
        Int_t nb_gen_old_tree = -999;
        Int_t nc_gen_old_tree = -999;
        Int_t parton_flavor_old_tree = -999;
        Int_t refparton_flavor_old_tree = -999;

        Double_t weight_old_tree = -999.;
        Double_t pthat_old_tree = -999.;
        Double_t jer_sf_nom_old_tree = -999.;
        Double_t jer_sf_up_old_tree = -999.;
        Double_t jer_sf_down_old_tree = -999.;
        Double_t jec_unc_old_tree = -999.;
        

        tree_old->SetBranchAddress("jtpt", &jtpt_old_tree);
        tree_old->SetBranchAddress("jteta", &jteta_old_tree);
        tree_old->SetBranchAddress("jtphi", &jtphi_old_tree);
        tree_old->SetBranchAddress("jtptCh", &jtptCh_old_tree);
        tree_old->SetBranchAddress("logrg", &logrg_old_tree);
        tree_old->SetBranchAddress("logkt", &logkt_old_tree);
        tree_old->SetBranchAddress("zg", &zg_old_tree);
        tree_old->SetBranchAddress("mb", &mb_old_tree);
        tree_old->SetBranchAddress("bpt", &bpt_old_tree);
        tree_old->SetBranchAddress("discr_b", &discr_b_old_tree);
        tree_old->SetBranchAddress("discr_lepb", &discr_lepb_old_tree);
        tree_old->SetBranchAddress("discr_bb", &discr_bb_old_tree);
        tree_old->SetBranchAddress("discr_particleNet_b", &discr_particleNet_b_old_tree);
        tree_old->SetBranchAddress("discr_particleNet_BvsAll", &discr_particleNet_BvsAll_old_tree);
        tree_old->SetBranchAddress("discr_particleNet_bb", &discr_particleNet_bb_old_tree);
        tree_old->SetBranchAddress("discr_pfJP", &discr_pfJP_old_tree);

        tree_old->SetBranchAddress("jtpt_gen", &jtpt_gen_old_tree);
        tree_old->SetBranchAddress("jteta_gen", &jteta_gen_old_tree);
        tree_old->SetBranchAddress("jtphi_gen", &jtphi_gen_old_tree);
        tree_old->SetBranchAddress("jtptCh_gen", &jtptCh_gen_old_tree);
        tree_old->SetBranchAddress("logrg_gen", &logrg_gen_old_tree);
        tree_old->SetBranchAddress("logkt_gen", &logkt_gen_old_tree);
        tree_old->SetBranchAddress("zg_gen", &zg_gen_old_tree);
        tree_old->SetBranchAddress("mb_gen", &mb_gen_old_tree);
        tree_old->SetBranchAddress("bpt_gen", &bpt_gen_old_tree);
        tree_old->SetBranchAddress("nb_gen", &nb_gen_old_tree);
        tree_old->SetBranchAddress("nc_gen", &nc_gen_old_tree);
        tree_old->SetBranchAddress("parton_flavor", &parton_flavor_old_tree);
        tree_old->SetBranchAddress("refparton_flavor", &refparton_flavor_old_tree);

        tree_old->SetBranchAddress("weight", &weight_old_tree);
        tree_old->SetBranchAddress("pthat", &pthat_old_tree);
        tree_old->SetBranchAddress("jer_sf_nom", &jer_sf_nom_old_tree);
        tree_old->SetBranchAddress("jer_sf_up", &jer_sf_up_old_tree);
        tree_old->SetBranchAddress("jer_sf_down", &jer_sf_down_old_tree);
        tree_old->SetBranchAddress("jec_unc", &jec_unc_old_tree);

        TTree *tree_new = new TTree(tree_name, "");

        // reco 
        Double_t jtpt_new_tree = -999.;
        Double_t jteta_new_tree = -999.;
        Double_t jtphi_new_tree = -999.;
        Double_t jtptCh_new_tree = -999.;
        Double_t logrg_new_tree = -999.; 
        Double_t logkt_new_tree = -999.; 
        Double_t zg_new_tree = -999.; 
        Double_t mb_new_tree = -999.;
        Double_t bpt_new_tree = -999.;
        Double_t discr_b_new_tree = -999.;
        Double_t discr_lepb_new_tree = -999.;
        Double_t discr_bb_new_tree = -999.;
        Double_t discr_particleNet_b_new_tree = -999.;
        Double_t discr_particleNet_BvsAll_new_tree = -999.;
        Double_t discr_particleNet_bb_new_tree = -999.;
        Double_t discr_pfJP_new_tree = -999.;

        // gen
        Double_t jtpt_gen_new_tree = -999.; 
        Double_t jteta_gen_new_tree = -999.; 
        Double_t jtphi_gen_new_tree = -999.; 
        Double_t jtptCh_gen_new_tree = -999.; 
        Double_t logrg_gen_new_tree = -999.; 
        Double_t logkt_gen_new_tree = -999.; 
        Double_t zg_gen_new_tree = -999.; 
        Double_t mb_gen_new_tree = -999.;
        Double_t bpt_gen_new_tree = -999.;
        Int_t nb_gen_new_tree = -999;
        Int_t nc_gen_new_tree = -999;
        Int_t parton_flavor_new_tree = -999;
        Int_t refparton_flavor_new_tree = -999;

        Double_t weight_new_tree = -999.;
        Double_t pthat_new_tree = -999.;
        Double_t jer_sf_nom_new_tree = -999.;
        Double_t jer_sf_up_new_tree = -999.;
        Double_t jer_sf_down_new_tree = -999.;
        Double_t jec_unc_new_tree = -999.;

        tree_new->Branch("jtpt", &jtpt_new_tree, "jtpt/D");
        tree_new->Branch("jteta", &jteta_new_tree, "jteta/D");
        tree_new->Branch("jtphi", &jtphi_new_tree, "jtphi/D");
        tree_new->Branch("jtptCh", &jtptCh_new_tree, "jtptCh/D");
        tree_new->Branch("logrg", &logrg_new_tree, "logrg/D");
        tree_new->Branch("logkt", &logkt_new_tree, "logkt/D");
        tree_new->Branch("zg", &zg_new_tree, "zg/D");
        tree_new->Branch("mb", &mb_new_tree, "mb/D");
        tree_new->Branch("bpt", &bpt_new_tree, "bpt/D");
        tree_new->Branch("discr_b", &discr_b_new_tree, "discr_b/D");
        tree_new->Branch("discr_lepb", &discr_lepb_new_tree, "discr_lepb/D");
        tree_new->Branch("discr_bb", &discr_bb_new_tree, "discr_bb/D");
        tree_new->Branch("discr_particleNet_b", &discr_particleNet_b_new_tree, "discr_particleNet_b/D");
        tree_new->Branch("discr_particleNet_BvsAll", &discr_particleNet_BvsAll_new_tree, "discr_particleNet_BvsAll/D");
        tree_new->Branch("discr_particleNet_bb", &discr_particleNet_bb_new_tree, "discr_particleNet_bb/D");
        tree_new->Branch("discr_pfJP", &discr_pfJP_new_tree, "discr_pfJP/D");

        tree_new->Branch("jtpt_gen", &jtpt_gen_new_tree, "jtpt_gen/D");
        tree_new->Branch("jteta_gen", &jteta_gen_new_tree, "jteta_gen/D");
        tree_new->Branch("jtphi_gen", &jtphi_gen_new_tree, "jtphi_gen/D");
        tree_new->Branch("jtptCh_gen", &jtptCh_gen_new_tree, "jtptCh_gen/D");
        tree_new->Branch("logrg_gen", &logrg_gen_new_tree, "logrg_gen/D");
        tree_new->Branch("logkt_gen", &logkt_gen_new_tree, "logkt_gen/D");
        tree_new->Branch("zg_gen", &zg_gen_new_tree, "zg_gen/D");
        tree_new->Branch("mb_gen", &mb_gen_new_tree, "mb_gen/D");
        tree_new->Branch("bpt_gen", &bpt_gen_new_tree, "bpt_gen/D");
        tree_new->Branch("nb_gen", &nb_gen_new_tree, "nb_gen/I");
        tree_new->Branch("nc_gen", &nc_gen_new_tree, "nc_gen/I");
        tree_new->Branch("parton_flavor", &parton_flavor_new_tree, "parton_flavor/I");
        tree_new->Branch("refparton_flavor", &refparton_flavor_new_tree, "refparton_flavor/I");

        tree_new->Branch("weight", &weight_new_tree, "weight/D");
        tree_new->Branch("pthat", &pthat_new_tree, "pthat/D");
        tree_new->Branch("jer_sf_nom", &jer_sf_nom_new_tree, "jer_sf_nom/D");
        tree_new->Branch("jer_sf_up", &jer_sf_up_new_tree, "jer_sf_up/D");
        tree_new->Branch("jer_sf_down", &jer_sf_down_new_tree, "jer_sf_down/D");
        tree_new->Branch("jec_unc", &jec_unc_new_tree, "jec_unc/D");
        

        double btag_cut = 0.7;

        Long64_t total_events = 10000000000;
        Long64_t nentries = tree_old->GetEntries();
        if (total_events > nentries) total_events = nentries;

        for (Long64_t ient = 0; ient < total_events; ient++) {
            // Print progress
            if (ient % 10000000 == 0) {
                std::cout << "ient = " << ient << std::endl;
            }

            tree_old->GetEntry(ient);

            jtpt_new_tree = jtpt_old_tree;
            jteta_new_tree = jteta_old_tree;
            jtphi_new_tree = jtphi_old_tree;
            jtptCh_new_tree = jtptCh_old_tree;
            logrg_new_tree = logrg_old_tree;
            logkt_new_tree = logkt_old_tree;
            zg_new_tree = zg_old_tree;
            mb_new_tree = mb_old_tree;
            bpt_new_tree = bpt_old_tree;
            discr_b_new_tree = discr_b_old_tree;
            discr_lepb_new_tree = discr_lepb_old_tree;
            discr_bb_new_tree = discr_bb_old_tree;
            discr_particleNet_b_new_tree = discr_particleNet_b_old_tree;
            discr_particleNet_BvsAll_new_tree = discr_particleNet_BvsAll_old_tree;
            discr_particleNet_bb_new_tree = discr_particleNet_bb_old_tree;
            discr_pfJP_new_tree = discr_pfJP_old_tree;

            jtpt_gen_new_tree = jtpt_gen_old_tree;
            jteta_gen_new_tree = jteta_gen_old_tree;
            jtphi_gen_new_tree = jtphi_gen_old_tree;
            jtptCh_gen_new_tree = jtptCh_gen_old_tree;
            logrg_gen_new_tree = logrg_gen_old_tree;
            logkt_gen_new_tree = logkt_gen_old_tree;
            zg_gen_new_tree = zg_gen_old_tree;
            mb_gen_new_tree = mb_gen_old_tree;
            bpt_gen_new_tree = bpt_gen_old_tree;
            nb_gen_new_tree = nb_gen_old_tree;
            nc_gen_new_tree = nc_gen_old_tree;

            weight_new_tree = weight_old_tree;
            pthat_new_tree = pthat_old_tree;
            jer_sf_nom_new_tree = jer_sf_nom_old_tree;
            jer_sf_up_new_tree = jer_sf_up_old_tree;
            jer_sf_down_new_tree = jer_sf_down_old_tree;
            jec_unc_new_tree = jec_unc_old_tree;

            // std::cout << "jtpt_old_tree=" << jtpt_old_tree << ", discr_b_old_tree=" << discr_b_old_tree << std::endl;
            bool passDeepFlavour = discr_b_new_tree+discr_lepb_new_tree > 0.7;
            bool passParticleNetXXT = discr_particleNet_BvsAll_new_tree > 0.99;
            bool passParticleNetXT = discr_particleNet_BvsAll_new_tree > 0.9;
            bool passParticleNetT = discr_particleNet_BvsAll_new_tree > 0.35;
            
            if (passParticleNetXXT) {
                // std::cout << "in" << std::endl;
                tree_new->Fill();
            }

        } // end entries loop
        tree_new->Write("", TObject::kOverwrite);
    }
    fout->Close();
}