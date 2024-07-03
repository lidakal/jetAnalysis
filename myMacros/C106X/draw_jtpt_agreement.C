#include "draw_utils.h"

void draw_jtpt_agreement()
{
    TFile *fin_incl = new TFile("./histos/dijet_PF40_aggrTMVA_inclusive_jtpt_agreement.root"); 
    TH2D *h_jtpt_incl = (TH2D *) fin_incl->Get("h_jtpt")->Clone("h_jtpt_incl");
    h_jtpt_incl->SetMarkerColor(kBlack);
    h_jtpt_incl->SetLineColor(kBlack);
    h_jtpt_incl->SetMarkerStyle(kFullCircle);
    h_jtpt_incl->SetTitle("inclusive jets");

    TFile *fin_bjet = new TFile("./histos/pythia_PF40_aggrTMVA_inclusive_jtpt_agreement.root"); 
    TH2D *h_jtpt_bjet = (TH2D *) fin_bjet->Get("h_jtpt")->Clone("h_jtpt_bjet");
    h_jtpt_bjet->SetMarkerColor(kBlue);
    h_jtpt_bjet->SetLineColor(kBlue);
    h_jtpt_bjet->SetMarkerStyle(kFullTriangleUp);
    h_jtpt_bjet->SetTitle("b jets");
    
    TH1D *h_jtpt_incl_1d = (TH1D *) h_jtpt_incl->ProjectionY();
    h_jtpt_incl_1d->Scale(1/h_jtpt_incl_1d->Integral());
    h_jtpt_incl_1d->GetXaxis()->SetTitle("p_{T}^{jet,rceo}/p_{T}^{jet,true}");
    h_jtpt_incl_1d->GetYaxis()->SetTitle("1/N dN/dp_{T}^{jet,rceo}/p_{T}^{jet,true}");

    TH1D *h_jtpt_bjet_1d = (TH1D *) h_jtpt_bjet->ProjectionY();
    h_jtpt_bjet_1d->Scale(1/h_jtpt_bjet_1d->Integral());

    TCanvas *c_jtpt_ratio = new TCanvas("c_jtpt_ratio", "", 800, 600);
    h_jtpt_incl_1d->Draw("pe1");
    h_jtpt_bjet_1d->Draw("pe1 same");
    c_jtpt_ratio->BuildLegend();
    drawHeaderSimulation();
    c_jtpt_ratio->Print("plots_an/jtpt_agreement_ratio.png");

    TProfile *h_jtpt_incl_prof = (TProfile *) h_jtpt_incl->ProfileX();
    h_jtpt_incl_prof->GetXaxis()->SetTitle("p_{T}^{jet,true}");
    h_jtpt_incl_prof->GetYaxis()->SetTitle("<p_{T}^{jet,rceo}/p_{T}^{jet,true}>");
    h_jtpt_incl_prof->GetYaxis()->SetRangeUser(0.975, 1.005);

    TProfile *h_jtpt_bjet_prof = (TProfile *) h_jtpt_bjet->ProfileX();

    TCanvas *c_jtpt_prof = new TCanvas("c_jtpt_prof", "", 800, 600);
    h_jtpt_incl_prof->Draw("pe1");
    h_jtpt_bjet_prof->Draw("pe1 same");
    c_jtpt_prof->BuildLegend();
    drawHeaderSimulation();
    c_jtpt_prof->Print("plots_an/jtpt_agreement_prof.png");




}