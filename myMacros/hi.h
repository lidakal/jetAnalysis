/* hi tree */

#include <TFile.h>
#include <TTree.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class hi {
public :
   TTree *tree;

   // Declaration of leaf types
   Int_t           mult;
   std::vector<float>   *pt = 0;
   std::vector<float>   *eta = 0;
   std::vector<float>   *phi = 0;
   std::vector<int>     *pdg = 0;
   std::vector<int>     *sta = 0;

   hi();
   hi(std::string fname, bool init = true);

   void Init(TTree *tree);
};

hi::hi()
{
   // default constructor
}

hi::hi(std::string fname, bool init = true) 
{
   TFile *f = new TFile(fname.c_str());
   
   if (init) {
      Init(tree);
   }
}

void hi::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
   
   // Set object pointer
   pt = 0;
   eta = 0;
   phi = 0;
   pdg = 0;
   sta = 0;
   // Set branch addresses and branch pointers

   tree->SetBranchAddress("mult", &mult);
   tree->SetBranchAddress("pt", &pt);
   tree->SetBranchAddress("eta", &eta);
   tree->SetBranchAddress("phi", &phi);
   tree->SetBranchAddress("pdg", &pdg);
   tree->SetBranchAddress("sta", &sta);
}

