#include <iostream>
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"

void plotNref(){

   TFile *f;
   f = new TFile("/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root");
   
   TTree *t;
   t = (TTree *) f->Get("ak4PFJetAnalyzer/t");

   // Following lines were created automatically
   // Declaration of leaves types
   Int_t           nref;
   Float_t         rawpt[15];
   Float_t         jtpt[15];
   Float_t         jteta[15];
   Float_t         jtphi[15];
   Float_t         jtHadFlav[15];
   Float_t         jtParFlav[15];
   Int_t           jtNbHad[15];
   Int_t           jtNcHad[15];
   Int_t           jtNbPar[15];
   Int_t           jtNcPar[15];
   Bool_t          jtHasGSPB[15];
   Bool_t          jtHasGSPC[15];
   Bool_t          jtIsHardest[15];
   Float_t         sjt1Pt[15];
   Float_t         sjt1Eta[15];
   Float_t         sjt1Phi[15];
   Float_t         sjt1HadFlav[15];
   Float_t         sjt1ParFlav[15];
   Float_t         sjt2Pt[15];
   Float_t         sjt2Eta[15];
   Float_t         sjt2Phi[15];
   Float_t         sjt2HadFlav[15];
   Float_t         sjt2ParFlav[15];
   Float_t         rsjt1Pt[15];
   Float_t         rsjt1Eta[15];
   Float_t         rsjt1Phi[15];
   Float_t         rsjt2Pt[15];
   Float_t         rsjt2Eta[15];
   Float_t         rsjt2Phi[15];
   Float_t         jtDiscCSVV2[15];
   Float_t         jtDiscDeepCSVB[15];
   Float_t         jtDiscDeepCSVBB[15];
   Float_t         jtDiscDeepCSVC[15];
   Float_t         jtDiscDeepFlavourB[15];
   Float_t         jtDiscDeepFlavourBB[15];
   Float_t         jtDiscDeepFlavourLEPB[15];
   Float_t         jtDiscDeepFlavourC[15];
   Float_t         jtDiscProb[15];
   Int_t           nsvtx[15];
   vector<vector<int> > svtxntrk;
   vector<vector<float> > svtxdls;
   vector<vector<float> > svtxdls2d;
   vector<vector<float> > svtxm;
   vector<vector<float> > svtxpt;
   vector<vector<float> > svtxmcorr;
   Float_t         refpt[15];
   Float_t         refeta[15];
   Float_t         refphi[15];
   Float_t         refdphijt[15];
   Float_t         refdrjt[15];
   Float_t         refparton_pt[15];
   Int_t           refparton_flavor[15];
   Bool_t          refIsHardest[15];
   Int_t           ngen;
   Int_t           genmatchindex[19];
   Float_t         genpt[19];
   Float_t         geneta[19];
   Float_t         genphi[19];
   Int_t           npar;
   Float_t         parpt[21];
   Float_t         pareta[21];
   Float_t         parphi[21];
   Int_t           parNb[21];
   Int_t           parNc[21];
   Bool_t          parHasGSPB[21];
   Bool_t          parHasGSPC[21];
   Bool_t          genIsHardest[19];
   Float_t         gsjt1Pt[19];
   Float_t         gsjt1Eta[19];
   Float_t         gsjt1Phi[19];
   Float_t         gsjt2Pt[19];
   Float_t         gsjt2Eta[19];
   Float_t         gsjt2Phi[19];
   Float_t         psjt1Pt[19];
   Float_t         psjt1Eta[19];
   Float_t         psjt1Phi[19];
   Float_t         psjt2Pt[19];
   Float_t         psjt2Eta[19];
   Float_t         psjt2Phi[19];

   // Set branch addresses.
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
   t->SetBranchAddress("sjt1Pt",sjt1Pt);
   t->SetBranchAddress("sjt1Eta",sjt1Eta);
   t->SetBranchAddress("sjt1Phi",sjt1Phi);
   t->SetBranchAddress("sjt1HadFlav",sjt1HadFlav);
   t->SetBranchAddress("sjt1ParFlav",sjt1ParFlav);
   t->SetBranchAddress("sjt2Pt",sjt2Pt);
   t->SetBranchAddress("sjt2Eta",sjt2Eta);
   t->SetBranchAddress("sjt2Phi",sjt2Phi);
   t->SetBranchAddress("sjt2HadFlav",sjt2HadFlav);
   t->SetBranchAddress("sjt2ParFlav",sjt2ParFlav);
   t->SetBranchAddress("rsjt1Pt",rsjt1Pt);
   t->SetBranchAddress("rsjt1Eta",rsjt1Eta);
   t->SetBranchAddress("rsjt1Phi",rsjt1Phi);
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
   /*
   t->SetBranchAddress("svtxntrk",&svtxntrk);
   t->SetBranchAddress("svtxdls",&svtxdls);
   t->SetBranchAddress("svtxdls2d",&svtxdls2d);
   t->SetBranchAddress("svtxm",&svtxm);
   t->SetBranchAddress("svtxpt",&svtxpt);
   t->SetBranchAddress("svtxmcorr",&svtxmcorr);
   */
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
   t->SetBranchAddress("gsjt1Pt",gsjt1Pt);
   t->SetBranchAddress("gsjt1Eta",gsjt1Eta);
   t->SetBranchAddress("gsjt1Phi",gsjt1Phi);
   t->SetBranchAddress("gsjt2Pt",gsjt2Pt);
   t->SetBranchAddress("gsjt2Eta",gsjt2Eta);
   t->SetBranchAddress("gsjt2Phi",gsjt2Phi);
   t->SetBranchAddress("psjt1Pt",psjt1Pt);
   t->SetBranchAddress("psjt1Eta",psjt1Eta);
   t->SetBranchAddress("psjt1Phi",psjt1Phi);
   t->SetBranchAddress("psjt2Pt",psjt2Pt);
   t->SetBranchAddress("psjt2Eta",psjt2Eta);
   t->SetBranchAddress("psjt2Phi",psjt2Phi);

   // Disable all branches
   t->SetBranchStatus("*", 0);
   
   // Activate only these branches
   t->SetBranchStatus("nref", 1);
   t->SetBranchStatus("ngen", 1);

   // Prepare 2D histogram with nref vs ngen  
   TH2F *h = new TH2F("nrefVSngen", "nrefVSngen", 6, -.5, 5.5, 6, -0.5, 5.5);

   Long64_t nentries = t->GetEntries();

   for (Long64_t i = 0; i < nentries; i++){

     if(i%1000000==0) cout<<" i = "<<i<<endl;
   
     t->GetEntry(i);
     
     Int_t x = nref;
     Int_t y = ngen;

     h->Fill(x, y, 1);
   }
   
   h->SetXTitle("nref");
   h->SetYTitle("ngen");
   h->SetTitle("Nv of generated vs Nb of reconstructed jets");
   TFile *fout = new TFile("myFile.root", "recreate");
   h->Write();
   fout->Close();

}
