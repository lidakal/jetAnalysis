#include <TTree.h>
#include <TFile.h>
#include <TString.h>

#include "hiTree.h"
#include "tTree.h"

void track_gen_association()
{
    TString fdir = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/";
    TString label = "HiForestMiniAOD_HighPU_1000events";
    TString fname = label + ".root";
    TString rootf = fdir + fname;

    tTree t = tTree(rootf);
    hiTree hi = hiTree(rootf);

    Long64_t nentries = t.GetEntries();
    for (Long64_t ient = 0; ient < nentries; ient++) {
        // Print progress
        if (ient % 1000 == 0) {
            std::cout << "ient = " << ient << std::endl;
        }

        t->GetEntry(ient);
        hi->GetEntry(ient);

        for (int itrk = 0; itrk < t.ntrk; itrk++) {
            std::cout << t.trkPt[itrk] << std::endl;
        } 

        
    }
    

    // std::cout << hi->mult << std::endl;
}

