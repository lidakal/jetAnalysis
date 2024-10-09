#include "../cms_palette.h"

void draw_info()
{
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(12);
    prelim->DrawLatex(0.14, 0.93, "#bf{CMS} #it{Simulation}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(32);
    lumi->DrawLatex(0.97, 0.93, "PYTHIA8 CP5 (pp 5.02 TeV)");
}

void draw_rg_parton_flav(TString observable="rg")
{
    // Run with ROOT 6.30, take PDFs
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetPalette(57);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R^{}/^{}R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    TString ylabel = "1/N^{} dN^{}/^{}d^{}" + xlabel;

    TFile *fin = new TFile("histos/dijet_PF40_aggrTMVA_inclusive_histograms_per_refflavor_gen.root");
    TH2D *h_g = (TH2D *) fin->Get("h_"+observable+"_g");
    TH2D *h_u = (TH2D *) fin->Get("h_"+observable+"_u");
    TH2D *h_d = (TH2D *) fin->Get("h_"+observable+"_d");
    TH2D *h_s = (TH2D *) fin->Get("h_"+observable+"_s");
    TH2D *h_c = (TH2D *) fin->Get("h_"+observable+"_c");
    TH2D *h_b = (TH2D *) fin->Get("h_"+observable+"_b");
    TH2D *h_bb = (TH2D *) fin->Get("h_"+observable+"_bb");
    TH2D *h_undef = (TH2D *) fin->Get("h_"+observable+"_undef");

    // Add histograms
    TH2D *h_q = (TH2D *) h_u->Clone("h_q");
    h_q->Add(h_d);
    h_q->Add(h_s);

    TH2D *h_gu = (TH2D *) h_g->Clone("h_gu");
    // h_gu->Add(h_undef);

    TH2D *h_incl = (TH2D *) h_u->Clone("h_incl");
    h_incl->Add(h_d);
    h_incl->Add(h_s);
    h_incl->Add(h_c);
    h_incl->Add(h_b);
    h_incl->Add(h_bb);
    h_incl->Add(h_g);
    h_incl->Add(h_undef);

    // Make projections and normalize
    int nbins_x = h_b->GetNbinsX();
    int nbins_pt = h_b->GetNbinsY();

    int ibin_pt = 2;
    int ibin_x_min = 2;
    int ibin_x_max = (observable=="rg") ? nbins_x - 1 : nbins_x;

    TH1D *h_u_1d = (TH1D *) h_u->ProjectionX("h_u_1d", ibin_pt, ibin_pt);
    TH1D *h_d_1d = (TH1D *) h_d->ProjectionX("h_d_1d", ibin_pt, ibin_pt);
    TH1D *h_s_1d = (TH1D *) h_s->ProjectionX("h_s_1d", ibin_pt, ibin_pt);
    TH1D *h_c_1d = (TH1D *) h_c->ProjectionX("h_c_1d", ibin_pt, ibin_pt);
    TH1D *h_b_1d = (TH1D *) h_b->ProjectionX("h_b_1d", ibin_pt, ibin_pt);
    TH1D *h_g_1d = (TH1D *) h_g->ProjectionX("h_g_1d", ibin_pt, ibin_pt);
    TH1D *h_q_1d = (TH1D *) h_q->ProjectionX("h_q_1d", ibin_pt, ibin_pt);
    TH1D *h_undef_1d = (TH1D *) h_undef->ProjectionX("h_undef_1d", ibin_pt, ibin_pt);
    TH1D *h_gu_1d = (TH1D *) h_gu->ProjectionX("h_gu_1d", ibin_pt, ibin_pt);
    TH1D *h_incl_1d = (TH1D *) h_incl->ProjectionX("h_incl_1d", ibin_pt, ibin_pt);

    TCanvas *c_flav = new TCanvas("c_flav", "", 800, 600);

    int color = 1;
    for (auto h : {
        h_g_1d, 
        // h_u_1d, h_d_1d, h_s_1d, 
        // h_c_1d, 
        h_b_1d, 
        h_q_1d, 
        h_undef_1d,
        // h_gu_1d, 
        h_incl_1d, 
    }) {
        h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        h->Scale(1/h->Integral(ibin_x_min, ibin_x_max), "width");
        h->SetMarkerColor(color);
        h->SetTitle(h->GetName());
        h->Draw("same");
        color++;
    }

    c_flav->BuildLegend();

    TCanvas *c_ratio = new TCanvas("c_ratio", "", 800, 700);
    c_ratio->SetRightMargin(0.03);
    c_ratio->SetLeftMargin(0.14);

    TH1D *h_b_over_incl = (TH1D *) h_b_1d->Clone("h_b_over_incl");
    h_b_over_incl->Divide(h_incl_1d);
    h_b_over_incl->SetTitle("b / inclusive jets");
    h_b_over_incl->GetXaxis()->SetTitle(xlabel);
    h_b_over_incl->GetYaxis()->SetTitle("Ratio");
    h_b_over_incl->GetYaxis()->SetRangeUser(0., 2.);
    if (observable=="zg") h_b_over_incl->GetYaxis()->SetRangeUser(0.6, 1.4);
    h_b_over_incl->SetMarkerSize(0);
    h_b_over_incl->SetLineColor(cmsBlue);
    h_b_over_incl->SetLineWidth(3);
    h_b_over_incl->SetLineStyle(kSolid);
    h_b_over_incl->Draw("hist same");

    TLine *line = new TLine(h_b_over_incl->GetXaxis()->GetBinLowEdge(ibin_x_min), 1, h_b_over_incl->GetXaxis()->GetBinUpEdge(ibin_x_max), 1);
    line->SetLineStyle(kSolid);
    line->SetLineColor(kBlack);

    line->Draw();
    h_b_over_incl->Draw("hist same");

    TH1D *h_b_over_q = (TH1D *) h_b_1d->Clone("h_b_over_q");
    h_b_over_q->Divide(h_q_1d);
    h_b_over_q->SetTitle("b / light quark jets");
    h_b_over_q->SetMarkerSize(0);
    h_b_over_q->SetLineWidth(3);
    h_b_over_q->SetLineColor(cmsRed);
    h_b_over_q->SetLineStyle(9);
    h_b_over_q->Draw("hist same");

    TLegend *leg_ratio = new TLegend(0.5, 0.65, 0.85, 0.8);
    leg_ratio->SetBorderSize(0);
    leg_ratio->SetMargin(0.25);
    leg_ratio->AddEntry(h_b_over_incl, "b jets / inclusive jets", "l");
    leg_ratio->AddEntry(h_b_over_q, "b jets / light quark jets", "l");
    leg_ratio->Draw();

    draw_info();

    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    jet_info->SetTextAlign(12);
    // for pdf
    jet_info->DrawLatex(0.2, 0.46, "Particle level");
    jet_info->DrawLatex(0.2, 0.4, "anti-k_{T}, R=0.4 jets");
    jet_info->DrawLatex(0.2, 0.34, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    jet_info->DrawLatex(0.2, 0.28, "Soft drop (charged particles)");
    jet_info->DrawLatex(0.2, 0.22, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");

    c_ratio->Print("plots_an/gen_"+observable+"_vs_light_quark.pdf");

}