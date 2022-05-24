// TreeAnalyzer_pt2dscan class for .../SD/mergedHiForestAOD.root

#include "TFile.h"
#include "TTree.h"
#include "TH3D.h"

#include <iostream>
#include <stdlib.h>
#include <cmath>

class TreeAnalyzer_pt2dscan
{
    public:
        // Class attributes
        TTree *t;
    
        Float_t     weight; 
        Long64_t    nentries;

        // t tree leaves
        Int_t           nref;
        
        Float_t         jtpt[30];
        Float_t         jteta[30];
        Float_t         jtphi[30];
        Float_t         jtHadFlav[30];
        Float_t         jtParFlav[30];
        Int_t           jtNbHad[30];
        Int_t           jtNcHad[30];
        Int_t           jtNbPar[30];
        Int_t           jtNcPar[30];
        Bool_t          jtHasGSPB[30];
        Bool_t          jtHasGSPC[30];
        Bool_t          jtIsHardest[30];
        Float_t         sjt1E[30];
        Float_t         sjt1Pt[30];
        Float_t         sjt1Eta[30];
        Float_t         sjt1Phi[30];
        Float_t         sjt1HadFlav[30];
        Float_t         sjt1ParFlav[30];
        Float_t         sjt2E[30];
        Float_t         sjt2Pt[30];
        Float_t         sjt2Eta[30];
        Float_t         sjt2Phi[30];
        Float_t         sjt2HadFlav[30];
        Float_t         sjt2ParFlav[30];

		Float_t         jtDiscDeepFlavourB[30];
		Float_t         jtDiscDeepFlavourBB[30];
		Float_t         jtDiscDeepFlavourLEPB[30];
        
        Float_t         refpt[30];
        Float_t         refeta[30];
        Float_t         refphi[30];
        Float_t         refdphijt[30];
        Float_t         refdrjt[30];
        Float_t         refparton_pt[30];
        Int_t           refparton_flavor[30];
        Bool_t          refIsHardest[30];
        Float_t         rsjt1E[30];
        Float_t         rsjt1Pt[30];
        Float_t         rsjt1Eta[30];
        Float_t         rsjt1Phi[30];
        Float_t         rsjt2E[30];
        Float_t         rsjt2Pt[30];
        Float_t         rsjt2Eta[30];
        Float_t         rsjt2Phi[30];
        
        Int_t           ngen;
        Int_t           genmatchindex[30];
        Float_t         genpt[30];
        Float_t         geneta[30];
        Float_t         genphi[30];
        Bool_t          genIsHardest[30];
        Float_t         gsjt1E[30];
        Float_t         gsjt1Pt[30];
        Float_t         gsjt1Eta[30];
        Float_t         gsjt1Phi[30];
        Float_t         gsjt2Pt[30];
        Float_t         gsjt2Eta[30];
        Float_t         gsjt2Phi[30];
        
        Int_t           npar;
        Float_t         parpt[30];
        Float_t         pareta[30];
        Float_t         parphi[30];
        Int_t           parNb[30];
        Int_t           parNc[30];
        Bool_t          parHasGSPB[30];
        Bool_t          parHasGSPC[30];
        Float_t         psjt1E[30];
        Float_t         psjt1Pt[30];
        Float_t         psjt1Eta[30];
        Float_t         psjt1Phi[30];
        Float_t         psjt2Pt[30];
        Float_t         psjt2Eta[30];
        Float_t         psjt2Phi[30];


        // Fat jet properties
        Int_t           njet;
        Float_t         jetpt[30];
        Float_t         jeteta[30];
        Int_t           jetNb[30];
        Int_t           jetNc[30];
        
        // Subjet properties
        Float_t         subjet1pt[30];
        Float_t         subjet1eta[30];
        Float_t         subjet1phi[30];
    
        Float_t         subjet2pt[30];
        Float_t         subjet2eta[30];
        Float_t         subjet2phi[30];

        Bool_t          splitIsHardest[30];

