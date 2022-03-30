#include "TFile.h"
#include "TTree.h"
#include <iostream>
    
using namespace std;
    
// Count how many of the actual b's are tagged as b's
vector<vector<Float_t>> bTagEff() 
{
    string path_incl = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/chargedSJ/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/chargedSJ/merged_HiForestAOD.root";
    TFile *f;
    TTree *t;
    TTree *HiTree;
    
    f = new TFile(path_bJet.c_str());
    t = (TTree *) f->Get("ak4PFJetAnalyzer/t");
    HiTree = (TTree *) f->Get("hiEvtAnalyzer/HiTree");
    
    Int_t           nref;
    Float_t         refeta[30];
    Float_t         refpt[30];
    Float_t         jtDiscCSVV2[30];
    Float_t         jtDiscDeepCSVB[30];
    Float_t         jtDiscDeepCSVBB[30];
    Float_t         jtDiscDeepCSVC[30];
    Float_t         jtDiscDeepFlavourB[30];
    Float_t         jtDiscDeepFlavourBB[30];
    Float_t         jtDiscDeepFlavourLEPB[30];
    Float_t         jtDiscDeepFlavourC[30];
    Float_t         jtDiscProb[30];
    Float_t         jtHadFlav[30];
    
    
    t->SetBranchAddress("nref", &nref);
    t->SetBranchAddress("refeta", refeta);
    t->SetBranchAddress("refpt", refpt);
    t->SetBranchAddress("jtHadFlav", jtHadFlav);
    
    t->SetBranchAddress("jtDiscCSVV2", jtDiscCSVV2);
    t->SetBranchAddress("jtDiscDeepCSVB", jtDiscDeepCSVB);
    t->SetBranchAddress("jtDiscDeepCSVBB", jtDiscDeepCSVBB);
    t->SetBranchAddress("jtDiscDeepCSVC", jtDiscDeepCSVC);
    t->SetBranchAddress("jtDiscDeepFlavourB", jtDiscDeepFlavourB);
    t->SetBranchAddress("jtDiscDeepFlavourBB", jtDiscDeepFlavourBB);
    t->SetBranchAddress("jtDiscDeepFlavourLEPB", jtDiscDeepFlavourLEPB);
    t->SetBranchAddress("jtDiscDeepFlavourC", jtDiscDeepFlavourC);
    t->SetBranchAddress("jtDiscProb", jtDiscProb);
    
    Long64_t nentries = t->GetEntries();
    
    t->SetBranchStatus("*", 0);
    
    t->SetBranchStatus("nref", 1);
    t->SetBranchStatus("refeta", 1);
    t->SetBranchStatus("refpt", 1);
    t->SetBranchStatus("jtHadFlav", 1);
    
    t->SetBranchStatus("jtDiscCSVV2", 1);
    t->SetBranchStatus("jtDiscDeepCSVB", 1);
    t->SetBranchStatus("jtDiscDeepCSVBB", 1);
    t->SetBranchStatus("jtDiscDeepCSVC", 1);
    t->SetBranchStatus("jtDiscDeepFlavourB", 1);
    t->SetBranchStatus("jtDiscDeepFlavourBB", 1);
    t->SetBranchStatus("jtDiscDeepFlavourLEPB", 1);
    t->SetBranchStatus("jtDiscDeepFlavourC", 1);
    t->SetBranchStatus("jtDiscProb", 1);

    const Int_t ntaggers = 3;
    const Int_t nwps = 9;

    Float_t wp_probs[nwps] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    Float_t wp_jetpt = 100.;
    
    Float_t actualBs = 0;

    Float_t taggedBsCSVV2[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    Float_t taggedBsDeepCSV[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    Float_t taggedBsDeepFlavour[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (Long64_t i = 0; i < nentries; i++) {
        // Print progress
        if (i%1000000 == 0) {
            cout << "i = " << i << endl;
        }

        t->GetEntry(i);
        for (Int_t j = 0; j < nref; j++) {
            if ((abs(refeta[j]) > 2) || (refpt[j] < wp_jetpt)) {
                continue;
            }

            // count real B's
            if (jtHadFlav[j] == 5) {
                actualBs += 1;
            } else { 
                continue; // don't count tagged b's when they're not b's 
            }
            
            Float_t probCSVV2_B = jtDiscCSVV2[j];
            Float_t probDeepCSV_B = jtDiscDeepCSVB[j] + jtDiscDeepCSVBB[j];
            Float_t probDeepFlavour_B = jtDiscDeepFlavourB[j] + jtDiscDeepFlavourBB[j] + jtDiscDeepFlavourLEPB[j];
            
            // count how many jets pass each wp for each tagger
            for (Int_t k = 0; k < nwps; k++) {
                if (probCSVV2_B >= wp_probs[k]) {
                    taggedBsCSVV2[k] += 1;
                }
                
                if (probDeepCSV_B >= wp_probs[k]) {
                    taggedBsDeepCSV[k] += 1;
                }
                
                if (probDeepFlavour_B >= wp_probs[k]) {
                    taggedBsDeepFlavour[k] += 1;
                }
            }
        }
    }
    
    vector<vector<Float_t>> efficiencies(ntaggers);

    for (Int_t k = 0; k < nwps; k ++) { 
        Float_t efficiencyCSVV2 = taggedBsCSVV2[k] / actualBs;
        Float_t efficiencyDeepCSV = taggedBsDeepCSV[k] / actualBs;
        Float_t efficiencyDeepFlavour = taggedBsDeepFlavour[k] / actualBs;

        efficiencies[0].push_back(efficiencyCSVV2);
        efficiencies[1].push_back(efficiencyDeepCSV);
        efficiencies[2].push_back(efficiencyDeepFlavour);
    }
    
    return efficiencies;
}

// count how many of the actual C's are tagged as B's
void missIdC(Float_t wp_prob, Float_t wp_jetpt) 
{
    string path_incl = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/chargedSJ/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/chargedSJ/merged_HiForestAOD.root";
    TFile *f;
    TTree *t;
    TTree *HiTree;
    
    f = new TFile(path_incl.c_str());
    t = (TTree *) f->Get("ak4PFJetAnalyzer/t");
    HiTree = (TTree *) f->Get("hiEvtAnalyzer/HiTree");
    
    Int_t           nref;
    Float_t         refeta[30];
    Float_t         refpt[30];
    Float_t         jtDiscCSVV2[30];
    Float_t         jtDiscDeepCSVB[30];
    Float_t         jtDiscDeepCSVBB[30];
    Float_t         jtDiscDeepCSVC[30];
    Float_t         jtDiscDeepFlavourB[30];
    Float_t         jtDiscDeepFlavourBB[30];
    Float_t         jtDiscDeepFlavourLEPB[30];
    Float_t         jtDiscDeepFlavourC[30];
    Float_t         jtDiscProb[30];
    Float_t         jtHadFlav[30];
    
    
    t->SetBranchAddress("nref", &nref);
    t->SetBranchAddress("refeta", refeta);
    t->SetBranchAddress("refpt", refpt);
    t->SetBranchAddress("jtHadFlav", jtHadFlav);
    
    t->SetBranchAddress("jtDiscCSVV2", jtDiscCSVV2);
    t->SetBranchAddress("jtDiscDeepCSVB", jtDiscDeepCSVB);
    t->SetBranchAddress("jtDiscDeepCSVBB", jtDiscDeepCSVBB);
    t->SetBranchAddress("jtDiscDeepCSVC", jtDiscDeepCSVC);
    t->SetBranchAddress("jtDiscDeepFlavourB", jtDiscDeepFlavourB);
    t->SetBranchAddress("jtDiscDeepFlavourBB", jtDiscDeepFlavourBB);
    t->SetBranchAddress("jtDiscDeepFlavourLEPB", jtDiscDeepFlavourLEPB);
    t->SetBranchAddress("jtDiscDeepFlavourC", jtDiscDeepFlavourC);
    t->SetBranchAddress("jtDiscProb", jtDiscProb);
    
    Long64_t nentries = t->GetEntries();
    
    t->SetBranchStatus("*", 0);
    
    t->SetBranchStatus("nref", 1);
    t->SetBranchStatus("refeta", 1);
    t->SetBranchStatus("refpt", 1);
    t->SetBranchStatus("jtHadFlav", 1);
    
    t->SetBranchStatus("jtDiscCSVV2", 1);
    t->SetBranchStatus("jtDiscDeepCSVB", 1);
    t->SetBranchStatus("jtDiscDeepCSVBB", 1);
    t->SetBranchStatus("jtDiscDeepCSVC", 1);
    t->SetBranchStatus("jtDiscDeepFlavourB", 1);
    t->SetBranchStatus("jtDiscDeepFlavourBB", 1);
    t->SetBranchStatus("jtDiscDeepFlavourLEPB", 1);
    t->SetBranchStatus("jtDiscDeepFlavourC", 1);
    t->SetBranchStatus("jtDiscProb", 1);
    
    Float_t actualCs = 0;
    
    Float_t taggedBsCSVV2 = 0;
    Float_t taggedBsDeepCSV = 0;
    Float_t taggedBsDeepFlavour = 0;

    for (Long64_t i = 0; i < nentries; i++) {
        // Print progress
        if (i%1000000 == 0) {
            cout << "i = " << i << endl;
        }

        t->GetEntry(i);
        for (Int_t j = 0; j < nref; j++) {
            if ((abs(refeta[j]) > 2) || (refpt[j] < wp_jetpt)) {
                continue;
            }
            
            // count real C's and L's
            if (jtHadFlav[j] == 4) {
                actualCs += 1;
            } else {
                continue;
            }

            Float_t probCSVV2_B = jtDiscCSVV2[j];
            Float_t probDeepCSV_B = jtDiscDeepCSVB[j] + jtDiscDeepCSVBB[j];
            Float_t probDeepFlavour_B = jtDiscDeepFlavourB[j] + jtDiscDeepFlavourBB[j] + jtDiscDeepFlavourLEPB[j];
            
            // count how many jets pass the wp for each tagger
            if (probCSVV2_B >= wp_prob) {
                taggedBsCSVV2 += 1;
            }
            
            if (probDeepCSV_B >= wp_prob) {
                taggedBsDeepCSV += 1;
            }
            
            if (probDeepFlavour_B >= wp_prob) {
                taggedBsDeepFlavour += 1;
            }
        }
    }
    
    Float_t efficiency = 0;
    
    cout << "For working point p > " << wp_prob << " and jet pt > " << wp_jetpt << ":\n" << endl; 
    
    efficiency = taggedBsCSVV2 / actualCs;
    cout << "CSVV2 C misidentification rate: " << efficiency << endl;
    
    efficiency = taggedBsDeepCSV / actualCs;
    cout << "DeepCSV C misidentification rate: " << efficiency << endl;
    
    efficiency = taggedBsDeepFlavour / actualCs;
    cout << "DeepFlavour C misidentification rate: " << efficiency << endl;
}

// count how many of the actual light jets are tagged as B's
void missIdL(Float_t wp_prob, Float_t wp_jetpt) 
{
    string path_incl = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/chargedSJ/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/chargedSJ/merged_HiForestAOD.root";
    TFile *f;
    TTree *t;
    TTree *HiTree;
    
    f = new TFile(path_incl.c_str());
    t = (TTree *) f->Get("ak4PFJetAnalyzer/t");
    HiTree = (TTree *) f->Get("hiEvtAnalyzer/HiTree");
    
    Int_t           nref;
    Float_t         refeta[30];
    Float_t         refpt[30];
    Float_t         jtDiscCSVV2[30];
    Float_t         jtDiscDeepCSVB[30];
    Float_t         jtDiscDeepCSVBB[30];
    Float_t         jtDiscDeepCSVC[30];
    Float_t         jtDiscDeepFlavourB[30];
    Float_t         jtDiscDeepFlavourBB[30];
    Float_t         jtDiscDeepFlavourLEPB[30];
    Float_t         jtDiscDeepFlavourC[30];
    Float_t         jtDiscProb[30];
    Float_t         jtHadFlav[30];
    
    
    t->SetBranchAddress("nref", &nref);
    t->SetBranchAddress("refeta", refeta);
    t->SetBranchAddress("refpt", refpt);
    t->SetBranchAddress("jtHadFlav", jtHadFlav);
    
    t->SetBranchAddress("jtDiscCSVV2", jtDiscCSVV2);
    t->SetBranchAddress("jtDiscDeepCSVB", jtDiscDeepCSVB);
    t->SetBranchAddress("jtDiscDeepCSVBB", jtDiscDeepCSVBB);
    t->SetBranchAddress("jtDiscDeepCSVC", jtDiscDeepCSVC);
    t->SetBranchAddress("jtDiscDeepFlavourB", jtDiscDeepFlavourB);
    t->SetBranchAddress("jtDiscDeepFlavourBB", jtDiscDeepFlavourBB);
    t->SetBranchAddress("jtDiscDeepFlavourLEPB", jtDiscDeepFlavourLEPB);
    t->SetBranchAddress("jtDiscDeepFlavourC", jtDiscDeepFlavourC);
    t->SetBranchAddress("jtDiscProb", jtDiscProb);
    
    Long64_t nentries = t->GetEntries();
    
    t->SetBranchStatus("*", 0);
    
    t->SetBranchStatus("nref", 1);
    t->SetBranchStatus("refeta", 1);
    t->SetBranchStatus("refpt", 1);
    t->SetBranchStatus("jtHadFlav", 1);
    
    t->SetBranchStatus("jtDiscCSVV2", 1);
    t->SetBranchStatus("jtDiscDeepCSVB", 1);
    t->SetBranchStatus("jtDiscDeepCSVBB", 1);
    t->SetBranchStatus("jtDiscDeepCSVC", 1);
    t->SetBranchStatus("jtDiscDeepFlavourB", 1);
    t->SetBranchStatus("jtDiscDeepFlavourBB", 1);
    t->SetBranchStatus("jtDiscDeepFlavourLEPB", 1);
    t->SetBranchStatus("jtDiscDeepFlavourC", 1);
    t->SetBranchStatus("jtDiscProb", 1);
    
    Float_t actualLs = 0;
    
    Float_t taggedBsCSVV2 = 0;
    Float_t taggedBsDeepCSV = 0;
    Float_t taggedBsDeepFlavour = 0;

    for (Long64_t i = 0; i < nentries; i++) {
        // Print progress
        if (i%1000000 == 0) {
            cout << "i = " << i << endl;
        }

        t->GetEntry(i);
        for (Int_t j = 0; j < nref; j++) {
            if ((abs(refeta[j]) > 2) || (refpt[j] < wp_jetpt)) {
                continue;
            }
            
            // count real C's and L's
            if (jtHadFlav[j] == 0) {
                actualLs += 1;
            } else {
                continue;
            }

            Float_t probCSVV2_B = jtDiscCSVV2[j];
            Float_t probDeepCSV_B = jtDiscDeepCSVB[j] + jtDiscDeepCSVBB[j];
            Float_t probDeepFlavour_B = jtDiscDeepFlavourB[j] + jtDiscDeepFlavourBB[j] + jtDiscDeepFlavourLEPB[j];
            
            // count how many jets pass the wp for each tagger
            if (probCSVV2_B >= wp_prob) {
                taggedBsCSVV2 += 1;
            }
            
            if (probDeepCSV_B >= wp_prob) {
                taggedBsDeepCSV += 1;
            }
            
            if (probDeepFlavour_B >= wp_prob) {
                taggedBsDeepFlavour += 1;
            }
        }
    }
    
    Float_t efficiency = 0;
    
    cout << "For working point p > " << wp_prob << " and jet pt > " << wp_jetpt << ":\n" << endl; 
    
    efficiency = taggedBsCSVV2 / actualLs;
    cout << "CSVV2 L misidentification rate: " << efficiency << endl;
    
    efficiency = taggedBsDeepCSV / actualLs;
    cout << "DeepCSV L misidentification rate: " << efficiency << endl;
    
    efficiency = taggedBsDeepFlavour / actualLs;
    cout << "DeepFlavour L misidentification rate: " << efficiency << endl;
}