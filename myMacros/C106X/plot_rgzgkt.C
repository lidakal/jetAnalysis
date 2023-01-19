#include "tTree.h"

void plot_rgzgkt() 
{
    TString fin = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/HiForestMiniAOD_HighPU_10000events_conmatch_truth.root";
    tTree t(fin);

    TString fout = "./histos/rgzgkt_truth.root";
    t.plot_rgzgkt(fout);
}
