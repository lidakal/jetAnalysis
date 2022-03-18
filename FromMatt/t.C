{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Tue Mar  1 10:48:08 2022 by ROOT version6.10/09)
//   from TTree t/ak4PFpatJetsWithBtagging Jet Analysis Tree
//   found on file: /data_CMS/cms/mnguyen/bJet2022/qcdMC/lateSD/merged_HiForestAOD.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
   gROOT->Reset();
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data_CMS/cms/mnguyen/bJet2022/qcdMC/lateSD/merged_HiForestAOD.root");
   if (!f) {
      f = new TFile("/data_CMS/cms/mnguyen/bJet2022/qcdMC/lateSD/merged_HiForestAOD.root");
   }
    TDirectory * dir = (TDirectory*)f->Get("/data_CMS/cms/mnguyen/bJet2022/qcdMC/lateSD/merged_HiForestAOD.root:/ak4PFJetAnalyzer");
    dir->GetObject("t",tree);

//Declaration of leaves types
   Int_t           nref;
   Float_t         rawpt[14];
   Float_t         jtpt[14];
   Float_t         jteta[14];
   Float_t         jtphi[14];
   Float_t         jtHadFlav[14];
   Float_t         jtParFlav[14];
   Int_t           jtNbHad[14];
   Int_t           jtNcHad[14];
   Int_t           jtNbPar[14];
   Int_t           jtNcPar[14];
   Bool_t          jtHasGSPB[14];
   Bool_t          jtHasGSPC[14];
   Bool_t          jtIsHardest[14];
   Float_t         sjt1Pt[14];
   Float_t         sjt1Eta[14];
   Float_t         sjt1Phi[14];
   Float_t         sjt1HadFlav[14];
   Float_t         sjt1ParFlav[14];
   Float_t         sjt2Pt[14];
   Float_t         sjt2Eta[14];
   Float_t         sjt2Phi[14];
   Float_t         sjt2HadFlav[14];
   Float_t         sjt2ParFlav[14];
   Float_t         rsjt1Pt[14];
   Float_t         rsjt1Eta[14];
   Float_t         rsjt1Phi[14];
   Float_t         rsjt2Pt[14];
   Float_t         rsjt2Eta[14];
   Float_t         rsjt2Phi[14];
   Float_t         jtDiscCSVV2[14];
   Float_t         jtDiscDeepCSVB[14];
   Float_t         jtDiscDeepCSVBB[14];
   Float_t         jtDiscDeepCSVC[14];
   Float_t         jtDiscDeepFlavourB[14];
   Float_t         jtDiscDeepFlavourBB[14];
   Float_t         jtDiscDeepFlavourLEPB[14];
   Float_t         jtDiscDeepFlavourC[14];
   Float_t         jtDiscProb[14];
   Int_t           nsvtx[14];
   vector<vector<int> > svtxntrk;
   vector<vector<float> > svtxdls;
   vector<vector<float> > svtxdls2d;
   vector<vector<float> > svtxm;
   vector<vector<float> > svtxpt;
   vector<vector<float> > svtxmcorr;
   Float_t         refpt[14];
   Float_t         refeta[14];
   Float_t         refphi[14];
   Float_t         refdphijt[14];
   Float_t         refdrjt[14];
   Float_t         refparton_pt[14];
   Int_t           refparton_flavor[14];
   Bool_t          refIsHardest[14];
   Int_t           ngen;
   Int_t           genmatchindex[15];
   Float_t         genpt[15];
   Float_t         geneta[15];
   Float_t         genphi[15];
   Int_t           npar;
   Float_t         parpt[15];
   Float_t         pareta[15];
   Float_t         parphi[15];
   Int_t           parNb[15];
   Int_t           parNc[15];
   Bool_t          parHasGSPB[15];
   Bool_t          parHasGSPC[15];
   Bool_t          genIsHardest[15];
   Float_t         gsjt1Pt[15];
   Float_t         gsjt1Eta[15];
   Float_t         gsjt1Phi[15];
   Float_t         gsjt2Pt[15];
   Float_t         gsjt2Eta[15];
   Float_t         gsjt2Phi[15];
   Float_t         psjt1Pt[15];
   Float_t         psjt1Eta[15];
   Float_t         psjt1Phi[15];
   Float_t         psjt2Pt[15];
   Float_t         psjt2Eta[15];
   Float_t         psjt2Phi[15];

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
   t->SetBranchAddress("svtxntrk",&svtxntrk);
   t->SetBranchAddress("svtxdls",&svtxdls);
   t->SetBranchAddress("svtxdls2d",&svtxdls2d);
   t->SetBranchAddress("svtxm",&svtxm);
   t->SetBranchAddress("svtxpt",&svtxpt);
   t->SetBranchAddress("svtxmcorr",&svtxmcorr);
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

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// t->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   Long64_t nentries = t->GetEntries();

   Long64_t nbytes = 0;
//   for (Long64_t i=0; i<nentries;i++) {
//      nbytes += t->GetEntry(i);
//   }
}
