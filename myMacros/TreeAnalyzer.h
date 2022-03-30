// TreeAnalyzer class for b-jets
// Looking to include l, c-jets next

#include "RConfig.h" // For int, float, etc typedefs

class TreeAnalyzer {
    public:
        // Class attributes
        TFile *fin; 
        TTree *t;
    
        Float_t     weight; 
        Long64_t    nentries;

        // Fat jet properties
        Int_t       njet[30];
        Float_t     jetpt[30];
        Float_t     jeteta[30];
        Float_t     jetNb[30];
        Float_t     jetNc[30];
        
        // Subjet properties
        Float_t     subjet1pt[30];
        Float_t     subjet1eta[30];
        Float_t     subjet1phi[30];
    
        Float_t     subjet2pt[30];
        Float_t     subjet2eta[30];
        Float_t     subjet2phi[30];

        // Constructors
        TreeAnalyzer();
    
        // Class methods
        SetAnalysisLevelParton();
        SetAnalysisLevelTruth();
        SetAnalysisLevelReco();
};

// qcd = true for qcdMC, qcd = false for bJetMC
TreeAnalyzer::TreeAnalyzer() 
{
    //std::string path_qcd = "/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root";
    std::string path_bJet = "/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root";
    
    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hi=hiEvtAnalyzer/HiTree");

    nentries = t->GetEntries();
}

TreeAnalyzer::SetAnalysisLevelParton()
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

TreeAnalyzer::SetAnalysisLevelTruth()
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

TreeAnalyzer::SetAnalysisLevelReco()
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