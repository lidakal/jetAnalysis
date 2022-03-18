// This functions creates histograms for the substructure variables zg and Rg
// using the reference jet variables

#include <iostream>
#include "stdio.h"
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"
#include <vector>

void plot_ref_sub() 
{
    const int n = 2;
    
    std::string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    std::string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";
    std::string fnames[n] = {path_qcd, path_bJet};
    
    std::string h_qcd = "h_qcd";
    std::string h_bJet = "h_bJet";
    std::string hnames[n] = {h_qcd, h_bJet};
    
    
    // zg histograms
    TH2F *hs_zgL[n];
    TH2F *hs_zgB[n];
    TH2F *hs_zgC[n];
    
    TH2F *hs_zgL_dynKt[n];
    TH2F *hs_zgB_dynKt[n];
    TH2F *hs_zgC_dynKt[n];
    
    // Rg histograms
    TH2F *hs_rgL[n];
    TH2F *hs_rgB[n];
    TH2F *hs_rgC[n];
    
    TH2F *hs_rgL_dynKt[n];
    TH2F *hs_rgB_dynKt[n];
    TH2F *hs_rgC_dynKt[n];
    

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
        for (auto activeBranchName : {"nref", "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", 
                                     "rsjt2Pt", "rsjt2Eta", "rsjt2Phi", "refpt", 
                                      "jtHadFlav", "refIsHardest", "refeta"}) {
            //std::cout << "Activating branch " << activeBranchName << endl;
            t->SetBranchStatus(activeBranchName, 1);
        }
        HiTree->SetBranchStatus("*", 0);
        HiTree->SetBranchStatus("weight", 1);

        // Create the new histograms
        
        int zgbins = 40;
        float zgmin = 0.1;
        float zgmax = 0.5;
        
        int rgbins = 40;
        float rgmin = 0.;
        float rgmax = 5.;
        
        int ptbins = 27*2;
        float ptmin = 30.;
        float ptmax = 300.;

        // X = zg, Y = refpt
        TH2F *h_zgL = new TH2F((hname + "_zgL").c_str(), "refpt vs zg, l jets", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        TH2F *h_zgB = new TH2F((hname + "_zgB").c_str(), "refpt vs zg, b jets", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        TH2F *h_zgC = new TH2F((hname + "_zgC").c_str(), "refpt vs zg, c jets", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        
        TH2F *h_zgL_dynKt = new TH2F((hname + "_zgL_dynKt").c_str(), "refpt vs zg, l jets, dynKt", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        TH2F *h_zgB_dynKt = new TH2F((hname + "_zgB_dynKt").c_str(), "refpt vs zg, b jets, dynKt", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        TH2F *h_zgC_dynKt = new TH2F((hname + "_zgC_dynKt").c_str(), "refpt vs zg, c jets, dynKt", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        
        // X = Rg, Y = refpt
        TH2F *h_rgL = new TH2F((hname + "_rgL").c_str(), "refpt vs rg, l jets", rgbins, rgmin, rgmax, ptbins, ptmin, ptmax);
        TH2F *h_rgB = new TH2F((hname + "_rgB").c_str(), "refpt vs rg, b jets", rgbins, rgmin, rgmax, ptbins, ptmin, ptmax);
        TH2F *h_rgC = new TH2F((hname + "_rgC").c_str(), "refpt vs rg, c jets", rgbins, rgmin, rgmax, ptbins, ptmin, ptmax);
        
        TH2F *h_rgL_dynKt = new TH2F((hname + "_rgL_dynKt").c_str(), "refpt vs rg, l jets, dynKt", rgbins, rgmin, rgmax, ptbins, ptmin, ptmax);
        TH2F *h_rgB_dynKt = new TH2F((hname + "_rgB_dynKt").c_str(), "refpt vs rg, b jets, dynKt", rgbins, rgmin, rgmax, ptbins, ptmin, ptmax);
        TH2F *h_rgC_dynKt = new TH2F((hname + "_rgC_dynKt").c_str(), "refpt vs rg, c jets, dynKt", rgbins, rgmin, rgmax, ptbins, ptmin, ptmax);

        Long64_t nentries = t->GetEntries();

        std::cout << "Creating histograms..." << endl;
        
        for (Long64_t i = 0; i < nentries; i++) {
            t->GetEntry(i);
            HiTree->GetEntry(i);

            // Print progress
            if (i%1000000 == 0) {
                std::cout << "i = " << i << endl;
            }
            
            // Calculate zg, Rg
            for (Long64_t j = 0; j < nref; j++)  {
                Float_t zg = -1.;
                Float_t rg = -1.;
                Float_t logrg = -1;
                
                
                if (refeta[j] > 2.) { 
                    continue;
                }
                
                if (rsjt2Pt[j] > 0.) {
                    zg = rsjt2Pt[j] / (rsjt2Pt[j]+rsjt1Pt[j]);
                    
                    Float_t deta = rsjt1Eta[j] - rsjt2Eta[j];
                    Float_t dphi = acos(cos(rsjt1Phi[j] - rsjt2Phi[j]));
                    rg = sqrt(deta * deta + dphi * dphi);
                    logrg = log(1/rg);
                }
                
                // Fill the histograms
                if (jtHadFlav[j] == 5) {
                    h_zgB->Fill(zg, refpt[j], weight);
                    h_rgB->Fill(logrg, refpt[j], weight);
                } else if (jtHadFlav[j] == 4) {   
                    h_zgC->Fill(zg, refpt[j], weight);
                    h_rgC->Fill(logrg, refpt[j], weight);
                } else {
                    h_zgL->Fill(zg, refpt[j], weight);
                    h_rgL->Fill(logrg, refpt[j], weight);
                }
                
                // Dynamic grooming : put failing jets in underflow bin
                if (!refIsHardest[j]) {
                    zg = -1;
                    logrg = -1;
                }
                if (jtHadFlav[j] == 5) {
                    h_zgB_dynKt->Fill(zg, refpt[j], weight);
                    h_rgB_dynKt->Fill(logrg, refpt[j], weight);
                } else if (jtHadFlav[j] == 4) { 
                    h_zgC_dynKt->Fill(zg, refpt[j], weight);
                    h_rgC_dynKt->Fill(logrg, refpt[j], weight);
                } else {
                    h_zgL_dynKt->Fill(zg, refpt[j], weight);
                    h_rgL_dynKt->Fill(logrg, refpt[j], weight);
                }
            }
        }
        hs_zgB[ni] = h_zgB;
        hs_rgB[ni] = h_rgB;
        
        hs_zgC[ni] = h_zgC;
        hs_rgC[ni] = h_rgC;
        
        hs_zgL[ni] = h_zgL;
        hs_rgL[ni] = h_rgL;
        
        hs_zgB_dynKt[ni] = h_zgB_dynKt;
        hs_rgB_dynKt[ni] = h_rgB_dynKt;
        
        hs_zgC_dynKt[ni] = h_zgC_dynKt;
        hs_rgC_dynKt[ni] = h_rgC_dynKt;
        
        hs_zgL_dynKt[ni] = h_zgL_dynKt;
        hs_rgL_dynKt[ni] = h_rgL_dynKt;
    }       
    std::string foutname = "ref_sub.root";
    std::cout << "\n(Re)creating file " << foutname << endl;
    TFile *fout = new TFile(foutname.c_str(),  "recreate");
    std::cout << "Saving histograms." << endl;
    
    for (int ni = 0; ni < n; ni++) {
        hs_zgB[ni]->Write();
        hs_rgB[ni]->Write();
        
        hs_zgC[ni]->Write();
        hs_rgC[ni]->Write();
        
        hs_zgL[ni]->Write();
        hs_rgL[ni]->Write();
        
        hs_zgB_dynKt[ni]->Write();
        hs_rgB_dynKt[ni]->Write();
        
        hs_zgC_dynKt[ni]->Write();
        hs_rgC_dynKt[ni]->Write();
        
        hs_zgL_dynKt[ni]->Write();
        hs_rgL_dynKt[ni]->Write();
    }
    fout->Close();
}
