#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "THStack.h"

#include "../utils.h" // normalise_histo, set_axes_labels

#include <algorithm> // std::max

void draw_svtx_BvsL(std::string variable = "svtxdls") 
{
    // variable can be "svtxdls", "svtxm", "svtxmcorr"
    std::string hsig_name = "hB_" + variable;
    std::string hbkg_name = "hL_" + variable;

    std::string indir = "/home/llr/cms/kalipoliti/rootFiles/";
	std::string fname = "svtx_histos_BvsL.root";

    TFile *fin = new TFile((indir + fname).c_str());
    TH2D *hsig = (TH2D *) fin->Get(hsig_name.c_str());
    TH2D *hbkg = (TH2D *) fin->Get(hbkg_name.c_str());

    // Copy histograms to make stack plot
    TH1D *hsig_stack = (TH1D *) hsig->Clone();
    TH1D *hbkg_stack = (TH1D *) hbkg->Clone();

    // Create sum histogram to normalise stack histograms
    TH1D *h_stack = (TH1D *) hsig->Clone();
    h_stack->Add(hbkg_stack);
    h_stack->GetXaxis()->SetRange(0, h_stack->GetNbinsX() + 1);
    Float_t norm = h_stack->Integral();

	std::string x1title = variable;
    std::string y1title = "1/N_{total SVs in B (L) jets} dN_{SVs in B (L) jets}/" + variable;
    std::string y2title = "1/N_{total SVs} dN_{SVs}/" + variable;
    
    // Modify the normal histograms
    hsig->GetXaxis()->SetRange(1, hsig->GetNbinsX() + 1);
    hsig->Scale(1 / hsig->Integral("width"));
    hsig->SetLineColor(4);
    set_axes_labels(hsig, x1title, y1title);

    hbkg->GetXaxis()->SetRange(1, hbkg->GetNbinsX() + 1);
    hbkg->Scale(1 / hbkg->Integral("width"));
    hbkg->SetLineColor(2);
    set_axes_labels(hbkg, x1title, y1title);

    // Modify the stack histograms
    THStack *hs = new THStack("hs", "");

    hbkg_stack->Scale(1 / norm);
    hbkg_stack->SetFillColor(2);
    hbkg_stack->SetFillStyle(1001);
    hs->Add(hbkg_stack);

    hsig_stack->Scale(1 / norm);
    hsig_stack->SetFillColor(4);
    hsig_stack->SetFillStyle(1001);
    hs->Add(hsig_stack);

	hs->SetTitle(Form("; %s; %s", x1title.c_str(), y2title.c_str()));
   
    // Create info box and legends
    TPaveText *info = new TPaveText(0.2, 0.75, 0.35, 0.85, "ndc");
	info->SetFillColor(0);
	info->SetBorderSize(1);
	info->SetTextSize(15);
    info->AddText("track p_{T} > 1 GeV");

    TLegend *leg = new TLegend(0.55, 0.75, 0.8, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
	gStyle->SetLegendTextSize(15);

    TLegend *leg = (TLegend *) leg->Clone();
    TLegend *leg_stack = (TLegend *) leg->Clone();
    
    leg->AddEntry(hsig, "SVs from B jets", "l");
    leg->AddEntry(hbkg, "SVs from L jets", "l");

    leg_stack->AddEntry(hsig_stack, "SVs from B jets", "l");
    leg_stack->AddEntry(hbkg_stack, "SVs from L jets", "l");

    TCanvas *c = new TCanvas("c", "", 1800, 800);
    c->Divide(2, 1);

    c->cd(1);
    hsig->Draw("hist");
    hbkg->Draw("hist same");
    info->Draw();
    leg->Draw();
    
    c->cd(2);
    hs->Draw("hist");
    info->Draw();
    leg_stack->Draw();

    c->Draw();

    std::string savename = variable + "_BvsL.png";
	c->Print(savename.c_str(), "png");
}
