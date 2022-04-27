// TreeAnalyzer class for ...root

#include <vector>

#include "TFile.h"
#include "TTree.h"

using namespace std;

class TreeAnalyzer 
{
    public:
        // Class attributes
        TFile *fin; 
        TTree *t;
        TTree *HiTree;
        TTree *hi;
     
        Long64_t    nentries;

        // t tree leaves
        Int_t           nref;
        Float_t         rawpt[8];
        Float_t         jtpt[8];
        Float_t         jteta[8];
        Float_t         jtphi[8];
        Float_t         jtHadFlav[8];
        Float_t         jtParFlav[8];
        Int_t           jtNbHad[8];
        Int_t           jtNcHad[8];
        Int_t           jtNbPar[8];
        Int_t           jtNcPar[8];
        Bool_t          jtHasGSPB[8];
        Bool_t          jtHasGSPC[8];
        Bool_t          jtIsHardest[8];
        Float_t         sjt1E[8];
        Float_t         sjt1Pt[8];
        Float_t         sjt1Eta[8];
        Float_t         sjt1Phi[8];
        Float_t         sjt1HadFlav[8];
        Float_t         sjt1ParFlav[8];
        Float_t         sjt2E[8];
        Float_t         sjt2Pt[8];
        Float_t         sjt2Eta[8];
        Float_t         sjt2Phi[8];
        Float_t         sjt2HadFlav[8];
        Float_t         sjt2ParFlav[8];
        Float_t         rsjt1E[8];
        Float_t         rsjt1Pt[8];
        Float_t         rsjt1Eta[8];
        Float_t         rsjt1Phi[8];
        Float_t         rsjt2E[8];
        Float_t         rsjt2Pt[8];
        Float_t         rsjt2Eta[8];
        Float_t         rsjt2Phi[8];
        Float_t         jtDiscCSVV2[8];
        Float_t         jtDiscDeepCSVB[8];
        Float_t         jtDiscDeepCSVBB[8];
        Float_t         jtDiscDeepCSVC[8];
        Float_t         jtDiscDeepFlavourB[8];
        Float_t         jtDiscDeepFlavourBB[8];
        Float_t         jtDiscDeepFlavourLEPB[8];
        Float_t         jtDiscDeepFlavourC[8];
        Float_t         jtDiscProb[8];
        Int_t           nsvtx[8];
        vector<vector<int> > svtxntrk;
        vector<vector<float> > svtxdls;
        vector<vector<float> > svtxdls2d;
        vector<vector<float> > svtxm;
        vector<vector<float> > svtxpt;
        vector<vector<float> > svtxmcorr;
        vector<vector<float> > svtxTrPt;
        vector<vector<float> > svtxTrEta;
        vector<vector<float> > svtxTrPhi;
        Int_t           nselIPtrk[8];
        Int_t           nIP;
        Float_t         ipPt[64];
        Float_t         ipEta[64];
        Float_t         ipPhi[64];
        Float_t         ipProb[64];
        Float_t         ip3dSig[64];
        Float_t         refpt[8];
        Float_t         refeta[8];
        Float_t         refphi[8];
        Float_t         refdphijt[8];
        Float_t         refdrjt[8];
        Float_t         refparton_pt[8];
        Int_t           refparton_flavor[8];
        Bool_t          refIsHardest[8];
        Int_t           ngen;
        Int_t           genmatchindex[8];
        Float_t         genpt[8];
        Float_t         geneta[8];
        Float_t         genphi[8];
        Int_t           npar;
        Float_t         parpt[9];
        Float_t         pareta[9];
        Float_t         parphi[9];
        Int_t           parNb[9];
        Int_t           parNc[9];
        Bool_t          parHasGSPB[9];
        Bool_t          parHasGSPC[9];
        Bool_t          genIsHardest[8];
        Float_t         gsjt1E[8];
        Float_t         gsjt1Pt[8];
        Float_t         gsjt1Eta[8];
        Float_t         gsjt1Phi[8];
        Float_t         gsjt2Pt[8];
        Float_t         gsjt2Eta[8];
        Float_t         gsjt2Phi[8];
        Float_t         psjt1E[9];
        Float_t         psjt1Pt[9];
        Float_t         psjt1Eta[9];
        Float_t         psjt1Phi[9];
        Float_t         psjt2Pt[9];
        Float_t         psjt2Eta[9];
        Float_t         psjt2Phi[9];

