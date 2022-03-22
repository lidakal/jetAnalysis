/*
    2D pt scan of ln(kt) vs ln(1/rg) (Lund plane)
    and zg vs ln(1/rg)
*/

#include <initializer_list>
#include <iostream>
#include <vector>
#include "TFile.h"
#include "TH3F.h"
#include "TTree.h"

using namespace std;

void plot_pt2dscan_ref() 
{
    const int n = 2;
    
    string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";
    string fnames[n] = {path_qcd, path_bJet};
    
    string h_qcd = "h_qcd";
    string h_bJet = "h_bJet";
    string hnames[n] = {h_qcd, h_bJet};
    
    // Initialize histograms
    
    // kt vs ln(1/rg) histograms
    TH3F *hs_ktL[n];
    TH3F *hs_ktB[n];
    TH3F *hs_ktC[n];
    
    TH3F *hs_ktL_dynKt[n];
    TH3F *hs_ktB_dynKt[n];
    TH3F *hs_ktC_dynKt[n];
    
    // zg vs ln(1/rg) histograms
    TH3F *hs_rgzgL[n];
    TH3F *hs_rgzgB[n];
    TH3F *hs_rgzgC[n];  
    
    TH3F *hs_rgzgL_dynKt[n];
    TH3F *hs_rgzgB_dynKt[n];
    TH3F *hs_rgzgC_dynKt[n];  

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
        vector<vector<int>> svtxntrk;
        vector<vector<float>> svtxdls;
        vector<vector<float>> svtxdls2d;
        vector<vector<float>> svtxm;
        vector<vector<float>> svtxpt;
        vector<vector<float>> svtxmcorr;
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
        for (auto activeBranchName : {"nref", "refpt", "refeta", "jtHadFlav", "refIsHardest",
                                      "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", 
                                      "rsjt2Pt", "rsjt2Eta", "rsjt2Phi"}) {
            //std::cout << "Activating branch " << activeBranchName << endl;
            t->SetBranchStatus(activeBranchName, 1);
        }
        HiTree->SetBranchStatus("*", 0);
        HiTree->SetBranchStatus("weight", 1);

        // Create the new histograms
        
        // ln(1/rg)
        int x1bins = 40;
        float x1min = 0.91;
        float x1max = 5.;
        
        // lnkt
        int y1bins = 40;
        float y1min = -5.; 
        float y1max = 5.;
        
        // zg
        int y2bins = 40;
        float y2min = 0.1;
        float y2max = 0.5;
        
        // jetpt
        int z1bins = 27*2;
        float z1min = 30;
        float z1max = 300;
        
        // X = ln(1/rg), Y = lnkt, Z = refpt
        TH3F *h_ktL = new TH3F((hname + "_ktL").c_str(), "rg, kt, pt, l jets", 
                               x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
        TH3F *h_ktB = new TH3F((hname + "_ktB").c_str(), "rg, kt, pt, b jets", 
                               x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
        TH3F *h_ktC = new TH3F((hname + "_ktC").c_str(), "rg, kt, pt, c jets", 
                               x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
        
        TH3F *h_ktL_dynKt = new TH3F((hname + "_ktL_dynKt").c_str(), "rg, kt, pt, dynKt, l jets", 
                                     x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
        TH3F *h_ktB_dynKt = new TH3F((hname + "_ktB_dynKt").c_str(), "rg, kt, pt, dynKt, b jets", 
                                     x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
        TH3F *h_ktC_dynKt = new TH3F((hname + "_ktC_dynKt").c_str(), "rg, kt, pt, dynKt, c jets", 
                                     x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
        
        // X = ln(1/rg), Y = zg, Z = refpt
        TH3F *h_rgzgL = new TH3F((hname + "_rgzgL").c_str(), "rg, zg, pt, l jets", 
                                 x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
        TH3F *h_rgzgB = new TH3F((hname + "_rgzgB").c_str(), "rg, zg, pt, b jets", 
                                 x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
        TH3F *h_rgzgC = new TH3F((hname + "_rgzgC").c_str(), "rg, zg, pt, c jets", 
                                 x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
        
        TH3F *h_rgzgL_dynKt = new TH3F((hname + "_rgzgL_dynKt").c_str(), "rg, zg, pt, dynKt, l jets", 
                                       x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
        TH3F *h_rgzgB_dynKt = new TH3F((hname + "_rgzgB_dynKt").c_str(), "rg, zg, pt, dynKt, b jets", 
                                       x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
        TH3F *h_rgzgC_dynKt = new TH3F((hname + "_rgzgC_dynKt").c_str(), "rg, zg, pt, dynKt, c jets", 
                                       x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
        
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
            for (Long64_t j = 0; j < nref; j++)  {
                Float_t zg = -1.;
                Float_t rg = -1.;
                Float_t kt = -1.;
                
                // Keep the log(1/Rg) = -1 for the underflowbin in the histogram
                Float_t logrg = -1;
                Float_t logkt = -10;
                
                if (refeta[j] > 2.) { 
                    continue;
                }
                
                // Calculate zg, rg for jets with proper substructure
                if (rsjt2Pt[j] > 0.) {
                    zg = rsjt2Pt[j] / (rsjt2Pt[j]+rsjt1Pt[j]);
                    
                    Float_t deta = rsjt1Eta[j] - rsjt2Eta[j];
                    Float_t dphi = acos(cos(rsjt1Phi[j] - rsjt2Phi[j]));
                    rg = sqrt(deta * deta + dphi * dphi);
                    
                    kt = rsjt2Pt[j] * rg;
                    
                    // calculate logs
                    logrg = log(1/rg);
                    logkt = log(kt);
                }
                
                // Fill the histograms
                if (jtHadFlav[j] == 5) {
                    h_ktB->Fill(logrg, logkt, refpt[j], weight);
                    h_rgzgB->Fill(logrg, zg, refpt[j], weight);
                } else if (jtHadFlav[j] == 4) {
                    h_ktC->Fill(logrg, logkt, refpt[j], weight);
                    h_rgzgC->Fill(logrg, zg, refpt[j], weight);
                } else {
                    h_ktL->Fill(logrg, logkt, refpt[j], weight);
                    h_rgzgL->Fill(logrg, zg, refpt[j], weight);
                }
                
                // Fill dynKt histograms
                if (!refIsHardest[j]) {
                    logrg = -1;
                    zg = -1;
                    logkt = -10;
                }
                if (jtHadFlav[j] == 5) {
                    h_ktB_dynKt->Fill(logrg, logkt, refpt[j], weight);
                    h_rgzgB_dynKt->Fill(logrg, zg, refpt[j], weight);
                } else if (jtHadFlav[j] == 4) {
                    h_ktC_dynKt->Fill(logrg, logkt, refpt[j], weight);
                    h_rgzgC_dynKt->Fill(logrg, zg, refpt[j], weight);
                } else {
                    h_ktL_dynKt->Fill(logrg, logkt, refpt[j], weight);
                    h_rgzgL_dynKt->Fill(logrg, zg, refpt[j], weight);
                }
            }
                
        }
        hs_ktB[ni] = h_ktB;
        hs_rgzgB[ni] = h_rgzgB;
        
        hs_ktC[ni] = h_ktC;
        hs_rgzgC[ni] = h_rgzgC;
        
        hs_ktL[ni] = h_ktL;
        hs_rgzgL[ni] = h_rgzgL;
        
        hs_ktB_dynKt[ni] = h_ktB_dynKt;
        hs_rgzgB_dynKt[ni] = h_rgzgB_dynKt;
        
        hs_ktC_dynKt[ni] = h_ktC_dynKt;
        hs_rgzgC_dynKt[ni] = h_rgzgC_dynKt;
        
        hs_ktL_dynKt[ni] = h_ktL_dynKt;
        hs_rgzgL_dynKt[ni] = h_rgzgL_dynKt;
    }       
    std::string foutname = "~/rootFiles/pt2dscan_ref.root";
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
        
        hs_ktB_dynKt[ni]->Write();
        hs_rgzgB_dynKt[ni]->Write();
        
        hs_ktC_dynKt[ni]->Write();
        hs_rgzgC_dynKt[ni]->Write();
        
        hs_ktL_dynKt[ni]->Write();
        hs_rgzgL_dynKt[ni]->Write();
    }
    fout->Close();
}
