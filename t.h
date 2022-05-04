//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May  4 11:47:35 2022 by ROOT version 6.10/09
// from TTree t/ak4PFpatJetsWithBtagging Jet Analysis Tree
// found on file: /data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root
//////////////////////////////////////////////////////////

#ifndef t_h
#define t_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"

class t {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           nref;
   Float_t         rawpt[15];   //[nref]
   Float_t         jtpt[15];   //[nref]
   Float_t         jteta[15];   //[nref]
   Float_t         jtphi[15];   //[nref]
   Float_t         jtHadFlav[15];   //[nref]
   Float_t         jtParFlav[15];   //[nref]
   Int_t           jtNbHad[15];   //[nref]
   Int_t           jtNcHad[15];   //[nref]
   Int_t           jtNbPar[15];   //[nref]
   Int_t           jtNcPar[15];   //[nref]
   Bool_t          jtHasGSPB[15];   //[nref]
   Bool_t          jtHasGSPC[15];   //[nref]
   Bool_t          jtIsHardest[15];   //[nref]
   Float_t         sjt1E[15];   //[nref]
   Float_t         sjt1Pt[15];   //[nref]
   Float_t         sjt1Eta[15];   //[nref]
   Float_t         sjt1Phi[15];   //[nref]
   Float_t         sjt1HadFlav[15];   //[nref]
   Float_t         sjt1ParFlav[15];   //[nref]
   Float_t         sjt2E[15];   //[nref]
   Float_t         sjt2Pt[15];   //[nref]
   Float_t         sjt2Eta[15];   //[nref]
   Float_t         sjt2Phi[15];   //[nref]
   Float_t         sjt2HadFlav[15];   //[nref]
   Float_t         sjt2ParFlav[15];   //[nref]
   Float_t         rsjt1E[15];   //[nref]
   Float_t         rsjt1Pt[15];   //[nref]
   Float_t         rsjt1Eta[15];   //[nref]
   Float_t         rsjt1Phi[15];   //[nref]
   Float_t         rsjt2E[15];   //[nref]
   Float_t         rsjt2Pt[15];   //[nref]
   Float_t         rsjt2Eta[15];   //[nref]
   Float_t         rsjt2Phi[15];   //[nref]
   Float_t         jtDiscCSVV2[15];   //[nref]
   Float_t         jtDiscDeepCSVB[15];   //[nref]
   Float_t         jtDiscDeepCSVBB[15];   //[nref]
   Float_t         jtDiscDeepCSVC[15];   //[nref]
   Float_t         jtDiscDeepFlavourB[15];   //[nref]
   Float_t         jtDiscDeepFlavourBB[15];   //[nref]
   Float_t         jtDiscDeepFlavourLEPB[15];   //[nref]
   Float_t         jtDiscDeepFlavourC[15];   //[nref]
   Float_t         jtDiscProb[15];   //[nref]
   Int_t           nsvtx[15];   //[nref]
   vector<vector<int> > *svtxntrk;
   vector<vector<float> > *svtxdls;
   vector<vector<float> > *svtxdls2d;
   vector<vector<float> > *svtxm;
   vector<vector<float> > *svtxpt;
   vector<vector<float> > *svtxmcorr;
   vector<vector<float> > *svtxTrPt;
   vector<vector<float> > *svtxTrEta;
   vector<vector<float> > *svtxTrPhi;
   Int_t           nselIPtrk[15];   //[nref]
   Int_t           nIP;
   Float_t         ipPt[149];   //[nIP]
   Float_t         ipEta[149];   //[nIP]
   Float_t         ipPhi[149];   //[nIP]
   Float_t         ipProb[149];   //[nIP]
   Float_t         ip3dSig[149];   //[nIP]
   Float_t         ipPtMatch[149];   //[nIP]
   Float_t         ipEtaMatch[149];   //[nIP]
   Float_t         ipPhiMatch[149];   //[nIP]
   Int_t           ipMatchStatus[149];   //[nIP]
   Float_t         refpt[15];   //[nref]
   Float_t         refeta[15];   //[nref]
   Float_t         refphi[15];   //[nref]
   Float_t         refdphijt[15];   //[nref]
   Float_t         refdrjt[15];   //[nref]
   Float_t         refparton_pt[15];   //[nref]
   Int_t           refparton_flavor[15];   //[nref]
   Bool_t          refIsHardest[15];   //[nref]
   Int_t           ngen;
   Int_t           genmatchindex[14];   //[ngen]
   Float_t         genpt[14];   //[ngen]
   Float_t         geneta[14];   //[ngen]
   Float_t         genphi[14];   //[ngen]
   Int_t           npar;
   Float_t         parpt[16];   //[npar]
   Float_t         pareta[16];   //[npar]
   Float_t         parphi[16];   //[npar]
   Int_t           parNb[16];   //[npar]
   Int_t           parNc[16];   //[npar]
   Bool_t          parHasGSPB[16];   //[npar]
   Bool_t          parHasGSPC[16];   //[npar]
   Bool_t          genIsHardest[14];   //[ngen]
   Float_t         gsjt1E[14];   //[ngen]
   Float_t         gsjt1Pt[14];   //[ngen]
   Float_t         gsjt1Eta[14];   //[ngen]
   Float_t         gsjt1Phi[14];   //[ngen]
   Float_t         gsjt2Pt[14];   //[ngen]
   Float_t         gsjt2Eta[14];   //[ngen]
   Float_t         gsjt2Phi[14];   //[ngen]
   Float_t         psjt1E[16];   //[npar]
   Float_t         psjt1Pt[16];   //[npar]
   Float_t         psjt1Eta[16];   //[npar]
   Float_t         psjt1Phi[16];   //[npar]
   Float_t         psjt2Pt[16];   //[npar]
   Float_t         psjt2Eta[16];   //[npar]
   Float_t         psjt2Phi[16];   //[npar]

