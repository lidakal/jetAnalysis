#include "TreeAnalyzer_sjdiff.h"

void plot_gen_reco_sjdiff() 
{
    std::string finname = "/data_CMS/cms/kalipoliti/bJetMC/aggregateB_ip3dSig_looserCut_fixedBugs/merged_HiForestAOD.root";
    std::string foutname = "sjdiff";

    TreeAnalyzer_sjdiff TA(finname, true);
    TA.plot_sjdiff(foutname, true); // sjdiff with GSP
    TA.plot_sjdiff(foutname, false); // sjdiff with no GSP
}
