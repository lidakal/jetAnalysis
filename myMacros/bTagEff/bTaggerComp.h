#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TLatex.h"
#include <iostream>
#include <vector>
    
using namespace std;
//using namespace vector;
    
// Count how many of the actual b's are tagged as b's
vector<vector<Float_t>> bTagEff() 
{
    string path_incl = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";
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
    
    Float_t         weight;
    
    HiTree->SetBranchAddress("weight", &weight);
    
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

    HiTree->SetBranchStatus("*", 0);
    HiTree->SetBranchStatus("weight", 1);
    
    const Int_t ntaggers = 3;
    const Int_t nwps = 14;

    Float_t wp_probs[nwps] = {0., 0.01, 0.03, 0.05, 0.07, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    Float_t wp_jetpt = 100.;
    
    Float_t actualBs = 0;

    Float_t taggedBsCSVV2[nwps] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Float_t taggedBsDeepCSV[nwps] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Float_t taggedBsDeepFlavour[nwps] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (Long64_t i = 0; i < nentries; i++) {
        // Print progress
        if (i%1000000 == 0) {
            cout << "i = " << i << endl;
        }

        t->GetEntry(i);
        HiTree->GetEntry(i);
        
        for (Int_t j = 0; j < nref; j++) {
            if ((abs(refeta[j]) > 2) || (refpt[j] < wp_jetpt)) {
                continue;
            }

            // count real B's
            if (jtHadFlav[j] == 5) {
                actualBs += weight;
            } else { 
                continue; // don't count tagged b's when they're not b's 
            }
            
            Float_t probCSVV2_B = jtDiscCSVV2[j];
            Float_t probDeepCSV_B = jtDiscDeepCSVB[j] + jtDiscDeepCSVBB[j];
            Float_t probDeepFlavour_B = jtDiscDeepFlavourB[j] + jtDiscDeepFlavourBB[j] + jtDiscDeepFlavourLEPB[j];
            
            // count how many jets pass each wp for each tagger
            for (Int_t k = 0; k < nwps; k++) {
                if (probCSVV2_B >= wp_probs[k]) {
                    taggedBsCSVV2[k] += weight;
                }
                
                if (probDeepCSV_B >= wp_probs[k]) {
                    taggedBsDeepCSV[k] += weight;
                }
                
                if (probDeepFlavour_B >= wp_probs[k]) {
                    taggedBsDeepFlavour[k] += weight;
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

// count how many of the actual C's and L's are tagged as B's
vector<vector<Float_t>> missIdC() 
{
    string path_incl = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";
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
    
    Float_t         weight;
    
    HiTree->SetBranchAddress("weight", &weight);
    
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
    
    HiTree->SetBranchStatus("*", 0);
    HiTree->SetBranchStatus("weight", 1);
    
    Float_t actualCs = 0;
    
    const Int_t ntaggers = 3;
    const Int_t nwps = 14;

    Float_t wp_probs[nwps] = {0., 0.01, 0.03, 0.05, 0.07, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    Float_t wp_jetpt = 100.;
    
    Float_t taggedBsCSVV2[nwps] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Float_t taggedBsDeepCSV[nwps] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Float_t taggedBsDeepFlavour[nwps] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (Long64_t i = 0; i < nentries; i++) {
        // Print progress
        if (i%1000000 == 0) {
            cout << "i = " << i << endl;
        }

        t->GetEntry(i);
        HiTree->GetEntry(i);
        
        for (Int_t j = 0; j < nref; j++) {
            if ((abs(refeta[j]) > 2) || (refpt[j] < wp_jetpt)) {
                continue;
            }
            
            // count real C's 
            if (jtHadFlav[j] == 4) {
                actualCs += weight;
            } else {
                continue;
            }

            Float_t probCSVV2_B = jtDiscCSVV2[j];
            Float_t probDeepCSV_B = jtDiscDeepCSVB[j] + jtDiscDeepCSVBB[j];
            Float_t probDeepFlavour_B = jtDiscDeepFlavourB[j] + jtDiscDeepFlavourBB[j] + jtDiscDeepFlavourLEPB[j];
            
            // count how many jets pass each wp for each tagger
            for (Int_t k = 0; k < nwps; k++) {
                if (probCSVV2_B >= wp_probs[k]) {
                    taggedBsCSVV2[k] += weight;
                }

                if (probDeepCSV_B >= wp_probs[k]) {
                    taggedBsDeepCSV[k] += weight;
                }

                if (probDeepFlavour_B >= wp_probs[k]) {
                    taggedBsDeepFlavour[k] += weight;
                }
            }
        }
    }
    
    vector<vector<Float_t>> efficiencies(ntaggers);
    
    for (Int_t k = 0; k < nwps; k ++) { 
        Float_t efficiencyCSVV2 = taggedBsCSVV2[k] / actualCs;
        Float_t efficiencyDeepCSV = taggedBsDeepCSV[k] / actualCs;
        Float_t efficiencyDeepFlavour = taggedBsDeepFlavour[k] / actualCs;

        efficiencies[0].push_back(efficiencyCSVV2);
        efficiencies[1].push_back(efficiencyDeepCSV);
        efficiencies[2].push_back(efficiencyDeepFlavour);
    }
    
    return efficiencies;
}

// count how many of the actual light jets are tagged as B's
vector<vector<Float_t>> missIdL() 
{
    string path_incl = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";
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
    
    Float_t         weight;
    
    HiTree->SetBranchAddress("weight", &weight);
    
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
    
    HiTree->SetBranchStatus("*", 0);
    HiTree->SetBranchStatus("weight", 1);
    
    Float_t actualLs = 0;
    
    const Int_t ntaggers = 3;
    const Int_t nwps = 14;

    Float_t wp_probs[nwps] = {0., 0.01, 0.03, 0.05, 0.07, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
    Float_t wp_jetpt = 100.;
    
    Float_t taggedBsCSVV2[nwps] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Float_t taggedBsDeepCSV[nwps] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Float_t taggedBsDeepFlavour[nwps] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (Long64_t i = 0; i < nentries; i++) {
        // Print progress
        if (i%1000000 == 0) {
            cout << "i = " << i << endl;
        }

        t->GetEntry(i);
        HiTree->GetEntry(i);
        
        for (Int_t j = 0; j < nref; j++) {
            if ((abs(refeta[j]) > 2) || (refpt[j] < wp_jetpt)) {
                continue;
            }
            
            // count real C's and L's
            if (jtHadFlav[j] == 0) {
                actualLs += weight;
            } else {
                continue;
            }

            Float_t probCSVV2_B = jtDiscCSVV2[j];
            Float_t probDeepCSV_B = jtDiscDeepCSVB[j] + jtDiscDeepCSVBB[j];
            Float_t probDeepFlavour_B = jtDiscDeepFlavourB[j] + jtDiscDeepFlavourBB[j] + jtDiscDeepFlavourLEPB[j];
            
            // count how many jets pass each wp for each tagger
            for (Int_t k = 0; k < nwps; k++) {
                if (probCSVV2_B >= wp_probs[k]) {
                    taggedBsCSVV2[k] += weight;
                }

                if (probDeepCSV_B >= wp_probs[k]) {
                    taggedBsDeepCSV[k] += weight;
                }

                if (probDeepFlavour_B >= wp_probs[k]) {
                    taggedBsDeepFlavour[k] += weight;
                }
            }
        }
    }
    
    vector<vector<Float_t>> efficiencies(ntaggers);
    
    for (Int_t k = 0; k < nwps; k ++) { 
        Float_t efficiencyCSVV2 = taggedBsCSVV2[k] / actualLs;
        Float_t efficiencyDeepCSV = taggedBsDeepCSV[k] / actualLs;
        Float_t efficiencyDeepFlavour = taggedBsDeepFlavour[k] / actualLs;

        efficiencies[0].push_back(efficiencyCSVV2);
        efficiencies[1].push_back(efficiencyDeepCSV);
        efficiencies[2].push_back(efficiencyDeepFlavour);
    }
    
    return efficiencies;
}

void plotROCcurve()
{
    cout << "Calculating b-tag efficiency. " << endl;
    vector<vector<Float_t>> effB = bTagEff();
    
    cout << "Calculating C misidentification rate. " << endl;
    vector<vector<Float_t>> effC = missIdC();
    
    cout << "Calculating L misidentification rate. " << endl;
    vector<vector<Float_t>> effL = missIdL();
    
    TFile *fout = new TFile("ROCcurves.root", "recreate");
    TTree *tree = new TTree("tree", "b tagger efficiencies");
    
    tree->Branch("effB_CSVV2", effB[0].data(), "effB_CSVV2[14]/F");
    tree->Branch("effC_CSVV2", effC[0].data(), "effC_CSVV2[14]/F");
    tree->Branch("effL_CSVV2", effL[0].data(), "effL_CSVV2[14]/F");
    
    tree->Branch("effB_DeepCSV", effB[1].data(), "effB_DeepCSV[14]/F");
    tree->Branch("effC_DeepCSV", effC[1].data(), "effC_DeepCSV[14]/F");
    tree->Branch("effL_DeepCSV", effL[1].data(), "effL_DeepCSV[14]/F");
    
    tree->Branch("effB_DeepFlavour", effB[2].data(), "effB_DeepFlavour[14]/F");
    tree->Branch("effC_DeepFlavour", effC[2].data(), "effC_DeepFlavour[14]/F");
    tree->Branch("effL_DeepFlavour", effL[2].data(), "effL_DeepFlavour[14]/F");
    
    tree->Fill();
    tree->Write();
    
    fout->Close();
    
}

void drawROCcurve()
{
    TFile *fin = new TFile("ROCcurves.root");
    TTree *tree = (TTree *) fin->Get("tree");
    
    const Int_t nwps = 14;
    
    Float_t effB_CSVV2[nwps];
    Float_t effC_CSVV2[nwps];
    Float_t effL_CSVV2[nwps];
    
    Float_t effB_DeepCSV[nwps];
    Float_t effC_DeepCSV[nwps];
    Float_t effL_DeepCSV[nwps];
    
    Float_t effB_DeepFlavour[nwps];
    Float_t effC_DeepFlavour[nwps];
    Float_t effL_DeepFlavour[nwps];
    
    tree->SetBranchAddress("effB_CSVV2", effB_CSVV2);
    tree->SetBranchAddress("effC_CSVV2", effC_CSVV2);
    tree->SetBranchAddress("effL_CSVV2", effL_CSVV2);
    
    tree->SetBranchAddress("effB_DeepCSV", effB_DeepCSV);
    tree->SetBranchAddress("effC_DeepCSV", effC_DeepCSV);
    tree->SetBranchAddress("effL_DeepCSV", effL_DeepCSV);
    
    tree->SetBranchAddress("effB_DeepFlavour", effB_DeepFlavour);
    tree->SetBranchAddress("effC_DeepFlavour", effC_DeepFlavour);
    tree->SetBranchAddress("effL_DeepFlavour", effL_DeepFlavour);
    
    tree->GetEntry(0);
    
    TCanvas *c = new TCanvas("c","b-tagger comparison", 900, 900);
    c->SetLogy();
    
    TLegend *leg = new TLegend(0.15, 0.7, 0.8, 0.9);
    gStyle->SetLegendTextSize(18);
    
    TGraph *gr1 = new TGraph(nwps, effB_CSVV2, effC_CSVV2);
    gr1->SetMarkerStyle(kFullCircle);
    gr1->SetMarkerColor(kRed+2);
    gr1->SetLineStyle(kSolid);
    gr1->SetLineColor(kRed+2);
    leg->AddEntry(gr1, "CSVV2, B vs C", "pl");
    
    TGraph *gr2 = new TGraph(nwps, effB_CSVV2, effL_CSVV2);
    gr2->SetMarkerStyle(kCircle);
    gr2->SetMarkerColor(kRed+2);
    gr2->SetLineStyle(kDashed);
    gr2->SetLineColor(kRed+2);
    leg->AddEntry(gr2, "CSVV2, B vs L", "pl");
    
    TGraph *gr3 = new TGraph(nwps, effB_DeepCSV, effC_DeepCSV);
    gr3->SetMarkerStyle(kFullSquare);
    gr3->SetMarkerColor(kGreen+2);
    gr3->SetLineStyle(kSolid);
    gr3->SetLineColor(kGreen+2);
    leg->AddEntry(gr3, "DeepCSV, B vs C", "pl");
    
    TGraph *gr4 = new TGraph(nwps, effB_DeepCSV, effL_DeepCSV);
    gr4->SetMarkerStyle(kOpenSquare);
    gr4->SetMarkerColor(kGreen+2);
    gr4->SetLineStyle(kDashed);
    gr4->SetLineColor(kGreen+2);
    leg->AddEntry(gr4, "DeepCSV, B vs L", "pl");
    
    TGraph *gr5 = new TGraph(nwps, effB_DeepFlavour, effC_DeepFlavour);
    gr5->SetMarkerStyle(kFullTriangleUp);
    gr5->SetMarkerColor(kBlue+2);
    gr5->SetLineStyle(kSolid);
    gr5->SetLineColor(kBlue+2);
    leg->AddEntry(gr5, "DeepFlavour, B vs C", "pl");
    
    TGraph *gr6 = new TGraph(nwps, effB_DeepFlavour, effL_DeepFlavour);
    gr6->SetMarkerStyle(kOpenTriangleUp);
    gr6->SetMarkerColor(kBlue+2);
    gr6->SetLineStyle(kDashed);
    gr6->SetLineColor(kBlue+2);
    leg->AddEntry(gr6, "DeepFlavour, B vs L", "pl");
    
    
    gr1->Draw();
    gr2->Draw("PL same");
    gr3->Draw("PL same");
    gr4->Draw("PL same");
    gr5->Draw("PL same");
    gr6->Draw("PL same");
    
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->Draw();
    
    gr1->GetYaxis()->SetRangeUser(0, 1);
    gr1->GetXaxis()->SetRangeUser(0, 1);
    
    gr1->GetYaxis()->SetTitle("Misidentification rate");
    gr1->GetXaxis()->SetTitle("b-tag Efficiency");
    
    // Draw Title
    TLatex *title = new TLatex();
    title->SetTextSize(25);
    title->DrawLatexNDC(0.4, 0.94, "b-tagger comparison");
    
    c->Draw();
    c->Print("btagcomp.png");
}