   // List of branches
   TBranch        *b_nref;   //!
   TBranch        *b_rawpt;   //!
   TBranch        *b_jtpt;   //!
   TBranch        *b_jteta;   //!
   TBranch        *b_jtphi;   //!
   TBranch        *b_jtHadFlav;   //!
   TBranch        *b_jtParFlav;   //!
   TBranch        *b_jtNbHad;   //!
   TBranch        *b_jtNcHad;   //!
   TBranch        *b_jtNbPar;   //!
   TBranch        *b_jtNcPar;   //!
   TBranch        *b_jtHasGSPB;   //!
   TBranch        *b_jtHasGSPC;   //!
   TBranch        *b_jtIsHardest;   //!
   TBranch        *b_sjt1E;   //!
   TBranch        *b_sjt1Pt;   //!
   TBranch        *b_sjt1Eta;   //!
   TBranch        *b_sjt1Phi;   //!
   TBranch        *b_sjt1HadFlav;   //!
   TBranch        *b_sjt1ParFlav;   //!
   TBranch        *b_sjt2E;   //!
   TBranch        *b_sjt2Pt;   //!
   TBranch        *b_sjt2Eta;   //!
   TBranch        *b_sjt2Phi;   //!
   TBranch        *b_sjt2HadFlav;   //!
   TBranch        *b_sjt2ParFlav;   //!
   TBranch        *b_rsjt1E;   //!
   TBranch        *b_rsjt1Pt;   //!
   TBranch        *b_rsjt1Eta;   //!
   TBranch        *b_rsjt1Phi;   //!
   TBranch        *b_rsjt2E;   //!
   TBranch        *b_rsjt2Pt;   //!
   TBranch        *b_rsjt2Eta;   //!
   TBranch        *b_rsjt2Phi;   //!
   TBranch        *b_jtDiscCSVV2;   //!
   TBranch        *b_jtDiscDeepCSVB;   //!
   TBranch        *b_jtDiscDeepCSVBB;   //!
   TBranch        *b_jtDiscDeepCSVC;   //!
   TBranch        *b_jtDiscDeepFlavourB;   //!
   TBranch        *b_jtDiscDeepFlavourBB;   //!
   TBranch        *b_jtDiscDeepFlavourLEPB;   //!
   TBranch        *b_jtDiscDeepFlavourC;   //!
   TBranch        *b_jtDiscProb;   //!
   TBranch        *b_nsvtx;   //!
   TBranch        *b_svtxntrk;   //!
   TBranch        *b_svtxdls;   //!
   TBranch        *b_svtxdls2d;   //!
   TBranch        *b_svtxm;   //!
   TBranch        *b_svtxpt;   //!
   TBranch        *b_svtxmcorr;   //!
   TBranch        *b_svtxTrPt;   //!
   TBranch        *b_svtxTrEta;   //!
   TBranch        *b_svtxTrPhi;   //!
   TBranch        *b_nselIPtrk;   //!
   TBranch        *b_nIP;   //!
   TBranch        *b_ipPt;   //!
   TBranch        *b_ipEta;   //!
   TBranch        *b_ipPhi;   //!
   TBranch        *b_ipProb;   //!
   TBranch        *b_ip3dSig;   //!
   TBranch        *b_ipPtMatch;   //!
   TBranch        *b_ipEtaMatch;   //!
   TBranch        *b_ipPhiMatch;   //!
   TBranch        *b_ipMatchStatus;   //!
   TBranch        *b_refpt;   //!
   TBranch        *b_refeta;   //!
   TBranch        *b_refphi;   //!
   TBranch        *b_refdphijt;   //!
   TBranch        *b_refdrjt;   //!
   TBranch        *b_refparton_pt;   //!
   TBranch        *b_refparton_flavor;   //!
   TBranch        *b_refIsHardest;   //!
   TBranch        *b_ngen;   //!
   TBranch        *b_genmatchindex;   //!
   TBranch        *b_genpt;   //!
   TBranch        *b_geneta;   //!
   TBranch        *b_genphi;   //!
   TBranch        *b_npar;   //!
   TBranch        *b_parpt;   //!
   TBranch        *b_pareta;   //!
   TBranch        *b_parphi;   //!
   TBranch        *b_parNb;   //!
   TBranch        *b_parNc;   //!
   TBranch        *b_parHasGSPB;   //!
   TBranch        *b_parHasGSPC;   //!
   TBranch        *b_genIsHardest;   //!
   TBranch        *b_gsjt1E;   //!
   TBranch        *b_gsjt1Pt;   //!
   TBranch        *b_gsjt1Eta;   //!
   TBranch        *b_gsjt1Phi;   //!
   TBranch        *b_gsjt2Pt;   //!
   TBranch        *b_gsjt2Eta;   //!
   TBranch        *b_gsjt2Phi;   //!
   TBranch        *b_psjt1E;   //!
   TBranch        *b_psjt1Pt;   //!
   TBranch        *b_psjt1Eta;   //!
   TBranch        *b_psjt1Phi;   //!
   TBranch        *b_psjt2Pt;   //!
   TBranch        *b_psjt2Eta;   //!
   TBranch        *b_psjt2Phi;   //!

