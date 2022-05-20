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
    TH1D *heff_sel3 = (TH1D *) fin->Get("heff_sel3");
    TH1D *heff_sel4 = (TH1D *) fin->Get("heff_sel4");
    TH1D *heff_sel5 = (TH1D *) fin->Get("heff_sel5");
	std::string sel2 = "in SV || ip3dSig > (12, 9, 6, 3)";

    std::string x1title = "pass selection | From B decay";
    std::string y1title = "pass selection | Not from B decay";

    TMultiGraph *mg = new TMultiGraph();
    TLegend *leg = new TLegend(0.2, 0.7, 0.5, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
	gStyle->SetLegendTextSize(15);

    // These should be the same for all selections
	Float_t totalFromB = heff_sel1->GetBinContent(1, 1) + heff_sel1->GetBinContent(1, 2);
    Float_t totalNotFromB = heff_sel1->GetBinContent(2, 1) + heff_sel1->GetBinContent(2, 2);

    // Sel1
	Float_t passFromB_sel1 = heff_sel1->GetBinContent(1, 1);
    Float_t passNotFromB_sel1 = heff_sel1->GetBinContent(2, 1);
	Float_t xSel1 = passFromB_sel1 / totalFromB;
	Float_t ySel1 = passNotFromB_sel1 / totalNotFromB;

    TGraph *gr_sel1 = new TGraph(1, &xSel1, &ySel1);
    gr_sel1->SetMarkerColor(2);
    gr_sel1->SetMarkerStyle(kFullCircle);
    mg->Add(gr_sel1);
    leg->AddEntry(gr_sel1, sel1.c_str(), "p");

    // Sel2 + 3 + 4 + 5
    const Int_t nSel2 = 4;
    Float_t xSel2[nSel2];
    Float_t ySel2[nSel2];

    Float_t passFromB_sel2 = heff_sel2->GetBinContent(1, 1);
    Float_t passNotFromB_sel2 = heff_sel2->GetBinContent(2, 1);
	xSel2[0] = passFromB_sel2 / totalFromB;
	ySel2[0] = passNotFromB_sel2 / totalNotFromB;

    Float_t passFromB_sel3 = heff_sel3->GetBinContent(1, 1);
    Float_t passNotFromB_sel3 = heff_sel3->GetBinContent(2, 1);
	xSel2[1] = passFromB_sel3 / totalFromB;
	ySel2[1] = passNotFromB_sel3 / totalNotFromB;

    Float_t passFromB_sel4 = heff_sel4->GetBinContent(1, 1);
    Float_t passNotFromB_sel4 = heff_sel4->GetBinContent(2, 1);
	xSel2[2] = passFromB_sel4 / totalFromB;
	ySel2[2] = passNotFromB_sel4 / totalNotFromB;

    Float_t passFromB_sel5 = heff_sel5->GetBinContent(1, 1);
    Float_t passNotFromB_sel5 = heff_sel5->GetBinContent(2, 1);
	xSel2[3] = passFromB_sel5 / totalFromB;
	ySel2[3] = passNotFromB_sel5 / totalNotFromB;

    TGraph *gr_sel2 = new TGraph(nSel2, xSel2, ySel2);
    gr_sel2->SetMarkerColor(4);
    gr_sel2->SetMarkerStyle(kFullTriangleUp);
	gr_sel2->SetLineColor(4);
	gr_sel2->SetLineStyle(2);
    mg->Add(gr_sel2);
    leg->AddEntry(gr_sel2, sel2.c_str(), "pl");

	mg->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));
    //mg->GetXaxis()->SetTitle(x1title.c_str());
    //mg->GetYaxis()->SetTitle(y1title.c_str());

    mg->Draw("pla");
	leg->Draw();
}
