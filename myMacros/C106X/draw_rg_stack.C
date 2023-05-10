// #include "myPalette.h"
#include "hist_utils.h"

void draw_rg_stack()
{
    // ---- Settings ----
    double ptMin = 100.;
    double ptMax = 120.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos ----
    TString indir = "./histos/qcd_bjet_";
    TString label = "aggrTMVA_lateKt";
    TString fname = indir + label + "_rgkt.root";
    TFile *fin = new TFile(fname);

    // true distributions
    TH3D *hSingleB_rgkt_gen = (TH3D *) fin->Get("hSingleB_rgkt_gen");
    TH3D *hBB_rgkt_gen = (TH3D *) fin->Get("hBB_rgkt_gen");
    TH3D *hL_rgkt_gen = (TH3D *) fin->Get("hL_rgkt_gen");
    TH3D *hC_rgkt_gen = (TH3D *) fin->Get("hC_rgkt_gen");

    // tagged
    TH3D *hSingleBtag_rgkt_gen = (TH3D *) fin->Get("hSingleBtag_rgkt_gen");
    TH3D *hBBtag_rgkt_gen = (TH3D *) fin->Get("hBBtag_rgkt_gen");
    TH3D *hLtag_rgkt_gen = (TH3D *) fin->Get("hLtag_rgkt_gen");
    TH3D *hCtag_rgkt_gen = (TH3D *) fin->Get("hCtag_rgkt_gen");

    // Find pt bins
    Int_t izmin = hSingleB_rgkt_gen->GetZaxis()->FindBin(ptMin);
    Int_t izmax = hSingleB_rgkt_gen->GetZaxis()->FindBin(ptMax) - 1;

    // Find kt bins 
    double ktMin = 0.;
    Int_t iymin = hSingleB_rgkt_gen->GetYaxis()->FindBin(std::log(ktMin));
    Int_t iymax = hSingleB_rgkt_gen->GetNbinsY() + 1;

    // Make true rg projections 
    THStack *hStack = new THStack ("hStack", "");
    hStack->SetTitle("; ln(0.4/R_{g}); # jets");

    TLegend *leg = new TLegend(0.5, 0.7, 0.8, 0.85);
    leg->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > %.0f", ptMin, ptMax, ktMin));

    TH1F *hSingleB = (TH1F *) hSingleB_rgkt_gen->ProjectionX("hSingleB", iymin, iymax, izmin, izmax);
    hSingleB->SetFillStyle(1000);
    format_histo(hSingleB, mykRedLight);
    hStack->Add(hSingleB);
    leg->AddEntry(hSingleB, "Particle level, single b", "f");
    // std::cout << hSingleB->Integral() << std::endl;

    TH1F *hBB = (TH1F *) hBB_rgkt_gen->ProjectionX("hBB", iymin, iymax, izmin, izmax);
    hBB->SetFillStyle(1000);
    format_histo(hBB, mykBlueLight);
    hStack->Add(hBB);
    leg->AddEntry(hBB, "Particle level, bb", "f");

    TH1F *hC = (TH1F *) hC_rgkt_gen->ProjectionX("hC", iymin, iymax, izmin, izmax);
    hC->SetFillStyle(1000);
    format_histo(hC, mykGreenLight);
    // hStack->Add(hC);
    // leg->AddEntry(hC, "Particle level, c", "f");
    // std::cout << hC->Integral() << std::endl;

    TH1F *hL = (TH1F *) hL_rgkt_gen->ProjectionX("hL", iymin, iymax, izmin, izmax);
    hL->SetFillStyle(1000);
    format_histo(hL, mykYellowLight);
    // hStack->Add(hL);
    // leg->AddEntry(hL, "Particle level, guds", "f");
    // std::cout << hL->Integral() << std::endl;

    // Make tagged rg projections 
    THStack *hStack_tagged = new THStack ("hStack_tagged", "");
    hStack_tagged->SetTitle("; ln(0.4/R_{g}); # jets");

    TLegend *leg_tagged = new TLegend(0.45, 0.65, 0.8, 0.85);
    leg_tagged->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > %.0f, b tagged jets", ptMin, ptMax, ktMin));

    TH1F *hSingleBtag = (TH1F *) hSingleBtag_rgkt_gen->ProjectionX("hSingleBtag", iymin, iymax, izmin, izmax);
    hSingleBtag->SetFillStyle(1000);
    format_histo(hSingleBtag, mykRedLight);
    hStack_tagged->Add(hSingleBtag);
    leg_tagged->AddEntry(hSingleBtag, "Particle level, single b", "f");
    // std::cout << hSingleBtag->Integral() << std::endl;

    TH1F *hBBtag = (TH1F *) hBBtag_rgkt_gen->ProjectionX("hBBtag", iymin, iymax, izmin, izmax);
    hBBtag->SetFillStyle(1000);
    format_histo(hBBtag, mykBlueLight);
    hStack_tagged->Add(hBBtag);
    leg_tagged->AddEntry(hBBtag, "Particle level, bb", "f");

    TH1F *hCtag = (TH1F *) hCtag_rgkt_gen->ProjectionX("hCtag", iymin, iymax, izmin, izmax);
    hCtag->SetFillStyle(1000);
    format_histo(hCtag, mykGreenLight);
    hStack_tagged->Add(hCtag);
    leg_tagged->AddEntry(hCtag, "c jets", "f");
    // std::cout << hCtag->Integral() << std::endl;

    TH1F *hLtag = (TH1F *) hLtag_rgkt_gen->ProjectionX("hLtag", iymin, iymax, izmin, izmax);
    hLtag->SetFillStyle(1000);
    format_histo(hLtag, mykYellowLight);
    hStack_tagged->Add(hLtag);
    leg_tagged->AddEntry(hLtag, "guds jets", "f");
    // std::cout << hLtag->Integral() << std::endl;

    // ---- Get percentage of bb ----
    double n_bb = hBBtag->Integral(0, -1);
    double n_b = hSingleBtag->Integral(0, -1);
    double n_c = hCtag->Integral(0, -1);
    double n_l = hLtag->Integral(0, -1);

    double n_bb_all = hBB->Integral(0, -1);
    double n_b_all = hSingleB->Integral(0, -1);
    double n_c_all = hC->Integral(0, -1);
    double n_l_all = hL->Integral(0, -1);

    TPaveText *info = new TPaveText(0.65, 0.25, 0.8, 0.45, "ndc");
    // info->SetTextColor(hBBtag->GetFillColor());
    info->SetFillStyle(0);
    info->SetBorderSize(0);
    info->SetTextSize(text_size);
    info->SetTextAlign(11); 
    // info->AddText(Form("single b eff = %.2f %%", n_b / n_b_all));
    // auto line = info->AddLine(0.01, 0.3, 0.99, 0.3); // crashes
    TText *b_info = info->AddText(Form("b = %.2f %%", n_b*100/(n_b+n_bb+n_c+n_l)));
    b_info->SetTextColor(hSingleBtag->GetFillColor());
    TText *bb_info = info->AddText(Form("bb = %.2f %%", n_bb*100/(n_b+n_bb+n_c+n_l)));
    bb_info->SetTextColor(hBBtag->GetFillColor());
    TText *c_info = info->AddText(Form("c = %.2f %%", n_c*100/(n_b+n_bb+n_c+n_l)));
    c_info->SetTextColor(hCtag->GetFillColor());
    TText *l_info = info->AddText(Form("l = %.2f %%", n_l*100/(n_b+n_bb+n_c+n_l)));
    l_info->SetTextColor(hLtag->GetFillColor());

    TPaveText *info_all = new TPaveText(0.65, 0.3, 0.8, 0.5, "ndc");
    // info_all->SetTextColor(hBBtag->GetFillColor());
    info_all->SetFillStyle(0);
    info_all->SetBorderSize(0);
    info_all->SetTextSize(text_size);
    info_all->SetTextAlign(11); 
    TText *b_info_all = info_all->AddText(Form("b = %.2f %%", n_b_all*100/(n_b_all+n_bb_all+n_c_all+n_l_all)));
    b_info_all->SetTextColor(hSingleB->GetFillColor());
    TText *bb_info_all = info_all->AddText(Form("bb = %.2f %%", n_bb_all*100/(n_b_all+n_bb_all+n_c_all+n_l_all)));
    bb_info_all->SetTextColor(hBB->GetFillColor());
    TText *c_info_all = info_all->AddText(Form("c = %.2f %%", n_c_all*100/(n_b_all+n_bb_all+n_c_all+n_l_all)));
    c_info_all->SetTextColor(hC->GetFillColor());
    TText *l_info_all = info_all->AddText(Form("l = %.2f %%", n_l_all*100/(n_b_all+n_bb_all+n_c_all+n_l_all)));
    l_info_all->SetTextColor(hL->GetFillColor());

    // Draw
    TCanvas *c = new TCanvas("c", "", 1200, 1000);
    // gPad->SetLogy();
    // hStack->SetMinimum(1);
    // hStack->SetMaximum(1);
    hStack->Draw("hist");
    info_all->Draw();
    leg->Draw();
    c->Draw();

    TCanvas *c_tagged = new TCanvas("c_tagged", "", 1200, 1000);
    // gPad->SetLogy();
    // hStack_tagged->SetMinimum(1);
    // hStack_tagged->SetMaximum(10000);
    hStack_tagged->Draw("hist");
    leg_tagged->Draw();
    info->Draw();
    c_tagged->Draw();

    Float_t total_jets = hSingleB->Integral(0, -1) + hBB->Integral(0, -1) + hL->Integral(0, -1) + hC->Integral(0, -1);
    std::cout << "single b = " << hSingleB->Integral(0, -1) / total_jets << " of total" << std::endl; 
    std::cout << "bb = " << hBB->Integral(0, -1) / total_jets << " of total" << std::endl; 
    std::cout << "c = " << hC->Integral(0, -1) / total_jets << " of total" << std::endl; 
    std::cout << "l = " << hL->Integral(0, -1) / total_jets << " of total" << std::endl; 

    Float_t total_tagged_jets = hSingleBtag->Integral(0, -1) + hBBtag->Integral(0, -1) + hLtag->Integral(0, -1) + hCtag->Integral(0, -1);
    std::cout << "single b = " << hSingleBtag->Integral(0, -1) / total_tagged_jets << " of tagged" << std::endl; 
    std::cout << "bb = " << hBBtag->Integral(0, -1) / total_tagged_jets << " of tagged" << std::endl; 
    std::cout << "c = " << hCtag->Integral(0, -1) / total_tagged_jets << " of tagged" << std::endl; 
    std::cout << "l = " << hLtag->Integral(0, -1) / total_tagged_jets << " of tagged" << std::endl; 

    std::cout << "b+bb tag / b+bb total (with kt cut) = " << (hSingleBtag->Integral(0, -1) + hBBtag->Integral(0, -1)) / (hSingleB->Integral(0, -1) + hBB->Integral(0, -1)) << std::endl;
    std::cout << "single b tag / single b total (with kt cut) = " << (hSingleBtag->Integral(0, -1)) / (hSingleB->Integral(0, -1)) << std::endl;

    // std::cout << "total b jets " << hSingleB->Integral(0, -1) << ", tagged b jets " << hSingleBtag->Integral(0, -1) << std::endl;
}