{
//////////////////////////////////////////////////////////
//   This file has been automatically generated 
//     (Fri Mar  4 10:07:25 2022 by ROOT version6.10/09)
//   from TTree HiTree/
//   found on file: /data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root
//////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
   gROOT->Reset();
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root");
   if (!f) {
      f = new TFile("/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root");
   }
    TDirectory * dir = (TDirectory*)f->Get("/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root:/hiEvtAnalyzer");
    dir->GetObject("HiTree",tree);

//Declaration of leaves types
   UInt_t          run;
   ULong64_t       evt;
   UInt_t          lumi;
   Float_t         vx;
   Float_t         vy;
   Float_t         vz;
   Float_t         pthat;
   Float_t         weight;

   // Set branch addresses.
   HiTree->SetBranchAddress("run",&run);
   HiTree->SetBranchAddress("evt",&evt);
   HiTree->SetBranchAddress("lumi",&lumi);
   HiTree->SetBranchAddress("vx",&vx);
   HiTree->SetBranchAddress("vy",&vy);
   HiTree->SetBranchAddress("vz",&vz);
   HiTree->SetBranchAddress("pthat",&pthat);
   HiTree->SetBranchAddress("weight",&weight);

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// HiTree->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   Long64_t nentries = HiTree->GetEntries();

   Long64_t nbytes = 0;
//   for (Long64_t i=0; i<nentries;i++) {
//      nbytes += HiTree->GetEntry(i);
//   }
}
