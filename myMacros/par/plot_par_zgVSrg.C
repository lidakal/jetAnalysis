// This functions creates histograms for the substructure variables zg and Rg
// using the reference jet variables

#include <iostream>
#include "stdio.h"
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include <vector>

void plot_par_zgVSrg() 
{
    const int n = 2;
    
    std::string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    std::string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";
    std::string fnames[n] = {path_qcd, path_bJet};
    
    std::string h_qcd = "h_qcd";
    std::string h_bJet = "h_bJet";
    std::string hnames[n] = {h_qcd, h_bJet};
    
    
    // zg histograms
    TH2F *hs_ktL[n];
    TH2F *hs_ktB[n];
    TH2F *hs_ktC[n];
    
    // Rg histograms
    TH2F *hs_rgzgL[n];
    TH2F *hs_rgzgB[n];
    TH2F *hs_rgzgC[n];    

    for (int ni = 0; ni < n; ni++) {
        
        auto finname = fnames[ni];
        auto hname = hnames[ni];

        std::cout << "\nReading from " << finname << endl;
        TFile *fin = new TFile(finname.c_str());
        TTree *t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
        TTree *HiTree = (TTree *) fin->Get("hiEvtAnalyzer/HiTree");

        //Declaration of leaves types

        // For JetAnalyzer tree
        Int_t           nref;
        Float_t         rawpt[30];
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
        Float_t         sjt1Pt[30];
        Float_t         sjt1Eta[30];
        Float_t         sjt1Phi[30];
        Float_t         sjt1HadFlav[30];
        Float_t         sjt1ParFlav[30];
        Float_t         sjt2Pt[30];
        Float_t         sjt2Eta[30];
        Float_t         sjt2Phi[30];
        Float_t         sjt2HadFlav[30];
        Float_t         sjt2ParFlav[30];
        Float_t         rsjt1Pt[30]; //ref
        Float_t         rsjt1Eta[30];
        Float_t         rsjt1Phi[30];
        Float_t         rsjt2Pt[30];
        Float_t         rsjt2Eta[30];
        Float_t         rsjt2Phi[30];
        Float_t         jtDiscCSVV2[30];
        Float_t         jtDiscDeepCSVB[30];
        Float_t         jtDiscDeepCSVBB[30];
        Float_t         jtDiscDeepCSVC[30];
        Float_t         jtDiscDeepFlavourB[30];
        Float_t         jtDiscDeepFlavourBB[30];
        Float_t         jtDiscDeepFlavourLEPB[30];
        Float_t         jtDiscDeepFlavourC[30];
        Float_t         jtDiscProb[30];
        Int_t           nsvtx[30];
        vector<vector<int> > svtxntrk;
        vector<vector<float> > svtxdls;
        vector<vector<float> > svtxdls2d;
        vector<vector<float> > svtxm;
        vector<vector<float> > svtxpt;
        vector<vector<float> > svtxmcorr;
        Float_t         refpt[30];
        Float_t         refeta[30];
        Float_t         refphi[30];
        Float_t         refdphijt[30];
        Float_t         refdrjt[30];
        Float_t         refparton_pt[30];
        Int_t           refparton_flavor[30];
        Bool_t          refIsHardest[30];
        Int_t           ngen;
        Int_t           genmatchindex[30];
        Float_t         genpt[30];
        Float_t         geneta[30];
        Float_t         genphi[30];
        Int_t           npar;
        Float_t         parpt[30];
        Float_t         pareta[30];
        Float_t         parphi[30];
        Int_t           parNb[30];
        Int_t           parNc[30];
        Bool_t          parHasGSPB[30];
        Bool_t          parHasGSPC[30];
        Bool_t          genIsHardest[30];
        Float_t         gsjt1Pt[30];
        Float_t         gsjt1Eta[30];
        Float_t         gsjt1Phi[30];
        Float_t         gsjt2Pt[30];
        Float_t         gsjt2Eta[30];
        Float_t         gsjt2Phi[30];
        Float_t         psjt1Pt[30];
        Float_t         psjt1Eta[30];
        Float_t         psjt1Phi[30];
        Float_t         psjt2Pt[30];
        Float_t         psjt2Eta[30];
        Float_t         psjt2Phi[30];

        // For EvtAnalyzer tree
        UInt_t          run;
        ULong64_t       evt;
        UInt_t          lumi;
        Float_t         vx;
        Float_t         vy;
        Float_t         vz;
        Float_t         pthat;
        Float_t         weight;    

        // Set branch addresses

        // For JetAnalyzer tree
        t->SetBranchAddress("nref", &nref);
        t->SetBranchAddress("rawpt", rawpt);
        t->SetBranchAddress("jtpt", jtpt);
        t->SetBranchAddress("jteta", jteta);
        t->SetBranchAddress("jtphi", jtphi);
        t->SetBranchAddress("jtHadFlav", jtHadFlav);
        t->SetBranchAddress("jtParFlav", jtParFlav);
        t->SetBranchAddress("jtNbHad", jtNbHad);
        t->SetBranchAddress("jtNcHad", jtNcHad);
        t->SetBranchAddress("jtNbPar", jtNbPar);
        t->SetBranchAddress("jtNcPar", jtNcPar);
        t->SetBranchAddress("jtHasGSPB", jtHasGSPB);
        t->SetBranchAddress("jtHasGSPC", jtHasGSPC);
        t->SetBranchAddress("jtIsHardest", jtIsHardest);
        t->SetBranchAddress("sjt1Pt", sjt1Pt);
        t->SetBranchAddress("sjt1Eta", sjt1Eta);
        t->SetBranchAddress("sjt1Phi", sjt1Phi);
        t->SetBranchAddress("sjt1HadFlav", sjt1HadFlav);
        t->SetBranchAddress("sjt1ParFlav", sjt1ParFlav);
        t->SetBranchAddress("sjt2Pt", sjt2Pt);
        t->SetBranchAddress("sjt2Eta", sjt2Eta);
        t->SetBranchAddress("sjt2Phi", sjt2Phi);
        t->SetBranchAddress("sjt2HadFlav", sjt2HadFlav);
        t->SetBranchAddress("sjt2ParFlav", sjt2ParFlav);
        t->SetBranchAddress("rsjt1Pt", rsjt1Pt);
        t->SetBranchAddress("rsjt1Eta", rsjt1Eta);
        t->SetBranchAddress("rsjt1Phi", rsjt1Phi);
        t->SetBranchAddress("rsjt2Pt", rsjt2Pt);
        t->SetBranchAddress("rsjt2Eta", rsjt2Eta);
        t->SetBranchAddress("rsjt2Phi", rsjt2Phi);
        t->SetBranchAddress("jtDiscCSVV2", jtDiscCSVV2);
        t->SetBranchAddress("jtDiscDeepCSVB", jtDiscDeepCSVB);
        t->SetBranchAddress("jtDiscDeepCSVBB", jtDiscDeepCSVBB);
        t->SetBranchAddress("jtDiscDeepCSVC", jtDiscDeepCSVC);
        t->SetBranchAddress("jtDiscDeepFlavourB", jtDiscDeepFlavourB);
        t->SetBranchAddress("jtDiscDeepFlavourBB", jtDiscDeepFlavourBB);
        t->SetBranchAddress("jtDiscDeepFlavourLEPB", jtDiscDeepFlavourLEPB);
        t->SetBranchAddress("jtDiscDeepFlavourC", jtDiscDeepFlavourC);
        t->SetBranchAddress("jtDiscProb", jtDiscProb);
        t->SetBranchAddress("nsvtx", nsvtx);
        /*
        t->SetBranchAddress("svtxntrk", &svtxntrk);
        t->SetBranchAddress("svtxdls", &svtxdls);
        t->SetBranchAddress("svtxdls2d", &svtxdls2d);
        t->SetBranchAddress("svtxm", &svtxm);
        t->SetBranchAddress("svtxpt", &svtxpt);
        t->SetBranchAddress("svtxmcorr", &svtxmcorr);
        */
        t->SetBranchAddress("refpt", refpt);
        t->SetBranchAddress("refeta", refeta);
        t->SetBranchAddress("refphi", refphi);
        t->SetBranchAddress("refdphijt", refdphijt);
        t->SetBranchAddress("refdrjt", refdrjt);
        t->SetBranchAddress("refparton_pt", refparton_pt);
        t->SetBranchAddress("refparton_flavor", refparton_flavor);
        t->SetBranchAddress("refIsHardest", refIsHardest);
        t->SetBranchAddress("ngen", &ngen);
        t->SetBranchAddress("genmatchindex", genmatchindex);
        t->SetBranchAddress("genpt", genpt);
        t->SetBranchAddress("geneta", geneta);
        t->SetBranchAddress("genphi", genphi);
        t->SetBranchAddress("npar", &npar);
        t->SetBranchAddress("parpt", parpt);
        t->SetBranchAddress("pareta", pareta);
        t->SetBranchAddress("parphi", parphi);
        t->SetBranchAddress("parNb", parNb);
        t->SetBranchAddress("parNc", parNc);
        t->SetBranchAddress("parHasGSPB", parHasGSPB);
        t->SetBranchAddress("parHasGSPC", parHasGSPC);
        t->SetBranchAddress("genIsHardest", genIsHardest);
        t->SetBranchAddress("gsjt1Pt", gsjt1Pt);
        t->SetBranchAddress("gsjt1Eta", gsjt1Eta);
        t->SetBranchAddress("gsjt1Phi", gsjt1Phi);
        t->SetBranchAddress("gsjt2Pt", gsjt2Pt);
        t->SetBranchAddress("gsjt2Eta", gsjt2Eta);
        t->SetBranchAddress("gsjt2Phi", gsjt2Phi);
        t->SetBranchAddress("psjt1Pt", psjt1Pt);
        t->SetBranchAddress("psjt1Eta", psjt1Eta);
        t->SetBranchAddress("psjt1Phi", psjt1Phi);
        t->SetBranchAddress("psjt2Pt", psjt2Pt);
        t->SetBranchAddress("psjt2Eta", psjt2Eta);
        t->SetBranchAddress("psjt2Phi", psjt2Phi);

        // For EvtAnalyzer tree
        HiTree->SetBranchAddress("run", &run);
        HiTree->SetBranchAddress("evt", &evt);
        HiTree->SetBranchAddress("lumi", &lumi);
        HiTree->SetBranchAddress("vx", &vx);
        HiTree->SetBranchAddress("vy", &vy);
        HiTree->SetBranchAddress("vz", &vz);
        HiTree->SetBranchAddress("pthat", &pthat);
        HiTree->SetBranchAddress("weight", &weight);

        // Activate specific branches
        t->SetBranchStatus("*", 0);
        for (auto activeBranchName : {"npar", "psjt1Pt", "psjt1Eta", "psjt1Phi", 
                                     "psjt2Pt", "psjt2Eta", "psjt2Phi", "parpt", 
                                      "parNb", "parNc", "pareta"}) {
            //std::cout << "Activating branch " << activeBranchName << endl;
            t->SetBranchStatus(activeBranchName, 1);
        }
        HiTree->SetBranchStatus("*", 0);
        HiTree->SetBranchStatus("weight", 1);

        // Create the new histograms
        
        // ln(1/rg)
        int xbins = 40;
        float xmin = 0.;
        float xmax = 5.;
        
        // lnkt
        int ybins = 40;
        float ymin = -5.; // log(1/Rg)
        float ymax = 5.;
        
        // ln(1/zg)
        int zbins = 40;
        float zmin = 0.69;
        float zmax = 2.4;
        
        // X = ln(1/rg), Y = lnkt
        TH2F *h_ktL = new TH2F((hname + "_ktL").c_str(), "kt vs rg, l jets", xbins, xmin, xmax, ybins, ymin, ymax);
        TH2F *h_ktB = new TH2F((hname + "_ktB").c_str(), "kt vs rg, b jets", xbins, xmin, xmax, ybins, ymin, ymax);
        TH2F *h_ktC = new TH2F((hname + "_ktC").c_str(), "kt vs rg, c jets", xbins, xmin, xmax, ybins, ymin, ymax);
        
        // X = ln(1/rg), Y = ln(1/zg)
        TH2F *h_rgzgL = new TH2F((hname + "_rgzgL").c_str(), "zg vs rg, l jets", xbins, xmin, xmax, zbins, zmin, zmax);
        TH2F *h_rgzgB = new TH2F((hname + "_rgzgB").c_str(), "zg vs rg, b jets", xbins, xmin, xmax, zbins, zmin, zmax);
        TH2F *h_rgzgC = new TH2F((hname + "_rgzgC").c_str(), "zg vs rg, c jets", xbins, xmin, xmax, zbins, zmin, zmax);
        
        Long64_t nentries = t->GetEntries();

        std::cout << "Creating histograms..." << endl;
        
        for (Long64_t i = 0; i < nentries; i++) {
            t->GetEntry(i);
            HiTree->GetEntry(i);

            // Print progress
            if (i%1000000 == 0) {
                std::cout << "i = " << i << endl;
            }
            
            // Calculate zg, Rg, kt
            for (Long64_t j = 0; j < npar; j++)  {
                Float_t zg = -1.;
                Float_t rg = -1.;
                Float_t kt = -1.;
                
                // Keep the log(1/Rg) = -1 for the underflowbin in the histogram
                Float_t logrg = -1;
                Float_t logzg = -1;
                Float_t logkt = -10;
                
                if (pareta[j] > 2.) { 
                    continue;
                }
                
                
                
                // Calculate zg, rg for jets with proper substructure
                if (psjt2Pt[j] > 0.) {
                    zg = psjt2Pt[j] / (psjt2Pt[j]+psjt1Pt[j]);
                    
                    Float_t deta = psjt1Eta[j] - psjt2Eta[j];
                    Float_t dphi = acos(cos(psjt1Phi[j] - psjt2Phi[j]));
                    rg = sqrt(deta * deta + dphi * dphi);
                    
                    kt = psjt2Pt[j] * rg;
                    
                    // calculate logs
                    logrg = log(1/rg);
                    logzg = log(1/zg);
                    logkt = log(kt);
                }
                
                // Fill the histograms
                if (parNb[j] > 0) {
                    h_ktB->Fill(logrg, logkt, weight);
                    h_rgzgB->Fill(logrg, logzg, weight);
                } else if (parNc[j] > 0) {
                    h_ktC->Fill(logrg, logkt, weight);
                    h_rgzgC->Fill(logrg, logzg, weight);
                } else {
                    h_ktL->Fill(logrg, logkt, weight);
                    h_rgzgL->Fill(logrg, logzg, weight);
                }
            }
        }
        hs_ktB[ni] = h_ktB;
        hs_rgzgB[ni] = h_rgzgB;
        
        hs_ktC[ni] = h_ktC;
        hs_rgzgC[ni] = h_rgzgC;
        
        hs_ktL[ni] = h_ktL;
        hs_rgzgL[ni] = h_rgzgL;
    }       
    std::string foutname = "kt_rgzg_par.root";
    std::cout << "\n(Re)creating file " << foutname << endl;
    TFile *fout = new TFile(foutname.c_str(),  "recreate");
    std::cout << "Saving histograms." << endl;
    
    for (int ni = 0; ni < n; ni++) {
        hs_ktB[ni]->Write();
        hs_rgzgB[ni]->Write();
        
        hs_ktC[ni]->Write();
        hs_rgzgC[ni]->Write();
        
        hs_ktL[ni]->Write();
        hs_rgzgL[ni]->Write();
    }
    fout->Close();
}
