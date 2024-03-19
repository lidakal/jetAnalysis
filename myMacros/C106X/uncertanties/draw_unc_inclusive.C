#include "../draw_utils.h"

void draw_unc_inclusive(TString observable="rg")
{
    TString xlabel;
    if (observable=="rg") xlabel = "ln(0.4/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";
    
    gStyle->SetErrorX(0.5);

    TFile *fin_jer = new TFile("histos/"+observable+"_jer_unc_inclusive.root");
    TFile *fin_jec = new TFile("histos/"+observable+"_jec_unc_inclusive.root");
    TFile *fin_trk_inef = new TFile("histos/"+observable+"_trk_inef_inclusive.root");
    TFile *fin_model_unc = new TFile("histos/"+observable+"_model_unc_inclusive.root");
    TFile *fin_stat_unc = new TFile("histos/"+observable+"_stat_unc_inclusive.root");
    TFile *fin_mc_stat_unc = new TFile("histos/"+observable+"_mc_stat_unc_inclusive.root");

    int nbins_pt = 3;
    
    TCanvas *c_unc = new TCanvas("c_unc", "", 800, 600);
    // TPad *pad11 = new TPad("pad11", "", 0., 0., 0.33, 1.);
    TPad *pad12 = new TPad("pad12", "", 0., 0., 1., 1.);
    // TPad *pad13 = new TPad("pad13", "", 0.66, 0., 0.99, 1.);

    TPad *pad11, *pad13;

    std::vector<TPad *> pads = {pad11, pad12, pad13};

    TString fout_name = "histos/total_unc_incl_"+observable+".root";
    std::cout << "Creating file: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (int ibin_pt=1; ibin_pt<=nbins_pt; ibin_pt++) {
        if (ibin_pt!=2) continue;
        int ipad = ibin_pt - 1;

        TLegend *leg = new TLegend(0.2, 0.6, 0.6, 0.9);
        leg->SetFillStyle(0);
        leg->SetBorderSize(0);
        leg->SetMargin(0.15);
        // leg->SetHeader(Form("jet pt bin %d", ibin_pt));
        leg->SetHeader("100 < p_{T}^{jet} < 120 (GeV)");

        TH1D *h_stat_unc_rel = (TH1D *) fin_stat_unc->Get(Form("h_stat_unc_rel_%d", ibin_pt))->Clone(Form("h_stat_unc_rel_%d", ibin_pt));
        h_stat_unc_rel->SetLineColor(kBlack);
        h_stat_unc_rel->SetFillStyle(3244);
        h_stat_unc_rel->SetMarkerStyle(1);
        h_stat_unc_rel->GetYaxis()->SetTitle("relative uncertainty");
        h_stat_unc_rel->GetYaxis()->SetTitleOffset(2.5);
        leg->AddEntry(h_stat_unc_rel, "statistical (sym)", "f");

        TH1D *h_stat_unc_rel_down = (TH1D *) h_stat_unc_rel->Clone(Form("h_stat_unc_rel_down_%d", ibin_pt));
        h_stat_unc_rel_down->Scale(-1.);

        std::cout << "after stat " << std::endl;

        TH1D *h_jer_up_rel = (TH1D *) fin_jer->Get(Form("h_unc_up_rel_%d", ibin_pt))->Clone(Form("h_jer_up_rel_%d", ibin_pt));
        h_jer_up_rel->SetMarkerStyle(kFullTriangleUp);
        h_jer_up_rel->SetLineColor(kRed);
        leg->AddEntry(h_jer_up_rel, "JER up", "pl");
        
        TH1D *h_jer_down_rel = (TH1D *) fin_jer->Get(Form("h_unc_down_rel_%d", ibin_pt))->Clone(Form("h_jer_down_rel_%d", ibin_pt));
        h_jer_down_rel->SetMarkerStyle(kFullTriangleDown);
        h_jer_down_rel->SetLineColor(kRed);
        leg->AddEntry(h_jer_down_rel, "JER down", "pl");

        std::cout << "after jer " << std::endl;

        TH1D *h_jec_up_rel = (TH1D *) fin_jec->Get(Form("h_unc_up_rel_%d", ibin_pt))->Clone(Form("h_jec_up_rel_%d", ibin_pt));
        h_jec_up_rel->SetLineColor(kBlue);
        h_jec_up_rel->SetMarkerStyle(kOpenTriangleUp);
        leg->AddEntry(h_jec_up_rel, "JEC up", "pl");

        TH1D *h_jec_down_rel = (TH1D *) fin_jec->Get(Form("h_unc_down_rel_%d", ibin_pt))->Clone(Form("h_jec_down_rel_%d", ibin_pt));
        h_jec_down_rel->SetLineColor(kBlue);
        h_jec_down_rel->SetMarkerStyle(kOpenTriangleDown);
        leg->AddEntry(h_jec_down_rel, "JEC down", "pl");

        std::cout << "after jec " << std::endl;

        TH1D *h_mc_stat_unc_up_rel = (TH1D *) fin_mc_stat_unc->Get(Form("h_mc_stat_unc_up_rel_%d", ibin_pt))->Clone(Form("h_mc_stat_unc_up_rel_%d", ibin_pt));
        h_mc_stat_unc_up_rel->SetLineColor(kGreen);
        h_mc_stat_unc_up_rel->SetMarkerStyle(kOpenDiamond);
        leg->AddEntry(h_mc_stat_unc_up_rel, "response matrix stat (sym)", "pl");

        TH1D *h_mc_stat_unc_down_rel = (TH1D *) h_mc_stat_unc_up_rel->Clone(Form("h_stat_unc_down_rel_%d", ibin_pt));
        h_mc_stat_unc_down_rel->Scale(-1.);

        TH1D *h_trk_inef_up_rel = (TH1D *) fin_trk_inef->Get(Form("h_inef_unc_rel_%d", ibin_pt))->Clone(Form("h_trk_inef_up_rel_%d", ibin_pt));
        h_trk_inef_up_rel->SetLineColor(kMagenta);
        h_trk_inef_up_rel->SetMarkerStyle(kFullCrossX);
        leg->AddEntry(h_trk_inef_up_rel, "3% trk inef (sym)", "pl");

        TH1D *h_trk_inef_down_rel = (TH1D *) h_trk_inef_up_rel->Clone(Form("h_trk_inef_down_rel_%d", ibin_pt));
        h_trk_inef_down_rel->Scale(-1.);

        std::cout << "after trk " << std::endl;

        TH1D *h_model_unc_rel = (TH1D *) fin_model_unc->Get(Form("h_model_unc_rel_%d", ibin_pt))->Clone(Form("h_model_unc_rel_%d", ibin_pt));
        h_model_unc_rel->SetLineColor(kOrange);
        h_model_unc_rel->SetMarkerStyle(kFullStar);
        leg->AddEntry(h_model_unc_rel, "modelling response", "pl");

        // std::vector<TH1D *> histos = {h_jer_up_rel, h_jer_down_rel, h_jec_up_rel, h_jec_down_rel, h_trk_inef_rel, h_model_unc_rel};
        std::vector<TH1D *> histos = {h_jer_up_rel, h_jer_down_rel, h_jec_up_rel, h_jec_down_rel, h_mc_stat_unc_up_rel, h_mc_stat_unc_down_rel, h_trk_inef_up_rel, h_trk_inef_down_rel, h_model_unc_rel};
        
        std::vector<TH1D *> all_histos = {h_stat_unc_rel, h_stat_unc_rel_down};
        all_histos.insert(all_histos.end(), histos.begin(), histos.end());
        TH1D *h_total_unc_up = (TH1D *) h_stat_unc_rel->Clone(Form("h_total_unc_up_%d", ibin_pt));
        h_total_unc_up->Reset();
        TH1D *h_total_unc_down = (TH1D *) h_total_unc_up->Clone(Form("h_total_unc_down_%d", ibin_pt));
        for (int ibin_x = 1; ibin_x <= h_total_unc_up->GetNbinsX(); ibin_x++) {
            double total_unc_up = 0;
            double total_unc_down = 0;

            for (auto h : all_histos) {
                double unc_h = h->GetBinContent(ibin_x);
                if (unc_h>0) {
                    total_unc_up += unc_h*unc_h;
                } else {
                    total_unc_down += unc_h*unc_h;
                }
            }

            total_unc_up = std::sqrt(total_unc_up);
            total_unc_down = std::sqrt(total_unc_down);
            total_unc_down *= -1;

            h_total_unc_up->SetBinContent(ibin_x, total_unc_up);
            h_total_unc_down->SetBinContent(ibin_x, total_unc_down);
        }
        h_total_unc_up->Write();
        h_total_unc_down->Write();

        leg->AddEntry(h_total_unc_up, "total unc.", "f");

        double ymax=0.2;
        double ymin=-0.2;

        pads[ipad]->cd();

        for (auto h : {h_total_unc_up, h_total_unc_down}) {
            h->GetYaxis()->SetRangeUser(ymin, ymax);
            h->SetFillColor(kGray);
            h->SetLineColor(kGray);
            h->SetMarkerColor(kGray);
            h->SetMarkerStyle(1);
            h->SetFillStyle(1001);
            h->GetXaxis()->SetTitle(xlabel);
            h->Draw("hist same");
        }

        for (auto h : {h_stat_unc_rel, h_stat_unc_rel_down}) {
            h->GetYaxis()->SetRangeUser(ymin, ymax);
            h->SetFillColor(h->GetLineColor());
            h->SetMarkerColor(h->GetLineColor());
            h->GetXaxis()->SetTitle(xlabel);
            h->Draw("hist same");
        }

        TLine *line = new TLine(h_jer_up_rel->GetXaxis()->GetBinLowEdge(1), 0, h_jer_up_rel->GetXaxis()->GetBinUpEdge(h_jer_up_rel->GetNbinsX()), 0);
        // line->SetLineStyle(kDashed);
        line->Draw();

        for (int i=0; i<histos.size(); i++) {
            TH1D *h = histos[i];
            h->GetYaxis()->SetRangeUser(ymin, ymax);
            h->SetMarkerColor(h->GetLineColor());
            h->SetLineWidth(1);
            h->SetLineStyle(kDashed);
            h->SetMarkerSize(2);
            h->GetYaxis()->SetTitle("relative uncertainty");
            h->GetYaxis()->SetTitleOffset(2);
            h->Draw("p l hist same");
        }
        // h_stat_unc_rel->Draw("pe2 same");
        leg->Draw();
        drawHeader();

        for (int ibin_x=1; ibin_x<h_jer_up_rel->GetNbinsX(); ibin_x++) {
            double x = h_jer_up_rel->GetXaxis()->GetBinUpEdge(ibin_x);
            TLine *line_x = new TLine(x, ymin, x, ymax);
            line_x->SetLineColor(kGray);
            line_x->SetLineStyle(kDashed);
            line_x->Draw();
        }

        double x1 = h_jer_up_rel->GetXaxis()->GetBinLowEdge(h_jer_up_rel->GetNbinsX());
        double x2 = h_jer_up_rel->GetXaxis()->GetBinUpEdge(h_jer_up_rel->GetNbinsX());
        double y1 = ymin;
        double y2 = ymax;
        TPaveText *overflow = new TPaveText(x1, y1, x2, y2);
        overflow->SetBorderSize(1);
        overflow->SetFillStyle(3644);
        auto overflow_txt = overflow->AddText("overflow");
        overflow_txt->SetTextAngle(90);
        overflow_txt->SetTextSize(20);
        // if (observable=="rg") overflow->Draw();

        c_unc->cd();
        pads[ipad]->Draw();
    }

    c_unc->Draw();
    c_unc->Print("plots_an/total_unc_incl_"+observable+".png");
}