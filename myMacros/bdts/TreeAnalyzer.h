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

		vector<vector<int> > *svtxntrk = 0;
        vector<vector<float> > *svtxdls = nullptr;
        vector<vector<float> > *svtxdls2d = nullptr;
        vector<vector<float> > *svtxm = nullptr;
        vector<vector<float> > *svtxpt = nullptr;
        vector<vector<float> > *svtxmcorr = nullptr;
        vector<vector<float> > *svtxTrPt = nullptr;
        vector<vector<float> > *svtxTrEta = nullptr;
        vector<vector<float> > *svtxTrPhi = nullptr;

        Int_t           nsvtx[8];
	
        Int_t           nselIPtrk[8];
        Int_t           nIP;
        Float_t         ipPt[64];
        Float_t         ipEta[64];
        Float_t         ipPhi[64];
        Float_t         ipProb[64];
        Float_t         ip3dSig[64];

	    Int_t           ipMatchStatus[100];
	    Float_t         ipPtMatch[100];
        Float_t         ipEtaMatch[100];
        Float_t         ipPhiMatch[100];
        Int_t           inSV[100];

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
        Float_t         weight; 

        // Constructors
        TreeAnalyzer();
        TreeAnalyzer(string fname = "/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root", bool init = true);
    
        // Class methods
        void Init();
        void GetEntry(Long64_t ient);
        void SetBranchStatus(string branchName, Int_t status);
    	void SetBranchStatus(vector<string> branchNames, Int_t status);
};

TreeAnalyzer::TreeAnalyzer(string fname = "/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root", bool init = true) 
{
    fin = new TFile(fname.c_str());

    t = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
    t->AddFriend("hiEvtAnalyzer/HiTree");

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

	t->SetBranchAddress("ipMatchStatus", &ipMatchStatus);
	t->SetBranchAddress("ipPtMatch", ipPtMatch);
	t->SetBranchAddress("ipEtaMatch", ipEtaMatch);
	t->SetBranchAddress("ipPhiMatch", ipPhiMatch);

    t->SetBranchAddress("inSV", inSV);

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

    t->SetBranchAddress("weight", &weight);
    
    nentries = t->GetEntries();
}

void TreeAnalyzer::GetEntry(Long64_t ient)
{
    t->GetEntry(ient);
}

void TreeAnalyzer::SetBranchStatus(string branchName, Int_t status)
{
    t->SetBranchStatus(branchName.c_str(), status);
}

void TreeAnalyzer::SetBranchStatus(vector<string> branchNames, Int_t status)
{
    for (string branchName : branchNames) {
        t->SetBranchStatus(branchName.c_str(), status);
    }
}