   t(TTree *tree=0);
   virtual ~t();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef t_cxx
t::t(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/data_CMS/cms/kalipoliti/chargedSJ_mergedSVtracks_gen_reco/merged_HiForestAOD.root:/ak4PFJetAnalyzer");
      dir->GetObject("t",tree);

   }
   Init(tree);
}

t::~t()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t t::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t t::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void t::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   svtxntrk = 0;
   svtxdls = 0;
   svtxdls2d = 0;
   svtxm = 0;
   svtxpt = 0;
   svtxmcorr = 0;
   svtxTrPt = 0;
   svtxTrEta = 0;
   svtxTrPhi = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("nref", &nref, &b_nref);
   fChain->SetBranchAddress("rawpt", rawpt, &b_rawpt);
   fChain->SetBranchAddress("jtpt", jtpt, &b_jtpt);
   fChain->SetBranchAddress("jteta", jteta, &b_jteta);
   fChain->SetBranchAddress("jtphi", jtphi, &b_jtphi);
   fChain->SetBranchAddress("jtHadFlav", jtHadFlav, &b_jtHadFlav);
   fChain->SetBranchAddress("jtParFlav", jtParFlav, &b_jtParFlav);
   fChain->SetBranchAddress("jtNbHad", jtNbHad, &b_jtNbHad);
   fChain->SetBranchAddress("jtNcHad", jtNcHad, &b_jtNcHad);
   fChain->SetBranchAddress("jtNbPar", jtNbPar, &b_jtNbPar);
   fChain->SetBranchAddress("jtNcPar", jtNcPar, &b_jtNcPar);
   fChain->SetBranchAddress("jtHasGSPB", jtHasGSPB, &b_jtHasGSPB);
   fChain->SetBranchAddress("jtHasGSPC", jtHasGSPC, &b_jtHasGSPC);
   fChain->SetBranchAddress("jtIsHardest", jtIsHardest, &b_jtIsHardest);
   fChain->SetBranchAddress("sjt1E", sjt1E, &b_sjt1E);
   fChain->SetBranchAddress("sjt1Pt", sjt1Pt, &b_sjt1Pt);
   fChain->SetBranchAddress("sjt1Eta", sjt1Eta, &b_sjt1Eta);
   fChain->SetBranchAddress("sjt1Phi", sjt1Phi, &b_sjt1Phi);
   fChain->SetBranchAddress("sjt1HadFlav", sjt1HadFlav, &b_sjt1HadFlav);
   fChain->SetBranchAddress("sjt1ParFlav", sjt1ParFlav, &b_sjt1ParFlav);
   fChain->SetBranchAddress("sjt2E", sjt2E, &b_sjt2E);
   fChain->SetBranchAddress("sjt2Pt", sjt2Pt, &b_sjt2Pt);
   fChain->SetBranchAddress("sjt2Eta", sjt2Eta, &b_sjt2Eta);
   fChain->SetBranchAddress("sjt2Phi", sjt2Phi, &b_sjt2Phi);
   fChain->SetBranchAddress("sjt2HadFlav", sjt2HadFlav, &b_sjt2HadFlav);
   fChain->SetBranchAddress("sjt2ParFlav", sjt2ParFlav, &b_sjt2ParFlav);
   fChain->SetBranchAddress("rsjt1E", rsjt1E, &b_rsjt1E);
   fChain->SetBranchAddress("rsjt1Pt", rsjt1Pt, &b_rsjt1Pt);
   fChain->SetBranchAddress("rsjt1Eta", rsjt1Eta, &b_rsjt1Eta);
   fChain->SetBranchAddress("rsjt1Phi", rsjt1Phi, &b_rsjt1Phi);
   fChain->SetBranchAddress("rsjt2E", rsjt2E, &b_rsjt2E);
   fChain->SetBranchAddress("rsjt2Pt", rsjt2Pt, &b_rsjt2Pt);
   fChain->SetBranchAddress("rsjt2Eta", rsjt2Eta, &b_rsjt2Eta);
   fChain->SetBranchAddress("rsjt2Phi", rsjt2Phi, &b_rsjt2Phi);
   fChain->SetBranchAddress("jtDiscCSVV2", jtDiscCSVV2, &b_jtDiscCSVV2);
   fChain->SetBranchAddress("jtDiscDeepCSVB", jtDiscDeepCSVB, &b_jtDiscDeepCSVB);
   fChain->SetBranchAddress("jtDiscDeepCSVBB", jtDiscDeepCSVBB, &b_jtDiscDeepCSVBB);
   fChain->SetBranchAddress("jtDiscDeepCSVC", jtDiscDeepCSVC, &b_jtDiscDeepCSVC);
   fChain->SetBranchAddress("jtDiscDeepFlavourB", jtDiscDeepFlavourB, &b_jtDiscDeepFlavourB);
   fChain->SetBranchAddress("jtDiscDeepFlavourBB", jtDiscDeepFlavourBB, &b_jtDiscDeepFlavourBB);
   fChain->SetBranchAddress("jtDiscDeepFlavourLEPB", jtDiscDeepFlavourLEPB, &b_jtDiscDeepFlavourLEPB);
   fChain->SetBranchAddress("jtDiscDeepFlavourC", jtDiscDeepFlavourC, &b_jtDiscDeepFlavourC);
   fChain->SetBranchAddress("jtDiscProb", jtDiscProb, &b_jtDiscProb);
   fChain->SetBranchAddress("nsvtx", nsvtx, &b_nsvtx);
   fChain->SetBranchAddress("svtxntrk", &svtxntrk, &b_svtxntrk);
   fChain->SetBranchAddress("svtxdls", &svtxdls, &b_svtxdls);
   fChain->SetBranchAddress("svtxdls2d", &svtxdls2d, &b_svtxdls2d);
   fChain->SetBranchAddress("svtxm", &svtxm, &b_svtxm);
   fChain->SetBranchAddress("svtxpt", &svtxpt, &b_svtxpt);
   fChain->SetBranchAddress("svtxmcorr", &svtxmcorr, &b_svtxmcorr);
   fChain->SetBranchAddress("svtxTrPt", &svtxTrPt, &b_svtxTrPt);
   fChain->SetBranchAddress("svtxTrEta", &svtxTrEta, &b_svtxTrEta);
   fChain->SetBranchAddress("svtxTrPhi", &svtxTrPhi, &b_svtxTrPhi);
   fChain->SetBranchAddress("nselIPtrk", nselIPtrk, &b_nselIPtrk);
   fChain->SetBranchAddress("nIP", &nIP, &b_nIP);
   fChain->SetBranchAddress("ipPt", ipPt, &b_ipPt);
   fChain->SetBranchAddress("ipEta", ipEta, &b_ipEta);
   fChain->SetBranchAddress("ipPhi", ipPhi, &b_ipPhi);
   fChain->SetBranchAddress("ipProb", ipProb, &b_ipProb);
   fChain->SetBranchAddress("ip3dSig", ip3dSig, &b_ip3dSig);
   fChain->SetBranchAddress("ipPtMatch", ipPtMatch, &b_ipPtMatch);
   fChain->SetBranchAddress("ipEtaMatch", ipEtaMatch, &b_ipEtaMatch);
   fChain->SetBranchAddress("ipPhiMatch", ipPhiMatch, &b_ipPhiMatch);
   fChain->SetBranchAddress("ipMatchStatus", ipMatchStatus, &b_ipMatchStatus);
   fChain->SetBranchAddress("refpt", refpt, &b_refpt);
   fChain->SetBranchAddress("refeta", refeta, &b_refeta);
   fChain->SetBranchAddress("refphi", refphi, &b_refphi);
   fChain->SetBranchAddress("refdphijt", refdphijt, &b_refdphijt);
   fChain->SetBranchAddress("refdrjt", refdrjt, &b_refdrjt);
   fChain->SetBranchAddress("refparton_pt", refparton_pt, &b_refparton_pt);
   fChain->SetBranchAddress("refparton_flavor", refparton_flavor, &b_refparton_flavor);
   fChain->SetBranchAddress("refIsHardest", refIsHardest, &b_refIsHardest);
   fChain->SetBranchAddress("ngen", &ngen, &b_ngen);
   fChain->SetBranchAddress("genmatchindex", genmatchindex, &b_genmatchindex);
   fChain->SetBranchAddress("genpt", genpt, &b_genpt);
   fChain->SetBranchAddress("geneta", geneta, &b_geneta);
   fChain->SetBranchAddress("genphi", genphi, &b_genphi);
   fChain->SetBranchAddress("npar", &npar, &b_npar);
   fChain->SetBranchAddress("parpt", parpt, &b_parpt);
   fChain->SetBranchAddress("pareta", pareta, &b_pareta);
   fChain->SetBranchAddress("parphi", parphi, &b_parphi);
   fChain->SetBranchAddress("parNb", parNb, &b_parNb);
   fChain->SetBranchAddress("parNc", parNc, &b_parNc);
   fChain->SetBranchAddress("parHasGSPB", parHasGSPB, &b_parHasGSPB);
   fChain->SetBranchAddress("parHasGSPC", parHasGSPC, &b_parHasGSPC);
   fChain->SetBranchAddress("genIsHardest", genIsHardest, &b_genIsHardest);
   fChain->SetBranchAddress("gsjt1E", gsjt1E, &b_gsjt1E);
   fChain->SetBranchAddress("gsjt1Pt", gsjt1Pt, &b_gsjt1Pt);
   fChain->SetBranchAddress("gsjt1Eta", gsjt1Eta, &b_gsjt1Eta);
   fChain->SetBranchAddress("gsjt1Phi", gsjt1Phi, &b_gsjt1Phi);
   fChain->SetBranchAddress("gsjt2Pt", gsjt2Pt, &b_gsjt2Pt);
   fChain->SetBranchAddress("gsjt2Eta", gsjt2Eta, &b_gsjt2Eta);
   fChain->SetBranchAddress("gsjt2Phi", gsjt2Phi, &b_gsjt2Phi);
   fChain->SetBranchAddress("psjt1E", psjt1E, &b_psjt1E);
   fChain->SetBranchAddress("psjt1Pt", psjt1Pt, &b_psjt1Pt);
   fChain->SetBranchAddress("psjt1Eta", psjt1Eta, &b_psjt1Eta);
   fChain->SetBranchAddress("psjt1Phi", psjt1Phi, &b_psjt1Phi);
   fChain->SetBranchAddress("psjt2Pt", psjt2Pt, &b_psjt2Pt);
   fChain->SetBranchAddress("psjt2Eta", psjt2Eta, &b_psjt2Eta);
   fChain->SetBranchAddress("psjt2Phi", psjt2Phi, &b_psjt2Phi);
   Notify();
}

Bool_t t::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void t::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t t::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef t_cxx
