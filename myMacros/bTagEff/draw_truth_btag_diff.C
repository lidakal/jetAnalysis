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

	THStack *hs = new THStack("hs", "");
	THStack *hs_ratio = new THStack("hs_ratio", "");

	std::string x1title = "ln(1/R_{g})";
	std::string y1title = "1/N_{2-prong jets} dN/dln(1/R_{g})";
	std::string y2title = "N_{true b-jets} / N_{tagged b-jets}";

	hs->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));
	hs_ratio->SetTitle(Form("; %s; %s", x1title.c_str(), y2title.c_str()));

	TLegend *leg = new TLegend(0.4, 0.6, 0.85, 0.85);
    leg->SetFillStyle(0);
    gStyle->SetLegendTextSize(15);

    TLegend *leg_ratio = new TLegend(0.5, 0.4, 0.8, 0.6);
	leg_ratio->SetFillStyle(0);
	leg_ratio->SetBorderSize(0);

    Float_t ptrange1[2] = {100., 150.};
    draw_rg_projection(c, hs, leg, hs_ratio, leg_ratio, ptrange1);

    Float_t ptrange2[2] = {200., 250.};
    draw_rg_projection(c, hs, leg, hs_ratio, leg_ratio, ptrange2);

    Float_t ptrange3[2] = {300., 350.};
    draw_rg_projection(c, hs, leg, hs_ratio, leg_ratio, ptrange3);

    TPaveText *info = new TPaveText(0.2, 0.25, 0.5, 0.4, "ndc");
	info->AddText("truth level, aggregated pseudo-B's");
    info->SetTextSize(15);
    info->SetFillStyle(0);
    info->SetBorderSize(0);

    c->cd(1);
	hs->Draw("nostack hist");
	leg->Draw();
    info->Draw();

    c->cd(2);
	hs_ratio->Draw("nostack hist");
	leg_ratio->Draw();
	info->Draw();

    c->Draw();

	std::string savename = "true_tagged_b_diff.png";
	c->Print(savename.c_str(), "png");
}
