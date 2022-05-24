#include "TFile.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TCanvas.h"

#include "utils.h" // make_graph()

void draw_aggregation_eff() 
{
    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "aggregation_eff.root";

	TCanvas *c = new TCanvas("c", "", 800, 800);

    TMultiGraph *mg = new TMultiGraph();
    TLegend *leg = new TLegend(0.2, 0.7, 0.5, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
	gStyle->SetLegendTextSize(15);

    std::string label_sel1 = "in SV";
    std::vector<Int_t> sel1 = {1};
    TGraph *gr_sel1 = make_graph(indir + fname, sel1, kFullCircle, kRed, leg, label_sel1);
    mg->Add(gr_sel1);

    std::string label_sel6 = "|ip3dSig| > (12, 9, 6, 3)";
    std::vector<Int_t> sel6 = {6, 7, 8, 9};
    TGraph *gr_sel6 = make_graph(indir + fname, sel6, kFullDiamond, kGreen, leg, label_sel6);
    mg->Add(gr_sel6);
    
	std::string label_sel2 = "in SV || |ip3dSig| > (12, 9, 6, 3)";
    std::vector<Int_t> sel2 = {2, 3, 4, 5};
    TGraph *gr_sel2 = make_graph(indir + fname, sel2, kFullTriangleUp, kBlue, leg, label_sel2);
    mg->Add(gr_sel2);

    std::string label_sel10 = "inSV && |ip3dSig| > (12, 9, 6, 3)";
    std::vector<Int_t> sel10 = {10, 11, 12, 13};
    TGraph *gr_sel10 = make_graph(indir + fname, sel10, kFullTriangleDown, kViolet, leg, label_sel10);
    mg->Add(gr_sel10);

	std::string label_sel14 = "(inSV && |ip3dSig| > 3) || (!inSV && |ip3dSig| > (9, 6, 3))";
	std::vector<Int_t> sel14 = {14, 15, 16};
	TGraph *gr_sel14 = make_graph(indir + fname, sel14, kOpenCircle, kOrange, leg, label_sel14);
	mg->Add(gr_sel14);

    std::string x1title = "pass selection | From B decay";
    std::string y1title = "pass selection | Not from B decay";
	mg->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));
    
    mg->Draw("pla");
	leg->Draw();

	c->Draw();

	std::string savename = "aggregation_eff_ROC.png";
	c->Print(savename.c_str(), "png");
}
