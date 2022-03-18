#include <iostream>
#include "TFile.h"
#include "TTree.h"

void make_small_tree(bool inclusive = false){

  TFile *fin;

  if (inclusive) {
    printf("\nOpening inclusive MC file.");
    fin = new TFile("/data_CMS/cms/mnguyen//bJet2022/qcdMC/SD/merged_HiForestAOD.root");
  }
  else {
    printf("\nOpening bjet MC file.");
    fin = new TFile("/data_CMS/cms/mnguyen//bJet2022/bJetMC/SD/merged_HiForestAOD.root");
  }

  printf("\nReading tree...");
  TTree *oldtree = (TTree *) fin->Get("ak4PFJetAnalyzer/t");
  //  fin->Close();

  printf("\nDeactivating all branches.");
  oldtree->SetBranchStatus("*", 0);
  for (auto activeBranchName : {"psjt1Pt", "psjt1Eta", "psjt1Phi", "psjt2Pt", "psjt2Eta", "psjt2Phi"}) {
    printf("\nActivating branch %s.", activeBranchName);
    oldtree->SetBranchStatus(activeBranchName, 1);
  }

  // New file to save the smaller tree
  TFile *fout;
  if (inclusive) {
    printf("\nCreating new file named small_qcdMC.root\n");
    fout = new TFile("small_qcdMC.root", "recreate");
  }
  else {
    printf("\nCreating new file named small_bJetMC.root\n");
    fout = new TFile("small_bJetMC.root", "recreate");
  }

  printf("Copying tree...");

  auto newtree = oldtree->CloneTree();
  newtree->Print();
  fout->Write();
  // fout->Close();
}