        // Constructors
        TreeAnalyzer_pt2dscan() {};
        TreeAnalyzer_pt2dscan(std::string fname, bool init = true);
    
        // Class methods
        void Init();
        void SetAnalysisLevelParton(bool activBranches = true);
        void SetAnalysisLevelTruth(bool activBranches = true);
        void SetAnalysisLevelReco(bool activBranches = true);

        void plot_pt2dscan(std::string foutname, std::string level, bool GSPincl, Float_t bTagWP = 0.9);
};

TreeAnalyzer_pt2dscan::TreeAnalyzer_pt2dscan(std::string fname, bool init = true) 
{
    TFile *fin = new TFile(fname.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree"); // to get the event weight

    if (init) {
        Init();
    }
}

void TreeAnalyzer_pt2dscan::Init() 
{
    // Set branch addresses for t tree
    t->SetBranchAddress("nref",&nref);
    
    t->SetBranchAddress("jtpt",jtpt);
    t->SetBranchAddress("jteta",jteta);
    t->SetBranchAddress("jtphi",jtphi);
    t->SetBranchAddress("jtHadFlav",jtHadFlav);
    t->SetBranchAddress("jtParFlav",jtParFlav);
    t->SetBranchAddress("jtNbHad",jtNbHad);
    t->SetBranchAddress("jtNcHad",jtNcHad);
    t->SetBranchAddress("jtNbPar",jtNbPar);
    t->SetBranchAddress("jtNcPar",jtNcPar);
    t->SetBranchAddress("jtHasGSPB",jtHasGSPB);
    t->SetBranchAddress("jtHasGSPC",jtHasGSPC);
    t->SetBranchAddress("jtIsHardest",jtIsHardest);
    t->SetBranchAddress("sjt1E",sjt1E);
    t->SetBranchAddress("sjt1Pt",sjt1Pt);
    t->SetBranchAddress("sjt1Eta",sjt1Eta);
    t->SetBranchAddress("sjt1Phi",sjt1Phi);
    t->SetBranchAddress("sjt1HadFlav",sjt1HadFlav);
    t->SetBranchAddress("sjt1ParFlav",sjt1ParFlav);
    t->SetBranchAddress("sjt2E",sjt2E);
    t->SetBranchAddress("sjt2Pt",sjt2Pt);
    t->SetBranchAddress("sjt2Eta",sjt2Eta);
    t->SetBranchAddress("sjt2Phi",sjt2Phi);
    t->SetBranchAddress("sjt2HadFlav",sjt2HadFlav);
    t->SetBranchAddress("sjt2ParFlav",sjt2ParFlav);

	t->SetBranchAddress("jtDiscDeepFlavourB", jtDiscDeepFlavourB);
	t->SetBranchAddress("jtDiscDeepFlavourBB", jtDiscDeepFlavourBB);
	t->SetBranchAddress("jtDiscDeepFlavourLEPB", jtDiscDeepFlavourLEPB);
    
    t->SetBranchAddress("refpt",refpt);
    t->SetBranchAddress("refeta",refeta);
    t->SetBranchAddress("refphi",refphi);
    t->SetBranchAddress("refdphijt",refdphijt);
    t->SetBranchAddress("refdrjt",refdrjt);
    t->SetBranchAddress("refparton_pt",refparton_pt);
    t->SetBranchAddress("refparton_flavor",refparton_flavor);
    t->SetBranchAddress("refIsHardest",refIsHardest);
    t->SetBranchAddress("rsjt1E",rsjt1E);
    t->SetBranchAddress("rsjt1Pt",rsjt1Pt);
    t->SetBranchAddress("rsjt1Eta",rsjt1Eta);
    t->SetBranchAddress("rsjt1Phi",rsjt1Phi);
    t->SetBranchAddress("rsjt2E",rsjt2E);
    t->SetBranchAddress("rsjt2Pt",rsjt2Pt);
    t->SetBranchAddress("rsjt2Eta",rsjt2Eta);
    t->SetBranchAddress("rsjt2Phi",rsjt2Phi);

    t->SetBranchAddress("ngen",&ngen);
    t->SetBranchAddress("genmatchindex",genmatchindex);
    t->SetBranchAddress("genpt",genpt);
    t->SetBranchAddress("geneta",geneta);
    t->SetBranchAddress("genphi",genphi);
    t->SetBranchAddress("genIsHardest",genIsHardest);
    t->SetBranchAddress("gsjt1E",gsjt1E);
    t->SetBranchAddress("gsjt1Pt",gsjt1Pt);
    t->SetBranchAddress("gsjt1Eta",gsjt1Eta);
    t->SetBranchAddress("gsjt1Phi",gsjt1Phi);
    t->SetBranchAddress("gsjt2Pt",gsjt2Pt);
    t->SetBranchAddress("gsjt2Eta",gsjt2Eta);
    t->SetBranchAddress("gsjt2Phi",gsjt2Phi);

    t->SetBranchAddress("npar",&npar);
    t->SetBranchAddress("parpt",parpt);
    t->SetBranchAddress("pareta",pareta);
    t->SetBranchAddress("parphi",parphi);
    t->SetBranchAddress("parNb",parNb);
    t->SetBranchAddress("parNc",parNc);
    t->SetBranchAddress("parHasGSPB",parHasGSPB);
    t->SetBranchAddress("parHasGSPC",parHasGSPC);
    t->SetBranchAddress("psjt1E",psjt1E);
    t->SetBranchAddress("psjt1Pt",psjt1Pt);
    t->SetBranchAddress("psjt1Eta",psjt1Eta);
    t->SetBranchAddress("psjt1Phi",psjt1Phi);
    t->SetBranchAddress("psjt2Pt",psjt2Pt);
    t->SetBranchAddress("psjt2Eta",psjt2Eta);
    t->SetBranchAddress("psjt2Phi",psjt2Phi);

    t->SetBranchAddress("weight", &weight);
    
    nentries = t->GetEntries();
}

void TreeAnalyzer_pt2dscan::SetAnalysisLevelParton(bool activateBranches = true)
{
    // Set branch addresses of class attributes to parton level branches
    t->SetBranchAddress("weight", &weight);

    t->SetBranchAddress("npar", &njet);
    t->SetBranchAddress("parpt", jetpt);
    t->SetBranchAddress("pareta", jeteta);
    t->SetBranchAddress("parNb", jetNb);
    t->SetBranchAddress("parNc", jetNc);

    t->SetBranchAddress("psjt1Pt", subjet1pt);
    t->SetBranchAddress("psjt1Eta", subjet1eta);
    t->SetBranchAddress("psjt1Phi", subjet1phi);

    t->SetBranchAddress("psjt2Pt", subjet2pt);
    t->SetBranchAddress("psjt2Eta", subjet2eta);
    t->SetBranchAddress("psjt2Phi", subjet2phi);

    if (activateBranches) {
        t->SetBranchStatus("*", 0);
        for (auto activeBranchName : {"npar", "parpt", "pareta", "parNb", "parNc",
                                      "psjt1Pt", "psjt1Eta", "psjt1Phi", 
                                      "psjt2Pt", "psjt2Eta", "psjt2Phi",
                                      "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB",
                                      "weight"}) {
            t->SetBranchStatus(activeBranchName, 1);
        }
    }
}

void TreeAnalyzer_pt2dscan::SetAnalysisLevelTruth(bool activateBranches = true)
{
    // Set branch addresses of class attributes to truth level branches
    t->SetBranchAddress("weight", &weight);

    t->SetBranchAddress("nref", &njet);
    t->SetBranchAddress("refpt", jetpt);
    t->SetBranchAddress("refeta", jeteta);
    t->SetBranchAddress("jtNbHad", jetNb);
    t->SetBranchAddress("jtNcHad", jetNc);

    t->SetBranchAddress("rsjt1Pt", subjet1pt);
    t->SetBranchAddress("rsjt1Eta", subjet1eta);
    t->SetBranchAddress("rsjt1Phi", subjet1phi);

    t->SetBranchAddress("rsjt2Pt", subjet2pt);
    t->SetBranchAddress("rsjt2Eta", subjet2eta);
    t->SetBranchAddress("rsjt2Phi", subjet2phi);

    t->SetBranchAddress("refIsHardest", splitIsHardest);

    if (activateBranches) {
        t->SetBranchStatus("*", 0);
        for (auto activeBranchName : {"nref", "refpt", "refeta", "jtHadFlav", "refIsHardest",
                                      "jtNbHad", "jtNcHad",
                                      "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", 
                                      "rsjt2Pt", "rsjt2Eta", "rsjt2Phi",
                                      "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB",
                                      "weight"}) {
            t->SetBranchStatus(activeBranchName, 1);
        }
    }
}

void TreeAnalyzer_pt2dscan::SetAnalysisLevelReco(bool activateBranches = true)
{
// Set branch addresses of class attributes to reco level branches
    t->SetBranchAddress("weight", &weight);

    t->SetBranchAddress("nref", &njet);
    t->SetBranchAddress("jtpt", jetpt);
    t->SetBranchAddress("jteta", jeteta);
    t->SetBranchAddress("jtNbHad", jetNb);
    t->SetBranchAddress("jtNcHad", jetNc);

    t->SetBranchAddress("sjt1Pt", subjet1pt);
    t->SetBranchAddress("sjt1Eta", subjet1eta);
    t->SetBranchAddress("sjt1Phi", subjet1phi);

    t->SetBranchAddress("sjt2Pt", subjet2pt);
    t->SetBranchAddress("sjt2Eta", subjet2eta);
    t->SetBranchAddress("sjt2Phi", subjet2phi);

    t->SetBranchAddress("jtIsHardest", splitIsHardest);

    if (activateBranches) {
        t->SetBranchStatus("*", 0);
        for (auto activeBranchName : {"nref", "jtpt", "jteta", "jtHadFlav", "jtIsHardest",
                                      "jtNbHad", "jtNcHad",
                                      "sjt1Pt", "sjt1Eta", "sjt1Phi", 
                                      "sjt2Pt", "sjt2Eta", "sjt2Phi",
			                          "jtDiscDeepFlavourB", "jtDiscDeepFlavourBB", "jtDiscDeepFlavourLEPB",
                                      "weight"}) {
            t->SetBranchStatus(activeBranchName, 1);
        }
    }
}

void TreeAnalyzer_pt2dscan::plot_pt2dscan(std::string foutname, std::string level, bool GSPincl, Float_t bTagWP = 0.9)
{
    if (!GSPincl) {
        foutname += "_noGSP";
    }

    // Set analysis level
    if (level == "par") {
        SetAnalysisLevelParton();
        foutname += "_par.root";
    } else if (level == "ref") {
        SetAnalysisLevelTruth();
        foutname += "_ref.root";
    } else if (level == "reco") {
        SetAnalysisLevelReco();
        foutname += "_reco.root";
    } else {
        std::cout << "Analysis level not recognised. Please choose between 'par', 'ref' or 'reco'. Exiting." << std::endl;
        std::exit(1);
    }

    // Create histograms

    // ln(1/rg)
    Int_t x1bins = 40;
    Float_t x1min = 0.91;
    Float_t x1max = 5.;

    // ln(kt)
    Int_t y1bins = 40;
    Float_t y1min = -5.;
    Float_t y1max = 5.;

    // jetpt
    Int_t z1bins = 27*2;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    TH3D *hB_rgkt = new TH3D("hB_rgkt", "rg, kt, pt, bjets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hB_rgkt_dynKt = new TH3D("hB_rgkt_dynKt", "rg, kt, pt, bjets, dynKt only", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hBtag_rgkt = new TH3D("hBtag_rgkt", "rg, kt, pt, b tagged jets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hBtag_rgkt_dynKt = new TH3D("hBtag_rgkt_dynKt", "rg, kt, pt, b tagged jets, dynKt only", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hC_rgkt = new TH3D("hC_rgkt", "rg, kt, pt, bjets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hC_rgkt_dynKt = new TH3D("hC_rgkt_dynKt", "rg, kt, pt, cjets, dynKt only", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hL_rgkt = new TH3D("hL_rgkt", "rg, kt, pt, bjets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hL_rgkt_dynKt = new TH3D("hL_rgkt_dynKt", "rg, kt, pt, ljets, dynKt only", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    std::cout << "Creating histograms ..." << std::endl;

    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 1000000 == 0) {
            cout << "ient = " << ient << endl;
        }

        t->GetEntry(ient);

        for (Int_t ijet = 0; ijet < njet; ijet++) {
            // universal eta cut
            if (std::abs(jeteta[ijet]) > 2) continue;

            Float_t rg = -1.;
            Float_t kt = -1.;

            Float_t logrg = -1.;
            Float_t logkt = -10.;

            // Calculate rg, kt only for 2 prong jets
            if (subjet2pt[ijet] > 0.) {
                Float_t deta = subjet1eta[ijet] - subjet2eta[ijet];
                Float_t dphi = std::acos(std::cos(subjet1phi[ijet] - subjet2phi[ijet]));

                rg = sqrt(deta * deta + dphi * dphi);
                kt = subjet2pt[ijet] * rg;
                
                // calculate logs
                logrg = log(1/rg);
                logkt = log(kt);
            }

            // Fill true-flavour histograms
            if (jetNb[ijet] > 0) {
                if (GSPincl || jetNb[ijet] == 1) {
                    hB_rgkt->Fill(logrg, logkt, jetpt[ijet], weight);
                }
            } else if (jetNc[ijet] > 0) {
                if (GSPincl || jetNc[ijet] == 1) {
                    hC_rgkt->Fill(logrg, logkt, jetpt[ijet], weight);
                }
            } else {
                hL_rgkt->Fill(logrg, logkt, jetpt[ijet], weight);
            }

            // Fill the b-tag histogram
            bool passWP = ((jtDiscDeepFlavourB[ijet] + jtDiscDeepFlavourBB[ijet] + jtDiscDeepFlavourLEPB[ijet]) > bTagWP);
            if (passWP) {
                if (GSPincl || jetNb[ijet] == 1) { // TODO: NEED TO FIX THIS TO NOT USE TRUTH GSP
                    hBtag_rgkt->Fill(logrg, logkt, jetpt[ijet], weight);
                }
            }

            // Same for dynKt histograms
            if (!splitIsHardest[ijet]) {
                logrg = -1.;
                logkt = -10.;
            }

            if (jetNb[ijet] > 0) {
                if (GSPincl || jetNb[ijet] == 1) {
                    hB_rgkt_dynKt->Fill(logrg, logkt, jetpt[ijet], weight);
                }
            } else if (jetNc[ijet] > 0) {
                if (GSPincl || jetNc[ijet] == 1) {
                    hC_rgkt_dynKt->Fill(logrg, logkt, jetpt[ijet], weight);
                }
            } else {
                hL_rgkt_dynKt->Fill(logrg, logkt, jetpt[ijet], weight);
            }

            if (passWP) {
                if (GSPincl || jetNb[ijet] == 1) { // TODO: NEED TO FIX THIS TO NOT USE TRUTH GSP
                    hBtag_rgkt_dynKt->Fill(logrg, logkt, jetpt[ijet], weight);
                }
            }
        } // jet loop
    } // entry loop

    // Save histograms
    cout << "\n(Re)creating file " << foutname << endl;
    TFile *fout = new TFile(foutname.c_str(), "recreate");

    for (auto h : {hB_rgkt, hB_rgkt_dynKt, hBtag_rgkt, hBtag_rgkt_dynKt, hL_rgkt, hL_rgkt_dynKt, hC_rgkt, hC_rgkt_dynKt}) {
        h->Write();
    }

    fout->Close();
}
