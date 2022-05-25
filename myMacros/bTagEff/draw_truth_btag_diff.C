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
    std::string fname = "/home/llr/cms/kalipoliti/rootFiles/aggregateB_ip3dSig_looserCut_ref.root";
    //std::string fname_noGSP = "/home/llr/cms/kalipoliti/rootFiles/aggregateB_ip3dSig_looserCut_noGSP_ref.root";

    TCanvas *c = new TCanvas("c", "", 1800, 800);
    c->Divide(2, 1);

    TLegend *leg = new TLegend(0.6, 0.7, 0.8, 0.85);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    gStyle->SetLegendTextSize(15);

    THStack *hs = new THStack("hs", "");

    Float_t ptrange[2] = {400., 500.};
    TPaveText *info = new TPaveText(0.6, 0.6, 0.8, 0.7, "ndc");
    info->AddText(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptrange[0], ptrange[1]));
	info->AddText("Aggregated pseudo-B's");
    info->SetTextSize(15);
    info->SetFillStyle(0);
    info->SetBorderSize(0);

    TH1D *hB_rg = do_rg_projection(fname, "hB_rgkt", ptrange);
    hB_rg->SetLineColor(1);
    hB_rg->SetLineStyle(1);
    leg->AddEntry(hB_rg, "truth level, true b-jets", "l");
    hs->Add(hB_rg);

    TH1D *hB_rg_dynKt = do_rg_projection(fname, "hB_rgkt_dynKt", ptrange);
    hB_rg_dynKt->SetLineColor(1);
    hB_rg_dynKt->SetLineStyle(2);
    //leg->AddEntry(hB_rg_dynKt, "truth, true b-jets, dynKt", "l");
    //hs->Add(hB_rg_dynKt);

    TH1D *hBtag_rg = do_rg_projection(fname, "hBtag_rgkt", ptrange);
    hBtag_rg->SetLineColor(2);
    hBtag_rg->SetLineStyle(1);
    leg->AddEntry(hBtag_rg, "truth level, tagged b-jets", "l");
    hs->Add(hBtag_rg);

    TH1D *hBtag_rg_dynKt = do_rg_projection(fname, "hBtag_rgkt_dynKt", ptrange);
    hBtag_rg_dynKt->SetLineColor(2);
    hBtag_rg_dynKt->SetLineStyle(2);
    //leg->AddEntry(hBtag_rg_dynKt, "truth, tagged b-jets, dynKt", "l");
    //hs->Add(hBtag_rg_dynKt);

	std::string x1title = "ln(1/R_{g})";
	std::string y1title = "1/N_{2-prong jets} dN/dln(1/R_{g})";
    std::string y2title = "N_{true b-jets} / N_{tagged b-jets}";

	hs->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));

    // Create ratio histogram
    TH1D *h_ratio = (TH1D *) hB_rg->Clone();
    h_ratio->Divide(hBtag_rg);
    h_ratio->SetTitle(Form("; %s; %s", x1title.c_str(), y2title.c_str()));
	TPaveText *info_ratio = (TPaveText *) info->Clone();
	info_ratio->AddText("truth level");

    c->cd(1);
    hs->Draw("nostack hist");
    leg->Draw();
    info->Draw();

    c->cd(2);
    h_ratio->Draw("hist");
	info_ratio->Draw();

    c->Draw();

	std::string savename = "true_tagged_b_diff.png";
	c->Print(savename.c_str(), "png");
}
