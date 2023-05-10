#include "tTree.h"

void plot_rgzgkt() 
{
    // TString suffix = "_noAggr";
    // TString suffix = "_truth";
    // TString suffix = "_bdt44_sig_vs_bkg";
    // TString fin = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/HiForestMiniAOD_HighPU_10000events_conmatch" + suffix + ".root";
    // TString fin = "/home/llr/cms/kalipoliti/C10630p1_miniAOD/src/HeavyIonsAnalysis/Configuration/test/rootf/HiForestMiniAOD_HighPU_10000events_conmatch" + suffix + ".root";
    TString label = "aggrTMVA_withNb";
    // TString indir = "/data_CMS/cms/kalipoliti/ttbarMC/highPU/" + label + "/";
    TString indir = "/data_CMS/cms/kalipoliti/qcdMC/bjet/" + label + "/";
    TString fin = indir + "merged_HiForestMiniAOD.root";
    tTree t(fin);

    // TString fout = "./histos/ttbar_highPU_" + label + "_rgzgkt.root";
    TString fout = "./histos/qcd_bjet_" + label + "_rgzgkt.root";
    t.plot_rgzgkt(fout);
}
