#include <TFile.h>
#include <TH3.h>
#include <TH1.h>
#include <THStack.h>
#include <TString.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPaveText.h>

// #include "myPalette.h"
#include "hist_utils.h"

void draw_mB()
{
    // ---- Settings
    float ptMin = 50.;
    float ptMax = 80.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Load the histos
    TString indir = "./histos/";
    TString label_aggrTMVA = "aggrTMVA_withNb";
    TString fname_aggrTMVA = indir + "qcd_dijet_" + label_aggrTMVA + "_mB.root";

    std::cout << "Grabbing histos from " << fname_aggrTMVA << std::endl;
    TFile *fin_aggrTMVA = new TFile(fname_aggrTMVA);

    TH3D *hB_mB = (TH3D *) fin_aggrTMVA->Get("hB_mB");
    TH3D *hB_mB_gen = (TH3D *) fin_aggrTMVA->Get("hB_mB_gen");

    TH3D *hL_mB = (TH3D *) fin_aggrTMVA->Get("hL_mB");
    TH3D *hL_mB_gen = (TH3D *) fin_aggrTMVA->Get("hL_mB_gen");

    TH3D *hC_mB = (TH3D *) fin_aggrTMVA->Get("hC_mB");
    TH3D *hC_mB_gen = (TH3D *) fin_aggrTMVA->Get("hC_mB_gen");

    TH3D *hSingleB_mB = (TH3D *) fin_aggrTMVA->Get("hSingleB_mB");
    TH3D *hSingleB_mB_gen = (TH3D *) fin_aggrTMVA->Get("hSingleB_mB_gen");

    TH3D *hBB_mB = (TH3D *) fin_aggrTMVA->Get("hBB_mB");
    TH3D *hBB_mB_gen = (TH3D *) fin_aggrTMVA->Get("hBB_mB_gen");

    TH3D *hBtag_mB = (TH3D *) fin_aggrTMVA->Get("hBtag_mB");
    TH3D *hBtag_mB_gen = (TH3D *) fin_aggrTMVA->Get("hBtag_mB_gen");

    TH3D *hLtag_mB = (TH3D *) fin_aggrTMVA->Get("hLtag_mB");
    TH3D *hLtag_mB_gen = (TH3D *) fin_aggrTMVA->Get("hLtag_mB_gen");

    TH3D *hCtag_mB = (TH3D *) fin_aggrTMVA->Get("hCtag_mB");
    TH3D *hCtag_mB_gen = (TH3D *) fin_aggrTMVA->Get("hCtag_mB_gen");

    TH3D *hSingleBtag_mB = (TH3D *) fin_aggrTMVA->Get("hSingleBtag_mB");
    TH3D *hSingleBtag_mB_gen = (TH3D *) fin_aggrTMVA->Get("hSingleBtag_mB_gen");

    TH3D *hBBtag_mB = (TH3D *) fin_aggrTMVA->Get("hBBtag_mB");
    TH3D *hBBtag_mB_gen = (TH3D *) fin_aggrTMVA->Get("hBBtag_mB_gen");

    Int_t izmin = hBtag_mB->GetZaxis()->FindBin(ptMin);
    Int_t izmax = hBtag_mB->GetZaxis()->FindBin(ptMax) - 1;

    // no kt cut needed for mb
    Int_t iymin = 0;
    Int_t iymax = hBtag_mB->GetNbinsY()+1;

    THStack *hStack_mB = new THStack("hStack_mB", "");
    hStack_mB->SetTitle("; m_{B}^{ch}; # jets");

    TLegend *leg_mB = new TLegend(0.5, 0.6, 0.85, 0.85);
    leg_mB->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), b tagged jets", ptMin, ptMax));
    leg_mB->SetFillStyle(0);
    leg_mB->SetBorderSize(0);
    leg_mB->SetMargin(0.15);

    THStack *hStack_mB_all = new THStack("hStack_mB_all", "");
    hStack_mB_all->SetTitle("; m_{B}^{ch}; # jets");

    TLegend *leg_mB_all = new TLegend(0.5, 0.6, 0.85, 0.85);
    leg_mB_all->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", ptMin, ptMax));
    leg_mB_all->SetFillStyle(0);
    leg_mB_all->SetBorderSize(0);
    leg_mB_all->SetMargin(0.15);

    // ---- Make projections 
    // gen all
    TH1D *hSingleB_mB_gen_1d = (TH1D *) hSingleB_mB_gen->ProjectionX("hSingleB_mB_gen_1d", iymin, iymax, izmin, izmax);
    format_histo(hSingleB_mB_gen_1d, mykRedLight);
    hSingleB_mB_gen_1d->SetFillStyle(1000);
    leg_mB_all->AddEntry(hSingleB_mB_gen_1d, "Particle level, single b", "lf");
    hStack_mB_all->Add(hSingleB_mB_gen_1d);

    TH1D *hBB_mB_gen_1d = (TH1D *) hBB_mB_gen->ProjectionX("hBB_mB_gen_1d", iymin, iymax, izmin, izmax);
    format_histo(hBB_mB_gen_1d, mykBlueLight);
    hBB_mB_gen_1d->SetFillStyle(1000);
    leg_mB_all->AddEntry(hBB_mB_gen_1d, "Particle level, bb", "lf");
    hStack_mB_all->Add(hBB_mB_gen_1d);

    TH1D *hC_mB_gen_1d = (TH1D *) hC_mB_gen->ProjectionX("hC_mB_gen_1d", iymin, iymax, izmin, izmax);
    format_histo(hC_mB_gen_1d, mykGreenLight);
    hC_mB_gen_1d->SetFillStyle(1000);
    leg_mB_all->AddEntry(hC_mB_gen_1d, "Particle level, c", "lf");
    hStack_mB_all->Add(hC_mB_gen_1d);

    TH1D *hL_mB_gen_1d = (TH1D *) hL_mB_gen->ProjectionX("hL_mB_gen_1d", iymin, iymax, izmin, izmax);
    format_histo(hL_mB_gen_1d, mykYellowLight);
    hL_mB_gen_1d->SetFillStyle(1000);
    leg_mB_all->AddEntry(hL_mB_gen_1d, "Particle level, guds", "lf");
    hStack_mB_all->Add(hL_mB_gen_1d);

    // gen tagged
    TH1D *hSingleBtag_mB_gen_1d = (TH1D *) hSingleBtag_mB_gen->ProjectionX("hSingleBtag_mB_gen_1d", iymin, iymax, izmin, izmax);
    format_histo(hSingleBtag_mB_gen_1d, mykRedLight);
    hSingleBtag_mB_gen_1d->SetFillStyle(1000);
    leg_mB->AddEntry(hSingleBtag_mB_gen_1d, "Particle level, single b", "lf");
    hStack_mB->Add(hSingleBtag_mB_gen_1d);

    TH1D *hBBtag_mB_gen_1d = (TH1D *) hBBtag_mB_gen->ProjectionX("hBBtag_mB_gen_1d", iymin, iymax, izmin, izmax);
    format_histo(hBBtag_mB_gen_1d, mykBlueLight);
    hBBtag_mB_gen_1d->SetFillStyle(1000);
    leg_mB->AddEntry(hBBtag_mB_gen_1d, "Particle level, bb", "lf");
    hStack_mB->Add(hBBtag_mB_gen_1d);

    TH1D *hCtag_mB_gen_1d = (TH1D *) hCtag_mB_gen->ProjectionX("hCtag_mB_gen_1d", iymin, iymax, izmin, izmax);
    format_histo(hCtag_mB_gen_1d, mykGreenLight);
    hCtag_mB_gen_1d->SetFillStyle(1000);
    leg_mB->AddEntry(hCtag_mB_gen_1d, "Particle level, c", "lf");
    hStack_mB->Add(hCtag_mB_gen_1d);

    TH1D *hLtag_mB_gen_1d = (TH1D *) hLtag_mB_gen->ProjectionX("hLtag_mB_gen_1d", iymin, iymax, izmin, izmax);
    format_histo(hLtag_mB_gen_1d, mykYellowLight);
    hLtag_mB_gen_1d->SetFillStyle(1000);
    leg_mB->AddEntry(hLtag_mB_gen_1d, "Particle level, guds", "lf");
    hStack_mB->Add(hLtag_mB_gen_1d);

    THStack *hStack_mB_norm = new THStack("hStack_mB_norm", "");
    hStack_mB_norm->SetTitle("; m_{B}^{ch}; #frac{1}{N_{gen jets}} #frac{dN}{d m_{B}^{ch}}");

    TLegend *leg_mB_norm = new TLegend(0.5, 0.6, 0.85, 0.85);
    leg_mB_norm->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), b tagged jets", ptMin, ptMax));
    leg_mB_norm->SetFillStyle(0);
    leg_mB_norm->SetBorderSize(0);
    leg_mB_norm->SetMargin(0.15);


    TH1D *hXtag_mB_gen_1d = (TH1D *) hSingleBtag_mB_gen_1d->Clone("hXtag_mB_gen_1d");
    hXtag_mB_gen_1d->Add(hBBtag_mB_gen_1d);
    hXtag_mB_gen_1d->Add(hCtag_mB_gen_1d);
    hXtag_mB_gen_1d->Add(hLtag_mB_gen_1d);
    hXtag_mB_gen_1d->Scale(1 / hXtag_mB_gen_1d->Integral("width"));
    hXtag_mB_gen_1d->SetLineColor(mykBlue);
    hXtag_mB_gen_1d->SetFillStyle(0);
    hXtag_mB_gen_1d->SetLineWidth(2);
    hStack_mB_norm->Add(hXtag_mB_gen_1d);
    leg_mB_norm->AddEntry(hXtag_mB_gen_1d, "Particle level, b+bb+c+l", "l");

    //reco 
    TH1D *hSingleBtag_mB_1d = (TH1D *) hSingleBtag_mB->ProjectionX("hSingleBtag_mB_1d", iymin, iymax, izmin, izmax);
    TH1D *hBBtag_mB_1d = (TH1D *) hBBtag_mB->ProjectionX("hBBtag_mB_1d", iymin, iymax, izmin, izmax);
    TH1D *hCtag_mB_1d = (TH1D *) hCtag_mB->ProjectionX("hCtag_mB_1d", iymin, iymax, izmin, izmax);
    TH1D *hLtag_mB_1d = (TH1D *) hLtag_mB->ProjectionX("hLtag_mB_1d", iymin, iymax, izmin, izmax);

    TH1D *hXtag_mB_1d = (TH1D *) hSingleBtag_mB_1d->Clone("hXtag_mB_1d");
    hXtag_mB_1d->Add(hBBtag_mB_1d);
    hXtag_mB_1d->Add(hCtag_mB_1d);
    hXtag_mB_1d->Add(hLtag_mB_1d);
    hXtag_mB_1d->Scale(1 / hXtag_mB_1d->Integral("width"));
    hXtag_mB_1d->SetLineColor(mykRed);
    hXtag_mB_1d->SetFillStyle(0);
    hXtag_mB_1d->SetLineWidth(2);
    hStack_mB_norm->Add(hXtag_mB_1d);
    leg_mB_norm->AddEntry(hXtag_mB_1d, "Detector level, b+bb+c+l", "l");

    // ---- Draw the histos

    // Get percentage of bb
    double n_bb = hBBtag_mB_gen_1d->Integral(0, -1);
    double n_b = hSingleBtag_mB_gen_1d->Integral(0, -1);
    double n_c = hCtag_mB_gen_1d->Integral(0, -1);
    double n_l = hLtag_mB_gen_1d->Integral(0, -1);

    double n_bb_all = hBB_mB_gen_1d->Integral(0, -1);
    double n_b_all = hSingleB_mB_gen_1d->Integral(0, -1);
    double n_c_all = hC_mB_gen_1d->Integral(0, -1);
    double n_l_all = hL_mB_gen_1d->Integral(0, -1);

    TPaveText *info = new TPaveText(0.65, 0.25, 0.8, 0.45, "ndc");
    // info->SetTextColor(hBBtag_mB_gen_1d->GetFillColor());
    info->SetFillStyle(0);
    info->SetBorderSize(0);
    info->SetTextSize(text_size);
    info->SetTextAlign(11); 
    // info->AddText(Form("single b eff = %.2f %%", n_b / n_b_all));
    // auto line = info->AddLine(0.01, 0.3, 0.99, 0.3); // crashes
    TText *b_info = info->AddText(Form("b = %.2f %%", n_b*100/(n_b+n_bb+n_c+n_l)));
    b_info->SetTextColor(hSingleBtag_mB_gen_1d->GetFillColor());
    TText *bb_info = info->AddText(Form("bb = %.2f %%", n_bb*100/(n_b+n_bb+n_c+n_l)));
    bb_info->SetTextColor(hBBtag_mB_gen_1d->GetFillColor());
    TText *c_info = info->AddText(Form("c = %.2f %%", n_c*100/(n_b+n_bb+n_c+n_l)));
    c_info->SetTextColor(hCtag_mB_gen_1d->GetFillColor());
    TText *l_info = info->AddText(Form("l = %.2f %%", n_l*100/(n_b+n_bb+n_c+n_l)));
    l_info->SetTextColor(hLtag_mB_gen_1d->GetFillColor());

    TPaveText *info_all = new TPaveText(0.65, 0.3, 0.8, 0.5, "ndc");
    // info_all->SetTextColor(hBBtag_mB_gen_1d->GetFillColor());
    info_all->SetFillStyle(0);
    info_all->SetBorderSize(0);
    info_all->SetTextSize(text_size);
    info_all->SetTextAlign(11); 
    TText *b_info_all = info_all->AddText(Form("b = %.2f %%", n_b_all*100/(n_b_all+n_bb_all+n_c_all+n_l_all)));
    b_info_all->SetTextColor(hSingleB_mB_gen_1d->GetFillColor());
    TText *bb_info_all = info_all->AddText(Form("bb = %.2f %%", n_bb_all*100/(n_b_all+n_bb_all+n_c_all+n_l_all)));
    bb_info_all->SetTextColor(hBB_mB_gen_1d->GetFillColor());
    TText *c_info_all = info_all->AddText(Form("c = %.2f %%", n_c_all*100/(n_b_all+n_bb_all+n_c_all+n_l_all)));
    c_info_all->SetTextColor(hC_mB_gen_1d->GetFillColor());
    TText *l_info_all = info_all->AddText(Form("l = %.2f %%", n_l_all*100/(n_b_all+n_bb_all+n_c_all+n_l_all)));
    l_info_all->SetTextColor(hL_mB_gen_1d->GetFillColor());

    TCanvas *c_mB_stack = new TCanvas("c_mB_stack", "", 1200, 800);   
    c_mB_stack->cd();
    hStack_mB->Draw("histo");
    leg_mB->Draw();
    info->Draw();
    c_mB_stack->Draw();

    TString cname_mB_stack = "./plots/mB_stack.png";
    c_mB_stack->Print(cname_mB_stack);

    TCanvas *c_mB_stack_all = new TCanvas("c_mB_stack_all", "", 1200, 800);   
    c_mB_stack_all->cd();
    c_mB_stack_all->SetLogy();
    hStack_mB_all->Draw("histo");
    leg_mB_all->Draw();
    info_all->Draw();
    c_mB_stack_all->Draw();

    TCanvas *c_mB_norm = new TCanvas("c_mB_norm", "", 1200, 800);   
    c_mB_norm->cd();
    hStack_mB_norm->Draw("histo nostack");
    leg_mB_norm->Draw();
    c_mB_norm->Draw();

    TString cname_mB_norm = "./plots/mB_norm.png";
    c_mB_norm->Print(cname_mB_norm);

    // debug 
    std::cout << "single b integral pass wp = " << hSingleBtag_mB_gen_1d->Integral(0, -1) << std::endl;
    std::cout << "double b integral pass wp = " << hBBtag_mB_gen_1d->Integral(0, -1) << std::endl;

    std::cout << "b+bb tag / b+bb total = " << (hSingleBtag_mB_gen_1d->Integral(0, -1) + hBBtag_mB_gen_1d->Integral(0, -1)) / (hSingleB_mB_gen_1d->Integral(0, -1) + hBB_mB_gen_1d->Integral(0, -1)) << std::endl;
    std::cout << "single b tag / single b total = " << (hSingleBtag_mB_gen_1d->Integral(0, -1)) / (hSingleB_mB_gen_1d->Integral(0, -1)) << std::endl;
    
}