#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TString.h>
#include <TH3D.h>
#include <Math/Vector4D.h>
#include <Math/Vector4Dfwd.h>
#include <Math/VectorUtil.h>

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
   Int_t           nvtx;
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

   // Subjets
   Float_t         sjt1Pt[500];
   Float_t         sjt1Eta[500];
   Float_t         sjt1Phi[500];

   Float_t         sjt2Pt[500];
   Float_t         sjt2Eta[500];
   Float_t         sjt2Phi[500];

   Float_t         rsjt1Pt[500];
   Float_t         rsjt1Eta[500];
   Float_t         rsjt1Phi[500];

   Float_t         rsjt2Pt[500];
   Float_t         rsjt2Eta[500];
   Float_t         rsjt2Phi[500];

   Float_t         jtmB[500]; //[nref]
   Float_t         refmB[500]; //[nref]


   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_evt;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_nref;   //!
   TBranch        *b_nvtx;   //!
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

   TBranch        *b_sjt1Pt;
   TBranch        *b_sjt1Eta;
   TBranch        *b_sjt1Phi;

   TBranch        *b_sjt2Pt;
   TBranch        *b_sjt2Eta;
   TBranch        *b_sjt2Phi;

   TBranch        *b_rsjt1Pt;
   TBranch        *b_rsjt1Eta;
   TBranch        *b_rsjt1Phi;

   TBranch        *b_rsjt2Pt;
   TBranch        *b_rsjt2Eta;
   TBranch        *b_rsjt2Phi;


   TBranch        *b_jtmB;   //!
   TBranch        *b_refmB;   //!

   tTree(TString rootf);
   ~tTree();
   Int_t GetEntry(Long64_t entry);
   Long64_t GetEntries();
   void Init(TTree *tree);
   void SetBranchStatus(TString branchName, Int_t status);
   void SetBranchStatus(std::vector<TString> branchNames, Int_t status);
   void plot_rgzgkt(TString foutname, Float_t bTagWP = 0.9);
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
   tree->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
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

   tree->SetBranchAddress("sjt1Pt", sjt1Pt, &b_sjt1Pt);
   tree->SetBranchAddress("sjt1Eta", sjt1Eta, &b_sjt1Eta);
   tree->SetBranchAddress("sjt1Phi", sjt1Phi, &b_sjt1Phi);

   tree->SetBranchAddress("sjt2Pt", sjt2Pt, &b_sjt2Pt);
   tree->SetBranchAddress("sjt2Eta", sjt2Eta, &b_sjt2Eta);
   tree->SetBranchAddress("sjt2Phi", sjt2Phi, &b_sjt2Phi);

   tree->SetBranchAddress("rsjt1Pt", rsjt1Pt, &b_rsjt1Pt);
   tree->SetBranchAddress("rsjt1Eta", rsjt1Eta, &b_rsjt1Eta);
   tree->SetBranchAddress("rsjt1Phi", rsjt1Phi, &b_rsjt1Phi);

   tree->SetBranchAddress("rsjt2Pt", rsjt2Pt, &b_rsjt2Pt);
   tree->SetBranchAddress("rsjt2Eta", rsjt2Eta, &b_rsjt2Eta);
   tree->SetBranchAddress("rsjt2Phi", rsjt2Phi, &b_rsjt2Phi);

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

