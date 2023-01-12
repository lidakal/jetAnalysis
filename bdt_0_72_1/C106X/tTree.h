#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TString.h>

// Header file for the classes stored in the TTree if any.

class tTree {
public :
   TString         fname;
   TTree           *tree;

   // Declaration of leaf types
   Int_t           run;
   Int_t           evt;
   Int_t           lumi;
   Int_t           nref;
   Float_t         rawpt[500];   //[nref]
   Float_t         jtpt[500];   //[nref]
   Float_t         jteta[500];   //[nref]
   Float_t         jty[500];   //[nref]
   Float_t         jtphi[500];   //[nref]
   Float_t         jtpu[500];   //[nref]
   Float_t         jtm[500];   //[nref]
   Float_t         jtarea[500];   //[nref]
   Float_t         jtPfCHF[500];   //[nref]
   Float_t         jtPfNHF[500];   //[nref]
   Float_t         jtPfCEF[500];   //[nref]
   Float_t         jtPfNEF[500];   //[nref]
   Float_t         jtPfMUF[500];   //[nref]
   Int_t           jtPfCHM[500];   //[nref]
   Int_t           jtPfNHM[500];   //[nref]
   Int_t           jtPfCEM[500];   //[nref]
   Int_t           jtPfNEM[500];   //[nref]
   Int_t           jtPfMUM[500];   //[nref]
   Float_t         jttau1[500];   //[nref]
   Float_t         jttau2[500];   //[nref]
   Float_t         jttau3[500];   //[nref]
   Int_t           jtNtrk[500];   //[nref]
   Int_t           ntrk;
   Int_t           trkJetId[500];   //[ntrk]
   Int_t           trkSvtxId[500];   //[ntrk]
   Float_t         trkPt[500];   //[ntrk]
   Float_t         trkEta[500];   //[ntrk]
   Float_t         trkPhi[500];   //[ntrk]
   Float_t         trkIp3d[500];   //[ntrk]
   Float_t         trkIp3dSig[500];   //[ntrk]
   Float_t         trkDistToAxisSig[500];   //[ntrk]
   Float_t         trkDistToAxis[500];   //[ntrk]
   Int_t           trkMatchSta[500];   //[ntrk]
   Int_t           jtNsvtx[500];   //[nref]
   Int_t           nsvtx;
   Int_t           svtxJetId[50];   //[nsvtx]
   Int_t           svtxNtrk[50];   //[nsvtx]
   Float_t         svtxdl[50];   //[nsvtx]
   Float_t         svtxdls[50];   //[nsvtx]
   Float_t         svtxdl2d[50];   //[nsvtx]
   Float_t         svtxdls2d[50];   //[nsvtx]
   Float_t         svtxm[50];   //[nsvtx]
   Float_t         svtxpt[50];   //[nsvtx]
   Float_t         discr_deepCSV[500];   //[nref]
   Float_t         discr_pfJP[500];   //[nref]
   Float_t         discr_deepFlavour_b[500];   //[nref]
   Float_t         discr_deepFlavour_bb[500];   //[nref]
   Float_t         discr_deepFlavour_lepb[500];   //[nref]
   Float_t         pthat;
   Float_t         refpt[500];   //[nref]
   Float_t         refeta[500];   //[nref]
   Float_t         refy[500];   //[nref]
   Float_t         refphi[500];   //[nref]
   Float_t         refm[500];   //[nref]
   Float_t         refarea[500];   //[nref]
   Float_t         refdphijt[500];   //[nref]
   Float_t         refdrjt[500];   //[nref]
   Float_t         refparton_pt[500];   //[nref]
   Int_t           refparton_flavor[500];   //[nref]
   Int_t           refparton_flavorForB[500];   //[nref]
   Float_t         genChargedSum[500];   //[nref]
   Float_t         genHardSum[500];   //[nref]
   Float_t         signalChargedSum[500];   //[nref]
   Float_t         signalHardSum[500];   //[nref]
   Int_t           ngen;
   Int_t           genmatchindex[100];   //[ngen]
   Float_t         genpt[100];   //[ngen]
   Float_t         geneta[100];   //[ngen]
   Float_t         geny[100];   //[ngen]
   Float_t         genphi[100];   //[ngen]
   Float_t         genm[100];   //[ngen]
   Float_t         gendphijt[100];   //[ngen]
   Float_t         gendrjt[100];   //[ngen]
   Int_t           jtHadFlav[500];   //[nref]

   Float_t         jtmB[500]; //[nref]
   Float_t         refmB[500]; //[nref]

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_evt;   //!
   TBranch        *b_lumi;   //!
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
   TBranch        *b_trkDistToAxisSig;   //!
   TBranch        *b_trkDistToAxis;   //!
   TBranch        *b_trkMatchSta;   //!
   TBranch        *b_jtNsvtx;   //!
   TBranch        *b_nsvtx;   //!
   TBranch        *b_svtxJetId;   //!
   TBranch        *b_svtxNtrk;   //!
   TBranch        *b_svtxdl;   //!
   TBranch        *b_svtxdls;   //!
   TBranch        *b_svtxdl2d;   //!
   TBranch        *b_svtxdls2d;   //!
   TBranch        *b_svtxm;   //!
   TBranch        *b_svtxpt;   //!
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
   tree->SetBranchAddress("trkDistToAxisSig", trkDistToAxisSig, &b_trkDistToAxisSig);
   tree->SetBranchAddress("trkDistToAxis", trkDistToAxis, &b_trkDistToAxis);
   tree->SetBranchAddress("trkMatchSta", trkMatchSta, &b_trkMatchSta);
   tree->SetBranchAddress("jtNsvtx", jtNsvtx, &b_jtNsvtx);
   tree->SetBranchAddress("nsvtx", &nsvtx, &b_nsvtx);
   tree->SetBranchAddress("svtxJetId", svtxJetId, &b_svtxJetId);
   tree->SetBranchAddress("svtxNtrk", svtxNtrk, &b_svtxNtrk);
   tree->SetBranchAddress("svtxdl", svtxdl, &b_svtxdl);
   tree->SetBranchAddress("svtxdls", svtxdls, &b_svtxdls);
   tree->SetBranchAddress("svtxdl2d", svtxdl2d, &b_svtxdl2d);
   tree->SetBranchAddress("svtxdls2d", svtxdls2d, &b_svtxdls2d);
   tree->SetBranchAddress("svtxm", svtxm, &b_svtxm);
   tree->SetBranchAddress("svtxpt", svtxpt, &b_svtxpt);
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
}

void tTree::SetBranchStatus(TString branchName, Int_t status)
{
    tree->SetBranchStatus(branchName, status);
}

void tTree::SetBranchStatus(vector<TString> branchNames, Int_t status)
{
    for (TString branchName : branchNames) {
        tree->SetBranchStatus(branchName, status);
    }
}