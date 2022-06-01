#include "TFile.h"
#include "TTree.h"

#include <vector>

class hiTreeAnalyzer {
    public :
        TFile *fin;
        TTree *hi;

        Long64_t nentries;

        // Declaration of leaf types
        Int_t           mult;
        std::vector<float>   *pt = 0;
        std::vector<float>   *eta = 0;
        std::vector<float>   *phi = 0;
        std::vector<int>     *pdg = 0;
        std::vector<int>     *sta = 0;

        hiTreeAnalyzer() {};
        hiTreeAnalyzer(std::string fname, bool init = true);

        void Init();
        void GetEntry(Long64_t ient);
        void SetBranchStatus(std::string branchName, Int_t status);
    	void SetBranchStatus(std::vector<std::string> branchNames, Int_t status);
};

hiTreeAnalyzer::hiTreeAnalyzer(std::string fname, bool init = true) 
{
   fin = new TFile(fname.c_str());
   
   if (init) {
      Init();
   }
}

void hiTreeAnalyzer::Init()
{
   hi->SetBranchAddress("mult", &mult);
   hi->SetBranchAddress("pt", &pt);
   hi->SetBranchAddress("eta", &eta);
   hi->SetBranchAddress("phi", &phi);
   hi->SetBranchAddress("pdg", &pdg);
   hi->SetBranchAddress("sta", &sta);

   nentries = hi->GetEntries();
}

void hiTreeAnalyzer::GetEntry(Long64_t ient)
{
    hi->GetEntry(ient);
}

void hiTreeAnalyzer::SetBranchStatus(std::string branchName, Int_t status)
{
    hi->SetBranchStatus(branchName.c_str(), status);
}

void hiTreeAnalyzer::SetBranchStatus(std::vector<std::string> branchNames, Int_t status)
{
    for (std::string branchName : branchNames) {
        hi->SetBranchStatus(branchName.c_str(), status);
    }
}