void tTree::plot_rgzgkt(TString foutname, Float_t bTagWP = 0.9)
{
    // Activate relevant branches
    tree->SetBranchStatus("*", 0);
    for (auto activeBranchName : {"nref", "refpt", "jtHadFlav",
                                  "rsjt1Pt", "rsjt1Eta", "rsjt1Phi", 
                                  "rsjt2Pt", "rsjt2Eta", "rsjt2Phi",
                                  "jtpt", "jteta",
                                  "sjt1Pt", "sjt1Eta", "sjt1Phi", 
                                  "sjt2Pt", "sjt2Eta", "sjt2Phi",
                                  "discr_deepFlavour_b", "discr_deepFlavour_bb", "discr_deepFlavour_lepb",
                                  }) {
        tree->SetBranchStatus(activeBranchName, 1);
    }

    // Create histograms

    // rg
    Int_t x1bins = 40;
    Float_t x1min = 0.;
    Float_t x1max = 0.4;

    // ln(kt)
    Int_t y1bins = 40;
    Float_t y1min = -5.;
    Float_t y1max = 5.;

    // zg
    Int_t y2bins = 40;
    Float_t y2min = 0.1;
    Float_t y2max = 0.5;

    // jtpt
    Int_t z1bins = 27*2;
    Float_t z1min = 30.;
    Float_t z1max = 300.;

    // reco level
    TH3D *hB_rgkt = new TH3D("hB_rgkt", "rg, kt, pt, bjets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hB_rgzg = new TH3D("hB_rgzg", "rg, zg, pt, bjets", x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hB_zgkt = new TH3D("hB_zgkt", "zg, kt, pt, bjets", y2bins, y2min, y2max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hBtag_rgkt = new TH3D("hBtag_rgkt", "rg, kt, pt, b tagged jets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hBtag_rgzg = new TH3D("hBtag_rgzg", "rg, zg, pt, b tagged jets", x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBtag_zgkt = new TH3D("hBtag_zgkt", "zg, kt, pt, b tagged jets", y2bins, y2min, y2max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    // gen level
    TH3D *hB_rgkt_gen = new TH3D("hB_rgkt_gen", "rg, kt, pt, bjets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hB_rgzg_gen = new TH3D("hB_rgzg_gen", "rg, zg, pt, bjets", x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hB_zgkt_gen = new TH3D("hB_zgkt_gen", "zg, kt, pt, bjets", y2bins, y2min, y2max, y1bins, y1min, y1max, z1bins, z1min, z1max);

    TH3D *hBtag_rgkt_gen = new TH3D("hBtag_rgkt_gen", "rg, kt, pt, b tagged jets", x1bins, x1min, x1max, y1bins, y1min, y1max, z1bins, z1min, z1max);
    TH3D *hBtag_rgzg_gen = new TH3D("hBtag_rgzg_gen", "rg, zg, pt, b tagged jets", x1bins, x1min, x1max, y2bins, y2min, y2max, z1bins, z1min, z1max);
    TH3D *hBtag_zgkt_gen = new TH3D("hBtag_zgkt_gen", "zg, kt, pt, b tagged jets", y2bins, y2min, y2max, y1bins, y1min, y1max, z1bins, z1min, z1max);


    std::cout << "Creating histograms ..." << std::endl;
   Long64_t nentries = tree->GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 1000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

		// For debugging purposes
		//if (ient > 15) break;
 
        tree->GetEntry(ient);

        for (Int_t ijet = 0; ijet < nref; ijet++) {
            // universal eta cut
            if (std::abs(jteta[ijet]) > 2) continue;

            Float_t rg = -1.;
            Float_t kt = -1.;
            Float_t zg = -1.;

            Float_t logrg = -1.;
            Float_t logkt = -10.;

            Float_t rg_gen = -1.;
            Float_t kt_gen = -1.;
            Float_t zg_gen = -1.;

            Float_t logrg_gen = -1.;
            Float_t logkt_gen = -10.;

            // Calculate rg, kt only for 2 prong jets
            if (sjt2Pt[ijet] > 0.) {
                ROOT::Math::PtEtaPhiMVector v1;
                v1.SetPt(sjt1Pt[ijet]);
                v1.SetEta(sjt1Eta[ijet]);
                v1.SetPhi(sjt1Phi[ijet]);

                ROOT::Math::PtEtaPhiMVector v2;
                v2.SetPt(sjt2Pt[ijet]);
                v2.SetEta(sjt2Eta[ijet]);
                v2.SetPhi(sjt2Phi[ijet]);

                rg = ROOT::Math::VectorUtil::DeltaR(v1, v2);
                kt = sjt2Pt[ijet] * rg;
                zg = sjt2Pt[ijet] / (sjt2Pt[ijet] + sjt2Pt[ijet]);
                
                // calculate logs
                logrg = log(1/rg);
                logkt = log(kt);
            }

            if (rsjt2Pt[ijet] > 0.) {
                ROOT::Math::PtEtaPhiMVector v1_gen;
                v1_gen.SetPt(rsjt1Pt[ijet]);
                v1_gen.SetEta(rsjt1Eta[ijet]);
                v1_gen.SetPhi(rsjt1Phi[ijet]);

                ROOT::Math::PtEtaPhiMVector v2_gen;
                v2_gen.SetPt(rsjt2Pt[ijet]);
                v2_gen.SetEta(rsjt2Eta[ijet]);
                v2_gen.SetPhi(rsjt2Phi[ijet]);

                rg_gen = ROOT::Math::VectorUtil::DeltaR(v1_gen, v2_gen);
                kt_gen = rsjt2Pt[ijet] * rg_gen;
                zg_gen = rsjt2Pt[ijet] / (rsjt2Pt[ijet] + rsjt2Pt[ijet]);
                
                // calculate logs
                logrg_gen = log(1/rg_gen);
                logkt_gen = log(kt_gen);
            }

            // Fill true-flavour histograms
            bool isBjet = (jtHadFlav[ijet] == 5);
            if (isBjet) {
               hB_rgkt->Fill(rg, logkt, jtpt[ijet]);
               hB_rgzg->Fill(rg, zg, jtpt[ijet]);
               hB_zgkt->Fill(zg, logkt, jtpt[ijet]);
            
               hB_rgkt_gen->Fill(rg_gen, logkt_gen, refpt[ijet]);
               hB_rgzg_gen->Fill(rg_gen, zg_gen, refpt[ijet]);
               hB_zgkt_gen->Fill(zg_gen, logkt_gen, refpt[ijet]);
            

               // Fill the b-tag histogram
               bool passWP = ((discr_deepFlavour_b[ijet] + discr_deepFlavour_bb[ijet] + discr_deepFlavour_lepb[ijet]) > bTagWP);
               if (passWP) {
                  hBtag_rgkt->Fill(rg, logkt, jtpt[ijet]);
                  hBtag_rgzg->Fill(rg, zg, jtpt[ijet]);
                  hBtag_zgkt->Fill(zg, logkt, jtpt[ijet]);

                  hBtag_rgkt_gen->Fill(rg_gen, logkt_gen, refpt[ijet]);
                  hBtag_rgzg_gen->Fill(rg_gen, zg_gen, refpt[ijet]);
                  hBtag_zgkt_gen->Fill(zg_gen, logkt_gen, refpt[ijet]);
               }
            }
        } // jet loop
    } // entry loop

    // Save histograms
    std::cout << "\n(Re)creating file " << foutname << std::endl;
    TFile *fout = new TFile(foutname, "recreate");

    for (auto h : {hB_rgkt, hB_rgzg, hB_zgkt, 
                   hBtag_rgkt, hBtag_rgzg, hBtag_zgkt,
                   hB_rgkt_gen, hB_rgzg_gen, hB_zgkt_gen, 
                   hBtag_rgkt_gen, hBtag_rgzg_gen, hBtag_zgkt_gen}) {
        h->Write();
    }

    fout->Close();
}