        // HiTree tree leaves
        UInt_t          run;
        ULong64_t       evt;
        UInt_t          lumi;
        Float_t         vx;
        Float_t         vy;
        Float_t         vz;
        Float_t         pthat;
        Float_t         weight; 

        // hi tree leaves
        Int_t           mult;
        vector<float>*  pt = 0;
        vector<float>   eta;
        vector<float>   phi;
        vector<int>     pdg;
        vector<int>     sta;

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

        // Constructors
        TreeAnalyzer(string fname = "/data_CMS/cms/kalipoliti/aggregatedB_truthInfo/merged_HiForestAOD.root", bool init = true);
    
        // Class methods
        void Init();
        void SetAnalysisLevelParton(bool activBranches = true);
        void SetAnalysisLevelTruth(bool activBranches = true);
        void SetAnalysisLevelReco(bool activBranches = true);
};

TreeAnalyzer::TreeAnalyzer(string fname = "/data_CMS/cms/kalipoliti/aggregatedB_truthInfo/merged_HiForestAOD.root", bool init = true) 
{
    fin = new TFile(fname.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    HiTree = (TTree *) fin->Get("hiEvtAnalyzer/HiTree");
    hi = (TTree *) fin->Get("bDecayAna/hi");

    if (init) {
        Init();
    }
}

void TreeAnalyzer::Init() 
{
    // Set branch addresses for t tree
    t->SetBranchAddress("nref",&nref);
    t->SetBranchAddress("rawpt",rawpt);
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
    t->SetBranchAddress("rsjt1E",rsjt1E);
    t->SetBranchAddress("rsjt1Pt",rsjt1Pt);
    t->SetBranchAddress("rsjt1Eta",rsjt1Eta);
    t->SetBranchAddress("rsjt1Phi",rsjt1Phi);
    t->SetBranchAddress("rsjt2E",rsjt2E);
    t->SetBranchAddress("rsjt2Pt",rsjt2Pt);
    t->SetBranchAddress("rsjt2Eta",rsjt2Eta);
    t->SetBranchAddress("rsjt2Phi",rsjt2Phi);
    t->SetBranchAddress("jtDiscCSVV2",jtDiscCSVV2);
    t->SetBranchAddress("jtDiscDeepCSVB",jtDiscDeepCSVB);
    t->SetBranchAddress("jtDiscDeepCSVBB",jtDiscDeepCSVBB);
    t->SetBranchAddress("jtDiscDeepCSVC",jtDiscDeepCSVC);
    t->SetBranchAddress("jtDiscDeepFlavourB",jtDiscDeepFlavourB);
    t->SetBranchAddress("jtDiscDeepFlavourBB",jtDiscDeepFlavourBB);
    t->SetBranchAddress("jtDiscDeepFlavourLEPB",jtDiscDeepFlavourLEPB);
    t->SetBranchAddress("jtDiscDeepFlavourC",jtDiscDeepFlavourC);
    t->SetBranchAddress("jtDiscProb",jtDiscProb);
    t->SetBranchAddress("nsvtx",nsvtx);
    
    t->SetBranchAddress("svtxntrk",&svtxntrk);
    t->SetBranchAddress("svtxdls",&svtxdls);
    t->SetBranchAddress("svtxdls2d",&svtxdls2d);
    t->SetBranchAddress("svtxm",&svtxm);
    t->SetBranchAddress("svtxpt",&svtxpt);
    t->SetBranchAddress("svtxmcorr",&svtxmcorr);
    t->SetBranchAddress("svtxTrPt",&svtxTrPt);
    t->SetBranchAddress("svtxTrEta",&svtxTrEta);
    t->SetBranchAddress("svtxTrPhi",&svtxTrPhi);
    
    t->SetBranchAddress("nselIPtrk",nselIPtrk);
    t->SetBranchAddress("nIP",&nIP);
    t->SetBranchAddress("ipPt",ipPt);
    t->SetBranchAddress("ipEta",ipEta);
    t->SetBranchAddress("ipPhi",ipPhi);
    t->SetBranchAddress("ipProb",ipProb);
    t->SetBranchAddress("ip3dSig",ip3dSig);
    t->SetBranchAddress("refpt",refpt);
    t->SetBranchAddress("refeta",refeta);
    t->SetBranchAddress("refphi",refphi);
    t->SetBranchAddress("refdphijt",refdphijt);
    t->SetBranchAddress("refdrjt",refdrjt);
    t->SetBranchAddress("refparton_pt",refparton_pt);
    t->SetBranchAddress("refparton_flavor",refparton_flavor);
    t->SetBranchAddress("refIsHardest",refIsHardest);
    t->SetBranchAddress("ngen",&ngen);
    t->SetBranchAddress("genmatchindex",genmatchindex);
    t->SetBranchAddress("genpt",genpt);
    t->SetBranchAddress("geneta",geneta);
    t->SetBranchAddress("genphi",genphi);
    t->SetBranchAddress("npar",&npar);
    t->SetBranchAddress("parpt",parpt);
    t->SetBranchAddress("pareta",pareta);
    t->SetBranchAddress("parphi",parphi);
    t->SetBranchAddress("parNb",parNb);
    t->SetBranchAddress("parNc",parNc);
    t->SetBranchAddress("parHasGSPB",parHasGSPB);
    t->SetBranchAddress("parHasGSPC",parHasGSPC);
    t->SetBranchAddress("genIsHardest",genIsHardest);
    t->SetBranchAddress("gsjt1E",gsjt1E);
    t->SetBranchAddress("gsjt1Pt",gsjt1Pt);
    t->SetBranchAddress("gsjt1Eta",gsjt1Eta);
    t->SetBranchAddress("gsjt1Phi",gsjt1Phi);
    t->SetBranchAddress("gsjt2Pt",gsjt2Pt);
    t->SetBranchAddress("gsjt2Eta",gsjt2Eta);
    t->SetBranchAddress("gsjt2Phi",gsjt2Phi);
    t->SetBranchAddress("psjt1E",psjt1E);
    t->SetBranchAddress("psjt1Pt",psjt1Pt);
    t->SetBranchAddress("psjt1Eta",psjt1Eta);
    t->SetBranchAddress("psjt1Phi",psjt1Phi);
    t->SetBranchAddress("psjt2Pt",psjt2Pt);
    t->SetBranchAddress("psjt2Eta",psjt2Eta);
    t->SetBranchAddress("psjt2Phi",psjt2Phi);

    // Set branch addresses for HiTree tree
    HiTree->SetBranchAddress("run", &run);
    HiTree->SetBranchAddress("evt", &evt);
    HiTree->SetBranchAddress("lumi", &lumi);
    HiTree->SetBranchAddress("vx", &vx);
    HiTree->SetBranchAddress("vy", &vy);
    HiTree->SetBranchAddress("vz", &vz);
    HiTree->SetBranchAddress("pthat", &pthat);
    HiTree->SetBranchAddress("weight", &weight);

    // Set branch addressses for hi tree
    hi->SetBranchAddress("mult", &mult);
    hi->SetBranchAddress("pt", &pt);
    hi->SetBranchAddress("eta", &eta);
    hi->SetBranchAddress("phi", &phi);
    hi->SetBranchAddress("pdg", &pdg);
    hi->SetBranchAddress("sta", &sta);
    
    nentries = t->GetEntries();
}

void TreeAnalyzer::SetAnalysisLevelParton(bool activBranches = true)
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

    if (activBranches) {
        t->SetBranchStatus("*", 0);
        for (auto activeBranchName : {"npar", "parpt", "pareta", "parNb", "parNc",
                                      "psjt1Pt", "psjt1Eta", "psjt1Phi", 
                                      "psjt2Pt", "psjt2Eta", "psjt2Phi",
                                      "weight"}) {
            t->SetBranchStatus(activeBranchName, 1);
        }
    }
}

void TreeAnalyzer::SetAnalysisLevelTruth(bool activBranches = true)
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

    if (activBranches) {
        t->SetBranchStatus("*", 0);
        for (auto activeBranchName : {"nref", "refpt", "refeta", "jtHadFlav", "refIsHardest",
                                      "jtNbHad", "jtNcHad",
                                      "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", 
                                      "rsjt2Pt", "rsjt2Eta", "rsjt2Phi",
                                      "weight"}) {
            t->SetBranchStatus(activeBranchName, 1);
        }
    }
}

void TreeAnalyzer::SetAnalysisLevelReco(bool activBranches = true)
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

    if (activBranches) {
        t->SetBranchStatus("*", 0);
        for (auto activeBranchName : {"nref", "jtpt", "jteta", "jtHadFlav", "jtIsHardest",
                                      "jtNbHad", "jtNcHad",
                                      "sjt1Pt", "sjt1Eta", "sjt1Phi", 
                                      "sjt2Pt", "sjt2Eta", "sjt2Phi",
                                      "weight"}) {
            t->SetBranchStatus(activeBranchName, 1);
        }
    }
}
