#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TString.h>

#include <vector>

// Header file for the classes stored in the TTree if any.

const Int_t MAXJETS = 100;
const Int_t MAXNSVTX = 50;
const Int_t MAXTRACKS = 500;

class tTree {
public :
   TString         fname;
   TTree           *tree;

   // Declaration of leaf types
   Int_t           run;
   Int_t           evt;
   Int_t           lumi;
   Int_t           nvtx;
   Int_t           nref;
   Float_t         rawpt[MAXJETS];   //[nref]
   Float_t         jtpt[MAXJETS];   //[nref]
   Float_t         jteta[MAXJETS];   //[nref]
   Float_t         jty[MAXJETS];   //[nref]
   Float_t         jtphi[MAXJETS];   //[nref]
   Float_t         jtpu[MAXJETS];   //[nref]
   Float_t         jtm[MAXJETS];   //[nref]
   Float_t         jtarea[MAXJETS];   //[nref]
   Float_t         jtPfCHF[MAXJETS];   //[nref]
   Float_t         jtPfNHF[MAXJETS];   //[nref]
   Float_t         jtPfCEF[MAXJETS];   //[nref]
   Float_t         jtPfNEF[MAXJETS];   //[nref]
   Float_t         jtPfMUF[MAXJETS];   //[nref]
   Int_t           jtPfCHM[MAXJETS];   //[nref]
   Int_t           jtPfNHM[MAXJETS];   //[nref]
   Int_t           jtPfCEM[MAXJETS];   //[nref]
   Int_t           jtPfNEM[MAXJETS];   //[nref]
   Int_t           jtPfMUM[MAXJETS];   //[nref]
   Float_t         jttau1[MAXJETS];   //[nref]
   Float_t         jttau2[MAXJETS];   //[nref]
   Float_t         jttau3[MAXJETS];   //[nref]
   Int_t           jtNtrk[MAXJETS];   //[nref]
   Int_t           ntrk;
   Int_t           trkJetId[MAXTRACKS];   //[ntrk]
   Int_t           trkSvtxId[MAXTRACKS];   //[ntrk]
   Float_t         trkPt[MAXTRACKS];   //[ntrk]
   Float_t         trkEta[MAXTRACKS];   //[ntrk]
   Float_t         trkPhi[MAXTRACKS];   //[ntrk]
   Float_t         trkIp3d[MAXTRACKS];   //[ntrk]
   Float_t         trkIp3dSig[MAXTRACKS];   //[ntrk]
   Float_t         trkIp2d[MAXTRACKS];     //[ntrk]
   Float_t         trkIp2dSig[MAXTRACKS];   //[ntrk]
   Float_t         trkDistToAxisSig[MAXTRACKS];   //[ntrk]
   Float_t         trkDistToAxis[MAXTRACKS];   //[ntrk]
   Float_t         trkDz[MAXTRACKS];   //[ntrk]
   Int_t           trkMatchSta[MAXTRACKS];   //[ntrk]
   Int_t           trkPdgId[MAXTRACKS];   //[ntrk]
   Int_t           jtNsvtx[MAXJETS];   //[nref]
   Int_t           nsvtx;
   Int_t           svtxJetId[MAXNSVTX];   //[nsvtx]
   Int_t           svtxNtrk[MAXNSVTX];   //[nsvtx]
   Float_t         svtxdl[MAXNSVTX];   //[nsvtx]
   Float_t         svtxdls[MAXNSVTX];   //[nsvtx]
   Float_t         svtxdl2d[MAXNSVTX];   //[nsvtx]
   Float_t         svtxdls2d[MAXNSVTX];   //[nsvtx]
   Float_t         svtxm[MAXNSVTX];   //[nsvtx]
   Float_t         svtxmcorr[MAXNSVTX];   //[nsvtx]
   Float_t         svtxpt[MAXNSVTX];   //[nsvtx]
   Float_t         svtxnormchi2[MAXNSVTX];   //[nsvtx]
   Float_t         discr_deepCSV[MAXJETS];   //[nref]
   Float_t         discr_pfJP[MAXJETS];   //[nref]
   Float_t         discr_deepFlavour_b[MAXJETS];   //[nref]
   Float_t         discr_deepFlavour_bb[MAXJETS];   //[nref]
   Float_t         discr_deepFlavour_lepb[MAXJETS];   //[nref]
   Float_t         pthat;
   Float_t         refpt[MAXJETS];   //[nref]
   Float_t         refeta[MAXJETS];   //[nref]
   Float_t         refy[MAXJETS];   //[nref]
   Float_t         refphi[MAXJETS];   //[nref]
   Float_t         refm[MAXJETS];   //[nref]
   Float_t         refarea[MAXJETS];   //[nref]
   Float_t         refdphijt[MAXJETS];   //[nref]
   Float_t         refdrjt[MAXJETS];   //[nref]
   Float_t         refparton_pt[MAXJETS];   //[nref]
   Int_t           refparton_flavor[MAXJETS];   //[nref]
   Int_t           refparton_flavorForB[MAXJETS];   //[nref]
   Float_t         genChargedSum[MAXJETS];   //[nref]
   Float_t         genHardSum[MAXJETS];   //[nref]
   Float_t         signalChargedSum[MAXJETS];   //[nref]
   Float_t         signalHardSum[MAXJETS];   //[nref]
   Int_t           ngen;
   Int_t           genmatchindex[MAXJETS];   //[ngen]
   Float_t         genpt[MAXJETS];   //[ngen]
   Float_t         geneta[MAXJETS];   //[ngen]
   Float_t         geny[MAXJETS];   //[ngen]
   Float_t         genphi[MAXJETS];   //[ngen]
   Float_t         genm[MAXJETS];   //[ngen]
   Float_t         gendphijt[MAXJETS];   //[ngen]
   Float_t         gendrjt[MAXJETS];   //[ngen]
   Int_t           jtHadFlav[MAXJETS];   //[nref]

