#include "tTree.h"

void plot_rgzgkt() 
{
    // TString suffix = "_noAggr";
    // TString suffix = "_truth";
    TString suffix = "_bdt29";
    TString fin = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/HiForestMiniAOD_HighPU_10000events_conmatch" + suffix + ".root";
    tTree t(fin);

    TString fout = "./histos/rgzgkt" + suffix + ".root";
    t.plot_rgzgkt(fout);
}
