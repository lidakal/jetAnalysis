//Substructure.h

#include <vector>
#include "TFile.h"
#include "TTree.h"

namespace Substructure
{
    class TreeAnalyzer;
    
}

// Look for str1 in str2
//return true for found and false for not found
bool inString(std::string str1, std::string str2) 
{
    bool found = false;
    if (str2.find(str1) != std::string::npos) { 
        found = true;
    }
    return found;
}
    

class TreeAnalyzer {
    public:
        bool qcdMC = false;
        TFile *fin; 
        TTree *t;
        TTree *HiTree;
    
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

        // Constructors
        TreeAnalyzer(bool qcd = true, bool init = true);
        TreeAnalyzer(std::string fpath, bool init = true);
    
        // Class methods
        void Init();
        void SetAnalysisLevel(bool parORref);
        void SetExcludeGSP(bool excl = true);
    
    private:
        bool exclGSP = false;
        Int_t *njets;
        Float_t *jetpt;
        Float_t *jeteta;
        Float_t *jetFlav;
        
};

// qcd = true for qcdMC, qcd = false for bJetMC
TreeAnalyzer::TreeAnalyzer(bool qcd = true, bool init = true) 
{
    std::string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    std::string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";

    if (qcd) {
        fin = new TFile(path_qcd.c_str());
        qcdMC = true;
    } else {
        fin = new TFile(path_bJet.c_str());
        qcdMC = false;
    }
    
    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    HiTree = (TTree *) fin->Get("hiEvtAnalyzer/HiTree");
    
    if (init) {
        Init();
    }
}