   Float_t         jtmB[MAXJETS]; //[nref]
   Float_t         refmB[MAXJETS]; //[nref]
   Float_t         weight; 

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_evt;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_nvtx;   //!
   TBranch        *b_nref;   //!
   TBranch        *b_rawpt;   //!
   TBranch        *b_jtpt;   //!
   TBranch        *b_jteta;   //!
   TBranch        *b_jty;   //!
   TBranch        *b_jtphi;   //!
   TBranch        *b_jtpu;   //!
   TBranch        *b_jtm;   //!
   TBranch        *b_jtarea;   //!
   TBranch        *b_jtPfCHF;   //!
   TBranch        *b_jtPfNHF;   //!
   TBranch        *b_jtPfCEF;   //!
   TBranch        *b_jtPfNEF;   //!
   TBranch        *b_jtPfMUF;   //!
   TBranch        *b_jtPfCHM;   //!
   TBranch        *b_jtPfNHM;   //!
   TBranch        *b_jtPfCEM;   //!
   TBranch        *b_jtPfNEM;   //!
   TBranch        *b_jtPfMUM;   //!
   TBranch        *b_jttau1;   //!
   TBranch        *b_jttau2;   //!
   TBranch        *b_jttau3;   //!
   TBranch        *b_jtNtrk;   //!
   TBranch        *b_ntrk;   //!
   TBranch        *b_trkJetId;   //!
   TBranch        *b_trkSvtxId;   //!
   TBranch        *b_trkPt;   //!
   TBranch        *b_trkEta;   //!
   TBranch        *b_trkPhi;   //!
   TBranch        *b_trkIp3d;   //!
   TBranch        *b_trkIp3dSig;   //!
   TBranch        *b_trkIp2d;   //!
   TBranch        *b_trkIp2dSig;   //!
   TBranch        *b_trkDistToAxisSig;   //!
   TBranch        *b_trkDistToAxis;   //!
   TBranch        *b_trkDz;   //!
   TBranch        *b_trkMatchSta;   //!
   TBranch        *b_trkPdgId;   //!
   TBranch        *b_jtNsvtx;   //!
   TBranch        *b_nsvtx;   //!
   TBranch        *b_svtxJetId;   //!
   TBranch        *b_svtxNtrk;   //!
   TBranch        *b_svtxdl;   //!
   TBranch        *b_svtxdls;   //!
   TBranch        *b_svtxdl2d;   //!
   TBranch        *b_svtxdls2d;   //!
   TBranch        *b_svtxm;   //!
   TBranch        *b_svtxmcorr;   //!
   TBranch        *b_svtxpt;   //!
   TBranch        *b_svtxnormchi2;   //!
   TBranch        *b_discr_deepCSV;   //!
   TBranch        *b_discr_pfJP;   //!
   TBranch        *b_discr_deepFlavour_b;   //!
   TBranch        *b_discr_deepFlavour_bb;   //!
   TBranch        *b_discr_deepFlavour_lepb;   //!
   TBranch        *b_pthat;   //!
   TBranch        *b_refpt;   //!
   TBranch        *b_refeta;   //!
   TBranch        *b_refy;   //!
   TBranch        *b_refphi;   //!
   TBranch        *b_refm;   //!
   TBranch        *b_refarea;   //!
   TBranch        *b_refdphijt;   //!
   TBranch        *b_refdrjt;   //!
   TBranch        *b_refparton_pt;   //!
   TBranch        *b_refparton_flavor;   //!
   TBranch        *b_refparton_flavorForB;   //!
   TBranch        *b_genChargedSum;   //!
   TBranch        *b_genHardSum;   //!
   TBranch        *b_signalChargedSum;   //!
   TBranch        *b_signalHardSum;   //!
   TBranch        *b_ngen;   //!
   TBranch        *b_genmatchindex;   //!
   TBranch        *b_genpt;   //!
   TBranch        *b_geneta;   //!
   TBranch        *b_geny;   //!
   TBranch        *b_genphi;   //!
   TBranch        *b_genm;   //!
   TBranch        *b_gendphijt;   //!
   TBranch        *b_gendrjt;   //!
   TBranch        *b_jtHadFlav;   //!

