#include <iostream>
#include "TFile.h"
#include "TH2.h"
#include "TTree.h"


void plotSubstructure(bool doLateSD = false){

  TFile *fL= NULL;
  if(doLateSD)fL= new TFile("/data_CMS/cms/mnguyen/bJet2022/qcdMC/lateSD/merged_HiForestAOD.root");
  else fL= new TFile("/data_CMS/cms/mnguyen/bJet2022/qcdMC/SD/merged_HiForestAOD.root");
  TTree *tL = (TTree*) fL->Get("ak4PFJetAnalyzer/t");
  TTree *tEvL = (TTree*) fL->Get("hiEvtAnalyzer/HiTree");


//Declaration of leaves types
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
   Float_t         rsjt1Pt[30];
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

   // Set branch addresses.
   tL->SetBranchAddress("nref",&nref);
   tL->SetBranchAddress("rawpt",rawpt);
   tL->SetBranchAddress("jtpt",jtpt);
   tL->SetBranchAddress("jteta",jteta);
   tL->SetBranchAddress("jtphi",jtphi);
   tL->SetBranchAddress("jtHadFlav",jtHadFlav);
   tL->SetBranchAddress("jtParFlav",jtParFlav);
   tL->SetBranchAddress("jtNbHad",jtNbHad);
   tL->SetBranchAddress("jtNcHad",jtNcHad);
   tL->SetBranchAddress("jtNbPar",jtNbPar);
   tL->SetBranchAddress("jtNcPar",jtNcPar);
   tL->SetBranchAddress("jtHasGSPB",jtHasGSPB);
   tL->SetBranchAddress("jtHasGSPC",jtHasGSPC);
   tL->SetBranchAddress("jtIsHardest",jtIsHardest);
   tL->SetBranchAddress("sjt1Pt",sjt1Pt);
   tL->SetBranchAddress("sjt1Eta",sjt1Eta);
   tL->SetBranchAddress("sjt1Phi",sjt1Phi);
   tL->SetBranchAddress("sjt1HadFlav",sjt1HadFlav);
   tL->SetBranchAddress("sjt1ParFlav",sjt1ParFlav);
   tL->SetBranchAddress("sjt2Pt",sjt2Pt);
   tL->SetBranchAddress("sjt2Eta",sjt2Eta);
   tL->SetBranchAddress("sjt2Phi",sjt2Phi);
   tL->SetBranchAddress("sjt2HadFlav",sjt2HadFlav);
   tL->SetBranchAddress("sjt2ParFlav",sjt2ParFlav);
   tL->SetBranchAddress("rsjt1Pt",rsjt1Pt);
   tL->SetBranchAddress("rsjt1Eta",rsjt1Eta);
   tL->SetBranchAddress("rsjt1Phi",rsjt1Phi);
   tL->SetBranchAddress("rsjt2Pt",rsjt2Pt);
   tL->SetBranchAddress("rsjt2Eta",rsjt2Eta);
   tL->SetBranchAddress("rsjt2Phi",rsjt2Phi);
   tL->SetBranchAddress("jtDiscCSVV2",jtDiscCSVV2);
   tL->SetBranchAddress("jtDiscDeepCSVB",jtDiscDeepCSVB);
   tL->SetBranchAddress("jtDiscDeepCSVBB",jtDiscDeepCSVBB);
   tL->SetBranchAddress("jtDiscDeepCSVC",jtDiscDeepCSVC);
   tL->SetBranchAddress("jtDiscDeepFlavourB",jtDiscDeepFlavourB);
   tL->SetBranchAddress("jtDiscDeepFlavourBB",jtDiscDeepFlavourBB);
   tL->SetBranchAddress("jtDiscDeepFlavourLEPB",jtDiscDeepFlavourLEPB);
   tL->SetBranchAddress("jtDiscDeepFlavourC",jtDiscDeepFlavourC);
   tL->SetBranchAddress("jtDiscProb",jtDiscProb);
   /*
   tL->SetBranchAddress("nsvtx",nsvtx);
   tL->SetBranchAddress("svtxntrk",&svtxntrk);
   tL->SetBranchAddress("svtxdls",&svtxdls);
   tL->SetBranchAddress("svtxdls2d",&svtxdls2d);
   tL->SetBranchAddress("svtxm",&svtxm);
   tL->SetBranchAddress("svtxpt",&svtxpt);
   tL->SetBranchAddress("svtxmcorr",&svtxmcorr);
   */
   tL->SetBranchAddress("refpt",refpt);
   tL->SetBranchAddress("refeta",refeta);
   tL->SetBranchAddress("refphi",refphi);
   tL->SetBranchAddress("refdphijt",refdphijt);
   tL->SetBranchAddress("refdrjt",refdrjt);
   tL->SetBranchAddress("refparton_pt",refparton_pt);
   tL->SetBranchAddress("refparton_flavor",refparton_flavor);
   tL->SetBranchAddress("refIsHardest",refIsHardest);
   tL->SetBranchAddress("ngen",&ngen);
   tL->SetBranchAddress("genmatchindex",genmatchindex);
   tL->SetBranchAddress("genpt",genpt);
   tL->SetBranchAddress("geneta",geneta);
   tL->SetBranchAddress("genphi",genphi);
   tL->SetBranchAddress("npar",&npar);
   tL->SetBranchAddress("parpt",parpt);
   tL->SetBranchAddress("pareta",pareta);
   tL->SetBranchAddress("parphi",parphi);
   tL->SetBranchAddress("parNb",parNb);
   tL->SetBranchAddress("parNc",parNc);
   tL->SetBranchAddress("parHasGSPB",parHasGSPB);
   tL->SetBranchAddress("parHasGSPC",parHasGSPC);
   tL->SetBranchAddress("genIsHardest",genIsHardest);
   tL->SetBranchAddress("gsjt1Pt",gsjt1Pt);
   tL->SetBranchAddress("gsjt1Eta",gsjt1Eta);
   tL->SetBranchAddress("gsjt1Phi",gsjt1Phi);
   tL->SetBranchAddress("gsjt2Pt",gsjt2Pt);
   tL->SetBranchAddress("gsjt2Eta",gsjt2Eta);
   tL->SetBranchAddress("gsjt2Phi",gsjt2Phi);
   tL->SetBranchAddress("psjt1Pt",psjt1Pt);
   tL->SetBranchAddress("psjt1Eta",psjt1Eta);
   tL->SetBranchAddress("psjt1Phi",psjt1Phi);
   tL->SetBranchAddress("psjt2Pt",psjt2Pt);
   tL->SetBranchAddress("psjt2Eta",psjt2Eta);
   tL->SetBranchAddress("psjt2Phi",psjt2Phi);

   float weight;
   tEvL->SetBranchAddress("weight",&weight);


   tL->SetBranchStatus("*",0);  // disable all branches
   tL->SetBranchStatus("nref",1);
   tL->SetBranchStatus("jtpt",1);
   tL->SetBranchStatus("jteta",1);
   tL->SetBranchStatus("sjt1Pt",1);
   tL->SetBranchStatus("sjt2Pt",1);
   tL->SetBranchStatus("sjt1Phi",1);
   tL->SetBranchStatus("sjt2Phi",1);
   tL->SetBranchStatus("sjt1Eta",1);
   tL->SetBranchStatus("sjt2Eta",1);
   tL->SetBranchStatus("jtHadFlav",1);
   tL->SetBranchStatus("jtIsHardest",1);


   TH2F *hB_ZgVsJetPt = new TH2F("hB_ZgVsJetPt","hB_ZgVsJetPt",50,0,0.5,48,20,500);
   TH2F *hC_ZgVsJetPt = new TH2F("hC_ZgVsJetPt","hC_ZgVsJetPt",50,0,0.5,48,20,500);
   TH2F *hL_ZgVsJetPt = new TH2F("hL_ZgVsJetPt","hL_ZgVsJetPt",50,0,0.5,48,20,500);

   TH2F *hB_ZgVsJetPt_dynKt = new TH2F("hB_ZgVsJetPt_dynKt","hB_ZgVsJetPt_dynKt",50,0,0.5,48,20,500);
   TH2F *hC_ZgVsJetPt_dynKt = new TH2F("hC_ZgVsJetPt_dynKt","hC_ZgVsJetPt_dynKt",50,0,0.5,48,20,500);
   TH2F *hL_ZgVsJetPt_dynKt = new TH2F("hL_ZgVsJetPt_dynKt","hL_ZgVsJetPt_dynKt",50,0,0.5,48,20,500);

   TH2F *hB_RgVsJetPt = new TH2F("hB_RgVsJetPt","hB_RgVsJetPt",50,0,0.5,48,20,500);
   TH2F *hC_RgVsJetPt = new TH2F("hC_RgVsJetPt","hC_RgVsJetPt",50,0,0.5,48,20,500);
   TH2F *hL_RgVsJetPt = new TH2F("hL_RgVsJetPt","hL_RgVsJetPt",50,0,0.5,48,20,500);

   TH2F *hB_RgVsJetPt_dynKt = new TH2F("hB_RgVsJetPt_dynKt","hB_RgVsJetPt_dynKt",50,0,0.5,48,20,500);
   TH2F *hC_RgVsJetPt_dynKt = new TH2F("hC_RgVsJetPt_dynKt","hC_RgVsJetPt_dynKt",50,0,0.5,48,20,500);
   TH2F *hL_RgVsJetPt_dynKt = new TH2F("hL_RgVsJetPt_dynKt","hL_RgVsJetPt_dynKt",50,0,0.5,48,20,500);


   Long64_t nentries = tL->GetEntries();

   for (Long64_t i=0; i<nentries;i++) {

     if(i%1000000==0) cout<<" i = "<<i<<endl;
     tL->GetEntry(i);
     tEvL->GetEntry(i);

     for (int j=0; j<nref;j++) {
       
       if(fabs(jteta[j])>2.) continue;

       double rg = 0.;
       double zg = 0.;
       
       if(sjt2Pt[j] > 0.){
	 zg = sjt2Pt[j]/(sjt1Pt[j]+sjt2Pt[j]);
	 double deta = sjt1Eta[j]-sjt2Eta[j];
	 double dphi = acos(cos(sjt1Phi[j]-sjt2Phi[j]));
	 rg = sqrt(deta*deta+dphi*dphi);
       }


       if(jtHadFlav[j]==0){
	 hL_ZgVsJetPt->Fill(zg,jtpt[j], weight);
	 hL_RgVsJetPt->Fill(rg,jtpt[j], weight);
       }
       else if(jtHadFlav[j]==5){
	 hB_ZgVsJetPt->Fill(zg,jtpt[j], weight);
	 hB_RgVsJetPt->Fill(rg,jtpt[j], weight);
       }
       else if(jtHadFlav[j]==4){
	 hC_ZgVsJetPt->Fill(zg,jtpt[j], weight);
	 hC_RgVsJetPt->Fill(rg,jtpt[j], weight);
       }       

       // zero out if not hardest
       if(!jtIsHardest[j]){
	 rg=0.;
	 zg=0.;
       }
       
       if(jtHadFlav[j]==0){
	 hL_ZgVsJetPt_dynKt->Fill(zg,jtpt[j], weight);
	 hL_RgVsJetPt_dynKt->Fill(rg,jtpt[j], weight);
       }
       else if(jtHadFlav[j]==5){
	 hB_ZgVsJetPt_dynKt->Fill(zg,jtpt[j], weight);
	 hB_RgVsJetPt_dynKt->Fill(rg,jtpt[j], weight);
       }
       else if(jtHadFlav[j]==4){
	 hC_ZgVsJetPt_dynKt->Fill(zg,jtpt[j], weight);	 
	 hC_RgVsJetPt_dynKt->Fill(rg,jtpt[j], weight);	 
       }
     }
     
   }
   /*
   TFile *fout = NULL;
   if(doLateSD)fout = new TFile("qcd_lateSD.root","recreate");  
   else fout = new TFile("qcd_SD.root","recreate");  
   hB_ZgVsJetPt->Write();
   hC_ZgVsJetPt->Write();
   hL_ZgVsJetPt->Write();
   hB_ZgVsJetPt_dynKt->Write();
   hC_ZgVsJetPt_dynKt->Write();
   hL_ZgVsJetPt_dynKt->Write();

   hB_RgVsJetPt->Write();
   hC_RgVsJetPt->Write();
   hL_RgVsJetPt->Write();
   hB_RgVsJetPt_dynKt->Write();
   hC_RgVsJetPt_dynKt->Write();
   hL_RgVsJetPt_dynKt->Write();

   fout->Close();
   */
}
