#include "TFile.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TCanvas.h"

#include "utils.h" // make_graph_eff_mistag(), make_graph_eff_pur()

void draw_aggregation_eff() 
{
    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "aggregation_eff.root";


    // ------------------- efficiency VS mistag rate ------------------------

	TCanvas *c = new TCanvas("c", "", 1800, 800);
    c->Divide(2,1);

    TMultiGraph *mg = new TMultiGraph();
    TLegend *leg = new TLegend(0.2, 0.7, 0.5, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
	gStyle->SetLegendTextSize(15);

    std::string label_sel1 = "in SV";
    std::vector<Int_t> sel1 = {1};
    TGraph *gr_sel1 = make_graph_eff_mistag(indir + fname, sel1, kFullCircle, kRed, leg, label_sel1);
    mg->Add(gr_sel1);

    std::string label_sel6 = "|ip3dSig| > (12, 9, 6, 3)";
    std::vector<Int_t> sel6 = {6, 7, 8, 9};
    TGraph *gr_sel6 = make_graph_eff_mistag(indir + fname, sel6, kFullDiamond, kGreen, leg, label_sel6);
    mg->Add(gr_sel6);
    
	std::string label_sel2 = "in SV || |ip3dSig| > (12, 9, 6, 3)";
    std::vector<Int_t> sel2 = {2, 3, 4, 5};
    TGraph *gr_sel2 = make_graph_eff_mistag(indir + fname, sel2, kFullTriangleUp, kBlue, leg, label_sel2);
    mg->Add(gr_sel2);

    std::string label_sel10 = "inSV && |ip3dSig| > (12, 9, 6, 3)";
    std::vector<Int_t> sel10 = {10, 11, 12, 13};
    TGraph *gr_sel10 = make_graph_eff_mistag(indir + fname, sel10, kFullTriangleDown, kViolet, leg, label_sel10);
    mg->Add(gr_sel10);

	std::string label_sel14 = "(inSV && |ip3dSig| > 3) || (!inSV && |ip3dSig| > (9, 6, 3))";
	std::vector<Int_t> sel14 = {14, 15, 16};
	TGraph *gr_sel14 = make_graph_eff_mistag(indir + fname, sel14, kOpenCircle, kOrange, leg, label_sel14);
	mg->Add(gr_sel14);

    std::string x1title = "pass selection | From B decay";
    std::string y1title = "pass selection | Not from B decay";
	mg->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));
    
    c->cd(1);
    mg->Draw("pla");
	leg->Draw();


    //-------------------------- efficiency VS purity ---------------------

    TMultiGraph *mg_pur = new TMultiGraph();
    TLegend *leg_pur = new TLegend(0.2, 0.7, 0.5, 0.85);
    leg_pur->SetBorderSize(0);
    leg_pur->SetFillStyle(0);
	gStyle->SetLegendTextSize(15);

    TGraph *gr_pur_sel1 = make_graph_eff_pur(indir + fname, sel1, kFullCircle, kRed, leg_pur, label_sel1);
    mg_pur->Add(gr_pur_sel1);

    TGraph *gr_pur_sel6 = make_graph_eff_pur(indir + fname, sel6, kFullDiamond, kGreen, leg_pur, label_sel6);
    mg_pur->Add(gr_pur_sel6);
    
	TGraph *gr_pur_sel2 = make_graph_eff_pur(indir + fname, sel2, kFullTriangleUp, kBlue, leg_pur, label_sel2);
    mg_pur->Add(gr_pur_sel2);

    TGraph *gr_pur_sel10 = make_graph_eff_pur(indir + fname, sel10, kFullTriangleDown, kViolet, leg_pur, label_sel10);
    mg_pur->Add(gr_pur_sel10);

	TGraph *gr_pur_sel14 = make_graph_eff_pur(indir + fname, sel14, kOpenCircle, kOrange, leg_pur, label_sel14);
	mg_pur->Add(gr_pur_sel14);

    std::string x1title_pur = "pass selection | From B decay";
    std::string y1title_pur = "From B decay | pass selection";
	mg_pur->SetTitle(Form("; %s; %s", x1title_pur.c_str(), y1title_pur.c_str()));
    
    c->cd(2);
    mg_pur->Draw("pla");
	leg_pur->Draw();

	c->Draw();

	std::string savename = "aggregation_eff_ROC.png";
	c->Print(savename.c_str(), "png");


}
