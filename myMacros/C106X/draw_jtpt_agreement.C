// #include "draw_utils.h"
void drawHeaderSimulation(void) {
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.15, 0.965, "#bf{CMS} #it{Private work}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.95, 0.965, "pp 301^{} pb^{-1} (5.02 TeV)");
}

void draw_jtpt_agreement()
{
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

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
    h_jtpt_incl_1d->GetXaxis()->SetTitle("p_{T}^{jet,reco}/p_{T}^{jet,true}");
    h_jtpt_incl_1d->GetXaxis()->SetTitleOffset(1.3);
    h_jtpt_incl_1d->GetYaxis()->SetTitle("1/N dN/dp_{T}^{jet,reco}/p_{T}^{jet,true}");
    h_jtpt_incl_1d->GetYaxis()->SetRangeUser(0., 0.15);

    TH1D *h_jtpt_bjet_1d = (TH1D *) h_jtpt_bjet->ProjectionY();
    h_jtpt_bjet_1d->Scale(1/h_jtpt_bjet_1d->Integral());

    TLegend *leg = new TLegend(0.7, 0.45, 0.9, 0.6);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->AddEntry(h_jtpt_incl_1d, "Inclusive jets", "pe1");
    leg->AddEntry(h_jtpt_bjet_1d, "b jets", "pe1");

    TCanvas *c_jtpt_ratio = new TCanvas("c_jtpt_ratio", "", 700, 600);
    c_jtpt_ratio->SetLeftMargin(0.15);
    c_jtpt_ratio->SetRightMargin(0.05);
    c_jtpt_ratio->SetTopMargin(0.07);
    c_jtpt_ratio->SetBottomMargin(0.15);
    h_jtpt_incl_1d->Draw("pe1");
    h_jtpt_bjet_1d->Draw("pe1 same");
    // c_jtpt_ratio->BuildLegend();
    leg->Draw();
    drawHeaderSimulation();

    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size-4);
    jet_info->SetTextAlign(12);
    jet_info->DrawLatex(0.18, 0.86, "anti-k_{T}, R=0.4 jets");
    jet_info->DrawLatex(0.18, 0.8, "80 < p_{T}^{jet,true} < 140 (GeV/c)");
    jet_info->DrawLatex(0.18, 0.74, "|#eta^{jet}| < 2");
    jet_info->Draw();

    // c_jtpt_ratio->Print("plots_an/jtpt_agreement_ratio.png");
    c_jtpt_ratio->Print("plots_thesis/jtpt_agreement_ratio.pdf");

    TProfile *h_jtpt_incl_prof = (TProfile *) h_jtpt_incl->ProfileX();
    h_jtpt_incl_prof->GetXaxis()->SetTitle("p_{T}^{jet,true} (GeV/c)");
    h_jtpt_incl_prof->GetYaxis()->SetTitle("<p_{T}^{jet,reco}/p_{T}^{jet,true}>");
    h_jtpt_incl_prof->GetYaxis()->SetRangeUser(0.95, 1.05);
    h_jtpt_incl_prof->GetXaxis()->SetTitleOffset(1.3);

    TProfile *h_jtpt_bjet_prof = (TProfile *) h_jtpt_bjet->ProfileX();

    TLegend *leg_prof = new TLegend(0.7, 0.7, 0.9, 0.85);
    leg_prof->SetFillStyle(0);
    leg_prof->SetBorderSize(0);
    leg_prof->SetMargin(0.15);
    leg_prof->AddEntry(h_jtpt_incl_1d, "Inclusive jets", "pe1");
    leg_prof->AddEntry(h_jtpt_bjet_1d, "b jets", "pe1");

    TCanvas *c_jtpt_prof = new TCanvas("c_jtpt_prof", "", 700, 600);
    c_jtpt_prof->SetLeftMargin(0.15);
    c_jtpt_prof->SetRightMargin(0.05);
    c_jtpt_prof->SetTopMargin(0.07);
    c_jtpt_prof->SetBottomMargin(0.15);
    h_jtpt_incl_prof->Draw("pe1");
    h_jtpt_bjet_prof->Draw("pe1 same");
    // c_jtpt_prof->BuildLegend();
    leg_prof->Draw();
    drawHeaderSimulation();

    jet_info->DrawLatex(0.2, 0.86, "anti-k_{T}, R=0.4 jets");
    // jet_info->DrawLatex(0.2, 0.8, "80 < p_{T}^{jet,reco} < 140 (GeV/c)");
    jet_info->DrawLatex(0.2, 0.8, "|#eta^{jet}| < 2");

    // c_jtpt_prof->Print("plots_an/jtpt_agreement_prof.png");
    c_jtpt_prof->Print("plots_thesis/jtpt_agreement_prof.pdf");
}