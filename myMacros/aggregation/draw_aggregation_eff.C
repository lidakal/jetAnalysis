#include "TFile.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TMultiGraph.h"

void draw_aggregation_eff() 
{
    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "aggregation_eff.root";

    TFile *fin = new TFile((indir + fname).c_str());
    TH1D *heff_sel1 = (TH1D *) fin->Get("heff_sel1");
    std::string sel1 = "in SV";
    TH1D *heff_sel2 = (TH1D *) fin->Get("heff_sel2");
    std::string sel2 = "in SV || ip3dSig > 9";

    std::string x1title = "pass selection | From B";
    std::string y1title = "pass selection | Not from B";

    TMultiGraph *mg = new TMultiGraph();
    TLegend *leg = new TLegend(0.2, 0.7, 0.5, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
	gStyle->SetLegendTextSize(15);

	Float_t totalFromB_sel1 = heff_sel1->GetBinContent(1, 1) + heff_sel1->GetBinContent(1, 2);
	Float_t passFromB_sel1 = heff_sel1->GetBinContent(1, 1);
	Float_t xSel1 = passFromB_sel1 / totalFromB_sel1;

	Float_t totalNotFromB_sel1 = heff_sel1->GetBinContent(2, 1) + heff_sel1->GetBinContent(2, 2);
	Float_t passNotFromB_sel1 = heff_sel1->GetBinContent(2, 1);
	Float_t ySel1 = passNotFromB_sel1 / totalNotFromB_sel1;

    TGraph *gr_sel1 = new TGraph(1, &xSel1, &ySel1);
    gr_sel1->SetMarkerColor(2);
    gr_sel1->SetMarkerStyle(kFullCircle);
    mg->Add(gr_sel1);
    leg->AddEntry(gr_sel1, sel1.c_str(), "p");

	Float_t totalFromB_sel2 = heff_sel2->GetBinContent(1, 1) + heff_sel2->GetBinContent(1, 2);
	Float_t passFromB_sel2 = heff_sel2->GetBinContent(1, 1);
	Float_t xSel2 = passFromB_sel2 / totalFromB_sel2;

	Float_t totalNotFromB_sel2 = heff_sel2->GetBinContent(2, 1) + heff_sel2->GetBinContent(2, 2);
	Float_t passNotFromB_sel2 = heff_sel2->GetBinContent(2, 1);
	Float_t ySel2 = passNotFromB_sel2 / totalNotFromB_sel2;

    TGraph *gr_sel2 = new TGraph(1, &xSel2, &ySel2);
    gr_sel2->SetMarkerColor(4);
    gr_sel2->SetMarkerStyle(kFullTriangleUp);
    mg->Add(gr_sel2);
    leg->AddEntry(gr_sel2, sel2.c_str(), "p");

	mg->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));
    //mg->GetXaxis()->SetTitle(x1title.c_str());
    //mg->GetYaxis()->SetTitle(y1title.c_str());

    mg->Draw("pa");
	leg->Draw();
}