   TBranch        *b_jtmB;   //!
   TBranch        *b_refmB;   //!
   TBranch        *b_weight;   //!

   tTree(TString rootf);
   ~tTree();
   Int_t GetEntry(Long64_t entry);
   Long64_t GetEntries();
   void Init(TTree *tree);
   void SetBranchStatus(TString branchName, Int_t status);
   void SetBranchStatus(std::vector<TString> branchNames, Int_t status);
};

tTree::tTree(TString rootf)
{
   TFile *fin = new TFile(rootf);
   tree = (TTree*) fin->Get("ak4PFJetAnalyzer/t");
   tree->AddFriend("hiEvtAnalyzer/HiTree");

   Init(tree);
}

tTree::~tTree()
{
   if (!tree) return;
   delete tree;
}

Int_t tTree::GetEntry(Long64_t entry)
{
   if (!tree) return 0;
   return tree->GetEntry(entry);
}

Long64_t tTree::GetEntries()
{
   if (!tree) return 0;
   return tree->GetEntries();
}

void tTree::Init(TTree *tree)
{
   // Set branch addresses and branch pointers
   if (!tree) return;

   tree->SetBranchAddress("run", &run, &b_run);
   tree->SetBranchAddress("evt", &evt, &b_evt);
   tree->SetBranchAddress("lumi", &lumi, &b_lumi);
   tree->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   tree->SetBranchAddress("nref", &nref, &b_nref);
   tree->SetBranchAddress("rawpt", rawpt, &b_rawpt);
   tree->SetBranchAddress("jtpt", jtpt, &b_jtpt);
   tree->SetBranchAddress("jteta", jteta, &b_jteta);
   tree->SetBranchAddress("jty", jty, &b_jty);
   tree->SetBranchAddress("jtphi", jtphi, &b_jtphi);
   tree->SetBranchAddress("jtpu", jtpu, &b_jtpu);
   tree->SetBranchAddress("jtm", jtm, &b_jtm);
   tree->SetBranchAddress("jtarea", jtarea, &b_jtarea);
   tree->SetBranchAddress("jtPfCHF", jtPfCHF, &b_jtPfCHF);
   tree->SetBranchAddress("jtPfNHF", jtPfNHF, &b_jtPfNHF);
   tree->SetBranchAddress("jtPfCEF", jtPfCEF, &b_jtPfCEF);
   tree->SetBranchAddress("jtPfNEF", jtPfNEF, &b_jtPfNEF);
   tree->SetBranchAddress("jtPfMUF", jtPfMUF, &b_jtPfMUF);
   tree->SetBranchAddress("jtPfCHM", jtPfCHM, &b_jtPfCHM);
   tree->SetBranchAddress("jtPfNHM", jtPfNHM, &b_jtPfNHM);
   tree->SetBranchAddress("jtPfCEM", jtPfCEM, &b_jtPfCEM);
   tree->SetBranchAddress("jtPfNEM", jtPfNEM, &b_jtPfNEM);
   tree->SetBranchAddress("jtPfMUM", jtPfMUM, &b_jtPfMUM);
   tree->SetBranchAddress("jttau1", jttau1, &b_jttau1);
   tree->SetBranchAddress("jttau2", jttau2, &b_jttau2);
   tree->SetBranchAddress("jttau3", jttau3, &b_jttau3);
   tree->SetBranchAddress("jtNtrk", jtNtrk, &b_jtNtrk);
   tree->SetBranchAddress("ntrk", &ntrk, &b_ntrk);
   tree->SetBranchAddress("trkJetId", trkJetId, &b_trkJetId);
   tree->SetBranchAddress("trkSvtxId", trkSvtxId, &b_trkSvtxId);
   tree->SetBranchAddress("trkPt", trkPt, &b_trkPt);
   tree->SetBranchAddress("trkEta", trkEta, &b_trkEta);
   tree->SetBranchAddress("trkPhi", trkPhi, &b_trkPhi);
   tree->SetBranchAddress("trkIp3d", trkIp3d, &b_trkIp3d);
   tree->SetBranchAddress("trkIp3dSig", trkIp3dSig, &b_trkIp3dSig);
   tree->SetBranchAddress("trkIp2d", trkIp2d, &b_trkIp2d);
   tree->SetBranchAddress("trkIp2dSig", trkIp2dSig, &b_trkIp2dSig);
   tree->SetBranchAddress("trkDistToAxisSig", trkDistToAxisSig, &b_trkDistToAxisSig);
   tree->SetBranchAddress("trkDistToAxis", trkDistToAxis, &b_trkDistToAxis);
   tree->SetBranchAddress("trkDz", trkDz, &b_trkDz);
   tree->SetBranchAddress("trkMatchSta", trkMatchSta, &b_trkMatchSta);
   tree->SetBranchAddress("trkPdgId", trkPdgId, &b_trkPdgId);
   tree->SetBranchAddress("jtNsvtx", jtNsvtx, &b_jtNsvtx);
   tree->SetBranchAddress("nsvtx", &nsvtx, &b_nsvtx);
   tree->SetBranchAddress("svtxJetId", svtxJetId, &b_svtxJetId);
   tree->SetBranchAddress("svtxNtrk", svtxNtrk, &b_svtxNtrk);
   tree->SetBranchAddress("svtxdl", svtxdl, &b_svtxdl);
   tree->SetBranchAddress("svtxdls", svtxdls, &b_svtxdls);
   tree->SetBranchAddress("svtxdl2d", svtxdl2d, &b_svtxdl2d);
   tree->SetBranchAddress("svtxdls2d", svtxdls2d, &b_svtxdls2d);
   tree->SetBranchAddress("svtxm", svtxm, &b_svtxm);
   tree->SetBranchAddress("svtxmcorr", svtxmcorr, &b_svtxmcorr);
   tree->SetBranchAddress("svtxpt", svtxpt, &b_svtxpt);
   tree->SetBranchAddress("svtxnormchi2", svtxnormchi2, &b_svtxnormchi2);
   tree->SetBranchAddress("discr_deepCSV", discr_deepCSV, &b_discr_deepCSV);
   tree->SetBranchAddress("discr_pfJP", discr_pfJP, &b_discr_pfJP);
   tree->SetBranchAddress("discr_deepFlavour_b", discr_deepFlavour_b, &b_discr_deepFlavour_b);
   tree->SetBranchAddress("discr_deepFlavour_bb", discr_deepFlavour_bb, &b_discr_deepFlavour_bb);
   tree->SetBranchAddress("discr_deepFlavour_lepb", discr_deepFlavour_lepb, &b_discr_deepFlavour_lepb);
   tree->SetBranchAddress("pthat", &pthat, &b_pthat);
   tree->SetBranchAddress("refpt", refpt, &b_refpt);
   tree->SetBranchAddress("refeta", refeta, &b_refeta);
   tree->SetBranchAddress("refy", refy, &b_refy);
   tree->SetBranchAddress("refphi", refphi, &b_refphi);
   tree->SetBranchAddress("refm", refm, &b_refm);
   tree->SetBranchAddress("refarea", refarea, &b_refarea);
   tree->SetBranchAddress("refdphijt", refdphijt, &b_refdphijt);
   tree->SetBranchAddress("refdrjt", refdrjt, &b_refdrjt);
   tree->SetBranchAddress("refparton_pt", refparton_pt, &b_refparton_pt);
   tree->SetBranchAddress("refparton_flavor", refparton_flavor, &b_refparton_flavor);
   tree->SetBranchAddress("refparton_flavorForB", refparton_flavorForB, &b_refparton_flavorForB);
   tree->SetBranchAddress("genChargedSum", genChargedSum, &b_genChargedSum);
   tree->SetBranchAddress("genHardSum", genHardSum, &b_genHardSum);
   tree->SetBranchAddress("signalChargedSum", signalChargedSum, &b_signalChargedSum);
   tree->SetBranchAddress("signalHardSum", signalHardSum, &b_signalHardSum);
   tree->SetBranchAddress("ngen", &ngen, &b_ngen);
   tree->SetBranchAddress("genmatchindex", genmatchindex, &b_genmatchindex);
   tree->SetBranchAddress("genpt", genpt, &b_genpt);
   tree->SetBranchAddress("geneta", geneta, &b_geneta);
   tree->SetBranchAddress("geny", geny, &b_geny);
   tree->SetBranchAddress("genphi", genphi, &b_genphi);
   tree->SetBranchAddress("genm", genm, &b_genm);
   tree->SetBranchAddress("gendphijt", gendphijt, &b_gendphijt);
   tree->SetBranchAddress("gendrjt", gendrjt, &b_gendrjt);
   tree->SetBranchAddress("jtHadFlav", jtHadFlav, &b_jtHadFlav);

   tree->SetBranchAddress("jtmB", jtmB, &b_jtmB);
   tree->SetBranchAddress("refmB", refmB, &b_refmB);

   tree->SetBranchAddress("weight", &weight, &b_weight);
}

void tTree::SetBranchStatus(TString branchName, Int_t status)
{
    tree->SetBranchStatus(branchName, status);
}

void tTree::SetBranchStatus(std::vector<TString> branchNames, Int_t status)
{
    for (TString branchName : branchNames) {
        tree->SetBranchStatus(branchName, status);
    }
}