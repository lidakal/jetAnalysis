#include "TFile.h"
#include "TH2D.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegendEntry.h"

#include "utils.h" // make_graph_eff_mistag(), make_graph_eff_pur()
#include "../pt2dscan/myPalette.h"

void draw_aggregation_eff() 
{
    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "aggregation_eff.root";


    // ------------------- efficiency VS mistag rate ------------------------

	TCanvas *c = new TCanvas("c", "", 1200, 800);
    
    //c->Divide(2,1);

    TMultiGraph *mg = new TMultiGraph();
    TLegend *leg = new TLegend(0.2, 0.7, 0.5, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
	gStyle->SetLegendTextSize(15);
    leg->SetHeader("Aggregation selection criteria:");

    std::string label_sel1 = "inSV";
    std::vector<Int_t> sel1 = {1};
    TGraph *gr_sel1 = make_graph_eff_mistag(indir + fname, sel1, kFullCircle, mykGreenLight, leg, label_sel1);
    mg->Add(gr_sel1);

    std::string label_sel6 = "|3D IP/#sigma| > (12, 9, 6, 3)";
    std::vector<Int_t> sel6 = {6, 7, 8, 9};
    TGraph *gr_sel6 = make_graph_eff_mistag(indir + fname, sel6, kFullDiamond, mykMagentaLight, leg, label_sel6);
    gr_sel6->SetMarkerSize(2);
    mg->Add(gr_sel6);
    
	std::string label_sel2 = "inSV OR |3D IP/#sigma| > (12, 9, 6, 3)";
    std::vector<Int_t> sel2 = {2, 3, 4, 5};
    TGraph *gr_sel2 = make_graph_eff_mistag(indir + fname, sel2, kFullTriangleUp, mykOrangeLight, leg, label_sel2);
    mg->Add(gr_sel2);

    std::string label_sel10 = "inSV AND |3D IP/#sigma| > (12, 9, 6, 3)";
    std::vector<Int_t> sel10 = {10, 11, 12, 13};
    TGraph *gr_sel10 = make_graph_eff_mistag(indir + fname, sel10, kFullTriangleDown, mykSpring, leg, label_sel10);
    mg->Add(gr_sel10);

	std::string label_sel14 = "(inSV AND |3D IP/#sigma| > 3) OR |3D IP/#sigma| > (9, 6, 3)";
	std::vector<Int_t> sel14 = {14, 15, 16};
	TGraph *gr_sel14 = make_graph_eff_mistag(indir + fname, sel14, kFullSquare, mykCyanLight, leg, label_sel14);
	mg->Add(gr_sel14);

    std::string x1title = "P(pass selection | From B decay)";
    std::string y1title = "P(pass selection | Not from B decay)";
	mg->SetTitle(Form("; %s; %s", x1title.c_str(), y1title.c_str()));
    
    //c->cd(1);
    //mg->Draw("pla");
	//leg->Draw();
    

    //-------------------------- efficiency VS purity ---------------------

    TMultiGraph *mg_pur = new TMultiGraph();
    TLegend *leg_pur = new TLegend(0.17, 0.15, 0.46, 0.4);
    leg_pur->SetBorderSize(1);
    leg_pur->SetFillStyle(1000);
	gStyle->SetLegendTextSize(15);
    leg_pur->SetHeader("From-B-decay track tagging criteria:");
    leg_pur->SetMargin(0.1);

    TLegendEntry *header = (TLegendEntry*) leg_pur->GetListOfPrimitives()->First();
    header->SetTextAlign(22);
    //header->SetTextColor(2);
    header->SetTextSize(17);

    TGraph *gr_pur_sel1 = make_graph_eff_pur(indir + fname, sel1, kFullCircle, mykGreenLight, leg_pur, label_sel1);
    mg_pur->Add(gr_pur_sel1);
    TGraph *gr_pur_sel1mark = make_graph_eff_pur(indir + fname, sel1, kOpenCircle, mykGreenLight, leg_pur, "");
    gr_pur_sel1mark->SetMarkerSize(4);
    mg_pur->Add(gr_pur_sel1mark);

    TGraph *gr_pur_sel6 = make_graph_eff_pur(indir + fname, sel6, kFullTriangleDown, mykMagentaLight, leg_pur, label_sel6);
    mg_pur->Add(gr_pur_sel6);
    TGraph *gr_pur_sel6mark = make_graph_eff_pur(indir + fname, {6}, kOpenCircle, mykMagentaLight, leg_pur, "");
    gr_pur_sel6mark->SetMarkerSize(4);
    mg_pur->Add(gr_pur_sel6mark);
    
	TGraph *gr_pur_sel2 = make_graph_eff_pur(indir + fname, sel2, kFullTriangleUp, mykOrangeLight, leg_pur, label_sel2);
    mg_pur->Add(gr_pur_sel2);
    TGraph *gr_pur_sel2mark = make_graph_eff_pur(indir + fname, {2}, kOpenCircle, mykOrangeLight, leg_pur, "");
    gr_pur_sel2mark->SetMarkerSize(4);
    mg_pur->Add(gr_pur_sel2mark);

    //TGraph *gr_pur_sel10 = make_graph_eff_pur(indir + fname, sel10, kFullTriangleDown, mykSpring, leg_pur, label_sel10);
    //mg_pur->Add(gr_pur_sel10);

	TGraph *gr_pur_sel14 = make_graph_eff_pur(indir + fname, sel14, kFullSquare, mykCyanLight, leg_pur, label_sel14);
	mg_pur->Add(gr_pur_sel14);
    TGraph *gr_pur_sel14mark = make_graph_eff_pur(indir + fname, {16}, kOpenCircle, mykCyanLight, leg_pur, "");
    gr_pur_sel14mark->SetMarkerSize(4);
	mg_pur->Add(gr_pur_sel14mark);

    std::string x1title_pur = "Track tagging efficiency";
    std::string y1title_pur = "Track tagging purity";
	mg_pur->SetTitle(Form("; %s; %s", x1title_pur.c_str(), y1title_pur.c_str()));
    
    //c->cd(2);
    c->SetGrid(1);
    mg_pur->Draw("pla");
	leg_pur->Draw();

    TPaveText *mcinfo = new TPaveText(0.15, 0.9, 0.4, 0.95, "ndc");
    mcinfo->SetBorderSize(0);
    mcinfo->SetFillColor(0);
    mcinfo->SetFillStyle(0);
    mcinfo->SetTextSize(20);
    mcinfo->AddText("#it{#sqrt{s}} = 5.02 TeV pp MC (PYTHIA8)");
    mcinfo->Draw();

	c->Draw();

	std::string savename = "aggregation_eff_ROC.png";
	c->Print(savename.c_str(), "png");


}