TreeAnalyzer::TreeAnalyzer(std::string fpath, bool init = true) 
{
    fin = new TFile(fpath.c_str());
    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    HiTree = (TTree *) fin->Get("hiEvtAnalyzer/HiTree");
    
    if (inString("qcd", fpath) {
        qcdMC = true;
    }
    
    if (init) {
        Init();
    }
}

void TreeAnalyzer::Init() 
{
    // Set branch addresses for t tree
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

    // Set branch addressses for HiTree
    HiTree->SetBranchAddress("run", &run);
    HiTree->SetBranchAddress("evt", &evt);
    HiTree->SetBranchAddress("lumi", &lumi);
    HiTree->SetBranchAddress("vx", &vx);
    HiTree->SetBranchAddress("vy", &vy);
    HiTree->SetBranchAddress("vz", &vz);
    HiTree->SetBranchAddress("pthat", &pthat);
    HiTree->SetBranchAddress("weight", &weight);
}
        
void TreeAnalyzer::SetAnalysisLevel(bool parORref)
{
    if (parORref) {
        jetpt = 


}

        
void TreeAnalyzer::SetExcludeGSP(bool excl = true)
{
    exclGSP = excl;
}
        
void TreeAnalyzer::makeHists(bool parORref = true) 
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
        
        t->SetBranchStatus("*", 0);
        
        // Parton or hadron (ref) level?
        const int nbranches = 11;
        std::string activeBranches[nbranches];
        
        if (parORref) {
            activeBranches = {"npar", "pareta", "parpt", "parNb", "parNc", 
                              "psjt1Pt", "psjt1Eta", "psjt1Phi",
                              "psjt2Pt", "psjt2Eta", "psjt2Phi"};               
        } else {        
            activeBranches = {"nref", "refeta", "refpt", "jtHadFlav", "refIsHardest",
                              "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", 
                              "rsjt2Pt", "rsjt2Eta", "rsjt2Phi"};
        }
    
        for (std::string activeBranchName : activeBranches) {
            t->SetBranchStatus(activeBranchName.c_str(), 1);
        }
    
        HiTree->SetBranchStatus("*", 0);
        HiTree->SetBranchStatus("weight", 1);

        // Create the new histograms
        
        int zgbins = 40;
        float zgmin = 0.;
        float zgmax = 0.5;
        
        int rgbins = 40;
        float rgmin = 0.;
        float rgmax = 0.5;
        
        int ptbins = 27*2;
        float ptmin = 30.;
        float ptmax = 300.;

        // X = zg, Y = parpt
        TH2F *h_zgL = new TH2F((hname + "_zgL").c_str(), "refpt vs zg, l jets", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        TH2F *h_zgB = new TH2F((hname + "_zgB").c_str(), "refpt vs zg, b jets", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        TH2F *h_zgC = new TH2F((hname + "_zgC").c_str(), "refpt vs zg, c jets", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        
        TH2F *h_zgL_dynKt = new TH2F((hname + "_zgL_dynKt").c_str(), "refpt vs zg, l jets, dynKt", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        TH2F *h_zgB_dynKt = new TH2F((hname + "_zgB_dynKt").c_str(), "refpt vs zg, b jets, dynKt", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        TH2F *h_zgC_dynKt = new TH2F((hname + "_zg_dynKtC").c_str(), "refpt vs zg, c jets, dynKt", zgbins, zgmin, zgmax, ptbins, ptmin, ptmax);
        
        // X = Rg, Y = parpt
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
            
            Int_t njets = 0;
            Double_t jeteta;
            Double_t jetpt;
            Int_t jetflav = 0;
            bool isHardest = false;
            
            Double_t pt1;
            Double_t eta1;
            Double_t phi1;
            Double_t pt2;
            Double_t eta2;
            Double_t phi2; 
            
            if (parORref) {
                njets = npar;
            } else {
                njets = nref;
            }
            
            // Calculate zg, Rg
            for (Long64_t j = 0; j < njets; j++)  {                
                if (parORref) {
                    jeteta = pareta[j];
                    jetpt = parpt[j];
                    pt1 = psjt1Pt[j];
                    eta1 = psjt1Eta[j];
                    phi1 = psjt1Phi[j];
                    pt1 = psjt1Pt[j];
                    eta1 = psjt1Eta[j];
                    phi1 = psjt1Phi[j];
                    if (parNb[j] > 0) {
                        jetflav = 5;
                    } else if (parNc[j] > 0) {
                        jetflav = 4;
                    }
                } else {
                    jeteta = refeta[j];
                    jetpt = refpt[j];
                    pt1 = rsjt1Pt[j];
                    eta1 = rsjt1Eta[j];
                    phi1 = rsjt1Phi[j];
                    pt1 = rsjt1Pt[j];
                    eta1 = rsjt1Eta[j];
                    phi1 = rsjt1Phi[j];
                    jetflav = jtHadFlav[j];
                    isHardest = refIsHardest[j];
                }
                    
                Float_t zg = -1.;
                Float_t rg = -1.;
                if (jeteta > 2.) { 
                    continue;
                }
                
                if (pt2 > 0.) {
                    zg = pt2 / (pt2 + pt1);
                    Float_t deta = eta1 - eta2;
                    Float_t dphi = acos(cos(phi1 - phi2));
                    rg = sqrt(deta * deta + dphi * dphi);
                }
                    
                // Fill the histograms
                    
                if (jetflav == 5) {
                    h_zgB->Fill(zg, jetpt, weight);
                    h_rgB->Fill(rg, jetpt, weight);
                    if ((!parORref) && isHardest) {
                        h_zgB_dynKt->Fill(zg, jetpt, weight);
                        h_rgB_dynKt->Fill(rg, refpt, weight);
                    }
                } else if (jetflav == 4) {
                    h_zgC->Fill(zg, jetpt, weight);
                    h_rgC->Fill(rg, jetpt, weight);
                    if ((!parORref) && isHardest) {
                        h_zgC_dynKt->Fill(zg, jetpt, weight);
                        h_rgC_dynKt->Fill(rg, jetpt, weight);
                    }
                } else {
                    h_zgL->Fill(zg, jetpt, weight);
                    h_rgL->Fill(rg, jetpt, weight);
                    if ((!parORref) && isHardest) {
                        h_zgL_dynKt->Fill(zg, refpt, weight);
                        h_rgL_dynKt->Fill(rg, refpt, weight);
                    }
                }
            }
        }
        hs_zgB[ni] = h_zgB;
        hs_rgB[ni] = h_rgB;
        
        hs_zgC[ni] = h_zgC;
        hs_rgC[ni] = h_rgC;
        
        hs_zgL[ni] = h_zgL;
        hs_rgL[ni] = h_rgL;
    
        // In the parton level case, the histograms below are empty
        
        hs_zgB_dynKt[ni] = h_zgB_dynKt;
        hs_rgB_dynKt[ni] = h_rgB_dynKt;
        
        hs_zgC_dynKt[ni] = h_zgC_dynKt;
        hs_rgC_dynKt[ni] = h_rgC_dynKt;
        
        hs_zgL_dynKt[ni] = h_zgL_dynKt;
        hs_rgL_dynKt[ni] = h_rgL_dynKt;
    }       
    std::string foutname = histfile;
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
        
        if (!parORref) {
            hs_zgB_dynKt[ni]->Write();
            hs_rgB_dynKt[ni]->Write();

            hs_zgC_dynKt[ni]->Write();
            hs_rgC_dynKt[ni]->Write();

            hs_zgL_dynKt[ni]->Write();
            hs_rgL_dynKt[ni]->Write();
        }
    }
    fout->Close();
}

void Substructure::drawBvsL(bool qcd = true, bool zgORrg = true, bool dynKt = true)
{
    //gStyle->SetOptTitle(0);
    //gStyle->SetTitleFontSize(1);
                        
    TFile *fin = new TFile(histfile.c_str());
    std::string hname1 = "h";
    std::string hname2 = "h";
    std::string title1d = "";
    std::string title2d = "";
    std::string xtitle = "";
    std::string ytitle2d = "refpt (GeV)";
    std::string ytitle1d = "1/N dN/d";

    if (qcd) {
        hname1 += "_incl";
        hname2 += "_incl";
        title2d += "qcdMC";
        title1d += "qcdMC";
    } else {
        hname1 += "_bJet";
        hname2 += "_bJet";
        title2d += "bJetMC";
        title1d += "bJetMC";
    }
    if (zgORrg) { 
        hname1 += "_zgB";
        hname2 += "_zgL";
        title2d += ", refpt vs z_{g}";
        title1d += ", z_{g}";
        xtitle += "z_{g}";
        ytitle1d += "z_{g}";
    } else {
        hname1 += "_rgB";
        hname2 += "_rgL";
        title2d += ", refpt vs R_{g}";
        title1d += ", R_{g}";
        xtitle += "R_{g}";
        ytitle1d += "R_{g}";
    }
    if (dynKt) {
        hname1 += "_dynKt";
        hname2 += "_dynKt";
        title2d += ", dynKt only";
        title1d += ", dynKt only";
    }
    
    // 2d histograms
    float maxpt2d = 130;
    
    TH2F *h2d1 = (TH2F *) fin->Get(hname1.c_str());
    TH2F *h2d2 = (TH2F *) fin->Get(hname2.c_str());
    
    TCanvas *c2d = new TCanvas("c2d", "", 1100, 500);
    c2d->Divide(2, 1);
    
    const int n = 2;
    TH2F *hs2d[n] = {h2d1, h2d2};
    
    for (int ni = 0; ni < n; ni++) {
        c2d->cd(ni + 1);
        
        TH2F *h = hs2d[ni];
        h->GetXaxis()->SetCanExtend(true);
        h->GetXaxis()->SetRange(0, h->GetXaxis()->GetNbins()); //include jets that don't have a proper substructure
        h->GetYaxis()->SetRange(1, h->GetYaxis()->FindBin(maxpt2d));
        h->SetXTitle(xtitle.c_str());
        h->SetYTitle(ytitle2d.c_str());
        h->Draw("COLZ");
    }
    
    // Add canvas title, histograms' titles
    c2d->cd();  
    TPad *pad2d = new TPad("all", "all", 0, 0, 1, 1);
    pad2d->SetFillStyle(4000);  // transparent
    pad2d->Draw();
    pad2d->cd();
    
    TLatex *ctitle = new TLatex();
    ctitle->SetTextSize(35);
    ctitle->DrawLatexNDC(0.38, 0.94, title2d.c_str());
    
    TLatex *lefttitle = new TLatex();
    lefttitle->SetTextSize(30);
    lefttitle->DrawLatexNDC(0.1, 0.92, "b-jets");
    
    TLatex *righttitle = new TLatex();
    righttitle->SetTextSize(30);
    righttitle->DrawLatexNDC(0.85, 0.92, "light jets");
    
    c2d->SetTitle(title2d.c_str());
    c2d->Draw();
    c2d->Show();   
    
    std::string savename2d = "/home/llr/cms/kalipoliti/myMacros/ref/bVSl/bVSl_" + title2d;
    savename2d = std::regex_replace(savename2d, std::regex(", "), "_");
    savename2d = std::regex_replace(savename2d, std::regex(" "), "_");
    savename2d = std::regex_replace(savename2d, std::regex("_\\{g\\}"), "g");
    savename2d += ".png";
    
    c2d->Print(savename2d.c_str());
    
    // Projections
    float minpt1d = 150;
    int firstbin = h2d1->GetYaxis()->FindBin(minpt1d);
    minpt1d = h2d1->GetYaxis()->GetBinLowEdge(firstbin);

    float maxpt1d = 250;
    int lastbin = h2d1->GetYaxis()->FindBin(maxpt1d);
    maxpt1d = h2d1->GetYaxis()->GetBinUpEdge(lastbin);
    
    TH1F *h1d1 =(TH1F *) h2d1->ProjectionX("h1d1", firstbin, lastbin, "e"); // want underflow in zg, rg not pt
    TH1F *h1d2 =(TH1F *) h2d2->ProjectionX("h1d2", firstbin, lastbin, "e");
    
    title1d += Form(", for p_{T} in [%.0f, %.0f] GeV", minpt1d, maxpt1d);
    
    TH1F *hs1d[n] = {h1d1, h1d2};
    
    TCanvas *c1d = new TCanvas("c1d", "", 600, 600);
    
    for (int ni = 0; ni < n; ni++) {
        TH1F *h = hs1d[ni];
        h->GetXaxis()->SetCanExtend(true);
        h->GetXaxis()->SetRange(0, h->GetXaxis()->GetNbins());
        h->Scale(1/h->Integral("width"));
        h->SetMarkerColor(ni + 1);
        h->SetMarkerSize(0.8);
        h->SetLineWidth(2);
        h->SetXTitle(xtitle.c_str());
        h->SetYTitle(ytitle1d.c_str());
        h->SetTitle(title1d.c_str());
        /* 
        // To have a line outlining the bins
        h->SetFillColor(0);
        h->SetLineColor(ni + 1);
        h->Draw("hist same");
        */
        h->Draw("e same");
    }
    
    TLegend *leg = new TLegend(0.75, 0.7, 0.85, 0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(h1d1, Form("#kern[-0.2]{ } b"), "pl");
    leg->AddEntry(h1d2, Form("#kern[-0.2]{ } l"), "pl");
    leg->Draw();
    
    // Add canvas title
    /*
    // Not needed for not divided canvas
    TPad *pad1d = new TPad("all", "all", 0, 0, 1, 1);
    pad1d->SetFillStyle(4000);  // transparent
    pad1d->Draw();
    pad1d->cd();
    */
    
    TLatex *c1dtitle = new TLatex();
    c1dtitle->SetTextSize(25);
    c1dtitle->DrawLatexNDC(0.15, 0.94, title1d.c_str());
    
    c1d->SetTitle(title1d.c_str());
    c1d->Draw();
    c1d->Show();
    
    std::string savename1d = "/home/llr/cms/kalipoliti/myMacros/ref/bVSl/bVSl_" + title1d;
    savename1d = std::regex_replace(savename1d, std::regex(", "), "_");
    savename1d = std::regex_replace(savename1d, std::regex(" "), "_");
    savename1d = std::regex_replace(savename1d, std::regex("_\\{g\\}"), "g");
    savename1d = std::regex_replace(savename1d, std::regex("_\\{T\\}"), "T");
    savename1d = std::regex_replace(savename1d, std::regex("\\["), "");
    savename1d = std::regex_replace(savename1d, std::regex("\\]"), "");
    savename1d += ".png";
    
    c1d->Print(savename1d.c_str());
}

void Substructure::drawdynKt(bool zgORrg = true) 
{
    
}