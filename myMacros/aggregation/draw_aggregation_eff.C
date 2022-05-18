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

    TGraph *gr_sel1 = new TGraph();
    gr_sel1->SetPoint(gr_sel1.getN(), heff_sel1->GetBinContent(1, 1), heff_sel1->GetBinContent(2, 1));
    gr_sel1->SetMarkerColor(2);
    gr_sel1->SetMarkerStyle(kFullCircle);
    mg->Add(gr_sel1);
    leg->Add(gr_sel1, sel1.c_str(), "p");

    TGraph *gr_sel2 = new TGraph();
    gr_sel2->SetPoint(gr_sel2.getN(), heff_sel2->GetBinContent(1, 1), heff_sel2->GetBinContent(2, 1));
    gr_sel2->SetMarkerColor(4);
    gr_sel2->SetMarkerStyle(kFullTriangleUp);
    mg->Add(gr_sel2);
    leg->Add(gr_sel2, sel2.c_str(), "p");

    mg->GetXaxis()->SetTitle(x1title);
    mg->GetYaxis()->SetTitle(y1title);

    mg->Draw("pa");
}