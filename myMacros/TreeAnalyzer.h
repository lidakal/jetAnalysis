// TreeAnalyzer class for b-jets
// Looking to include l, c-jets next

#include "TFile.h"
#include "TTree.h"

class TreeAnalyzer 
{
    public:
        // Class attributes
        TFile *fin; 
        TTree *t;
    
        Float_t     weight; 
        Long64_t    nentries;

        // t tree leaves
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

        // Fat jet properties
        Int_t           njet[30];
        Float_t         jetpt[30];
        Float_t         jeteta[30];
        Float_t         jetNb[30];
        Float_t         jetNc[30];
        
        // Subjet properties
        Float_t         subjet1pt[30];
        Float_t         subjet1eta[30];
        Float_t         subjet1phi[30];
    
        Float_t         subjet2pt[30];
        Float_t         subjet2eta[30];
        Float_t         subjet2phi[30];

        // Constructors
        TreeAnalyzer(bool init = true);
    
        // Class methods
        void SetAnalysisLevelParton();
        void SetAnalysisLevelTruth();
        void SetAnalysisLevelReco();
};

// qcd = true for qcdMC, qcd = false for bJetMC
TreeAnalyzer::TreeAnalyzer(bool init = true) 
{
    //std::string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    std::string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";
    
    fin = new TFile(path_bJet.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

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

    t->SetBranchAddress("weight", &weight);
    
    nentries = t->GetEntries();
}

void TreeAnalyzer::SetAnalysisLevelParton()
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
}

void TreeAnalyzer::SetAnalysisLevelTruth()
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
}

void TreeAnalyzer::SetAnalysisLevelReco()
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
}