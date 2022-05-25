#include "TFile.h"
#include "TH1D.h"
#include "TStyle.h"
#include "THStack.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "utils.h" // do_rg_projection()

void draw_truth_btag_diff()
{
    
    //std::string fname_noGSP = "/home/llr/cms/kalipoliti/rootFiles/aggregateB_ip3dSig_looserCut_noGSP_ref.root";

    TCanvas *c = new TCanvas("c", "", 1800, 800);
    c->Divide(2, 1);

    Float_t ptrange1[2] = {100., 150.};
    draw_rg_projection(c, ptrange1);

    Float_t ptrange2[2] = {200., 250.};
    draw_rg_projection(c, ptrange2);

    Float_t ptrange3[2] = {300., 350.};
    draw_rg_projection(c, ptrange3);

    TPaveText *info = new TPaveText(0.2, 0.3, 0.4, 0.45, "ndc");
	info->AddText("Aggregated pseudo-B's");
    info->SetTextSize(15);
    info->SetFillStyle(0);
    info->SetBorderSize(0);

    c->cd(1);
    info->Draw();

    c->cd(2);
	info_ratio->Draw();

    c->Draw();

	std::string savename = "true_tagged_b_diff.png";
	c->Print(savename.c_str(), "png");
}
