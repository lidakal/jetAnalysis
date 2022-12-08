#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include <vector>

class hiTree {
public :
   TString        fname;
   TTree          *tree;

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           event;
   Float_t         b;
   Float_t         npart;
   Float_t         ncoll;
   Float_t         nhard;
   Float_t         phi0;
   Float_t         scale;
   Int_t           n[10];
   Float_t         ptav[10];
   Int_t           mult;
   vector<float>   *pt;
   vector<float>   *eta;
   vector<float>   *phi;
   vector<int>     *pdg;
   vector<int>     *chg;
   vector<int>     *matchingID;
   vector<int>     *nMothers;
   vector<vector<int> > *motherIdx;
   vector<int>     *nDaughters;
   vector<vector<int> > *daughterIdx;
   vector<int>     *sta;
   vector<int>     *sube;
   Float_t         vx;
   Float_t         vy;
   Float_t         vz;
   Float_t         vr;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_b;   //!
   TBranch        *b_npart;   //!
   TBranch        *b_ncoll;   //!
   TBranch        *b_nhard;   //!
   TBranch        *b_phi0;   //!
   TBranch        *b_scale;   //!
   TBranch        *b_n;   //!
   TBranch        *b_ptav;   //!
   TBranch        *b_mult;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_chg;   //!
   TBranch        *b_matchingID;   //!
   TBranch        *b_nMothers;   //!
   TBranch        *b_motherIdx;   //!
   TBranch        *b_nDaughters;   //!
   TBranch        *b_daughterIdx;   //!
   TBranch        *b_sta;   //!
   TBranch        *b_sube;   //!
   TBranch        *b_vx;   //!
   TBranch        *b_vy;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_vr;   //!

   hiTree(TString rootf);
   ~hiTree();
   Int_t GetEntry(Long64_t entry);
   Long64_t GetEntries();
   void Init(TTree *tree);
   void SetBranchStatus(TString branchName, Int_t status);
   void SetBranchStatus(std::vector<TString> branchNames, Int_t status);
};

hiTree::hiTree(TString rootf)
{
   TFile *fin = new TFile(rootf);
   tree = (TTree*) fin->Get("bDecayAna/hi");

   Init(tree);
}

hiTree::~hiTree()
{
   if (!tree) return;
   delete tree;
}

Int_t hiTree::GetEntry(Long64_t entry)
{
   if (!tree) return 0;
   return tree->GetEntry(entry);
}

Long64_t hiTree::GetEntries()
{
   if (!tree) return 0;
   return tree->GetEntries();
}

void hiTree::Init(TTree *tree)
{
   // Set object pointer
   pt = 0;
   eta = 0;
   phi = 0;
   pdg = 0;
   chg = 0;
   matchingID = 0;
   nMothers = 0;
   motherIdx = 0;
   nDaughters = 0;
   daughterIdx = 0;
   sta = 0;
   sube = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;

   tree->SetBranchAddress("event", &event, &b_event);
   tree->SetBranchAddress("b", &b, &b_b);
   tree->SetBranchAddress("npart", &npart, &b_npart);
   tree->SetBranchAddress("ncoll", &ncoll, &b_ncoll);
   tree->SetBranchAddress("nhard", &nhard, &b_nhard);
   tree->SetBranchAddress("phi0", &phi0, &b_phi0);
   tree->SetBranchAddress("scale", &scale, &b_scale);
   tree->SetBranchAddress("n", n, &b_n);
   tree->SetBranchAddress("ptav", ptav, &b_ptav);
   tree->SetBranchAddress("mult", &mult, &b_mult);
   tree->SetBranchAddress("pt", &pt, &b_pt);
   tree->SetBranchAddress("eta", &eta, &b_eta);
   tree->SetBranchAddress("phi", &phi, &b_phi);
   tree->SetBranchAddress("pdg", &pdg, &b_pdg);
   tree->SetBranchAddress("chg", &chg, &b_chg);
   tree->SetBranchAddress("matchingID", &matchingID, &b_matchingID);
   tree->SetBranchAddress("nMothers", &nMothers, &b_nMothers);
   tree->SetBranchAddress("motherIdx", &motherIdx, &b_motherIdx);
   tree->SetBranchAddress("nDaughters", &nDaughters, &b_nDaughters);
   tree->SetBranchAddress("daughterIdx", &daughterIdx, &b_daughterIdx);
   tree->SetBranchAddress("sta", &sta, &b_sta);
   tree->SetBranchAddress("sube", &sube, &b_sube);
   tree->SetBranchAddress("vx", &vx, &b_vx);
   tree->SetBranchAddress("vy", &vy, &b_vy);
   tree->SetBranchAddress("vz", &vz, &b_vz);
   tree->SetBranchAddress("vr", &vr, &b_vr);
}

void hiTree::SetBranchStatus(TString branchName, Int_t status)
{
    tree->SetBranchStatus(branchName, status);
}

void hiTree::SetBranchStatus(vector<TString> branchNames, Int_t status)
{
    for (TString branchName : branchNames) {
        tree->SetBranchStatus(branchName, status);
    }
}