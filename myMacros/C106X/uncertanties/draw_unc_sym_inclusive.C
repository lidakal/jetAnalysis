#include "../draw_utils.h"
#include "../cms_palette.h"

void draw_unc_sym_inclusive(TString observable="rg")
{
    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch} #equiv p_{T}^{B,ch}/p_{T}^{jet,ch}";

    // Setup
    gStyle->SetCanvasPreferGL(kTRUE);
    gStyle->SetErrorX(0.5);
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    TFile *fin_jer = new TFile("histos/"+observable+"_jer_unc_inclusive.root");
    TFile *fin_jec = new TFile("histos/"+observable+"_jec_unc_src_inclusive.root");
    TFile *fin_trk_inef = new TFile("histos/"+observable+"_trk_inef_inclusive.root");
    TFile *fin_model_unc = new TFile("histos/"+observable+"_model_unc_inclusive.root");
    TFile *fin_stat_unc = new TFile("histos/"+observable+"_stat_unc_inclusive.root");
    TFile *fin_mc_stat_unc = new TFile("histos/"+observable+"_mc_stat_unc_inclusive.root");
    TFile *fin_pythia_var = new TFile("histos/"+observable+"_pythia_var_inclusive.root");

    int nbins_pt = 3;
    
    TCanvas *c_unc = new TCanvas("c_unc", "", 1400,600);
    c_unc->SetRightMargin(0.03);
    c_unc->SetLeftMargin(0.08);
    c_unc->SetBottomMargin(0.25);

    TString fout_name = "histos/total_unc_incl_"+observable+".root";
    std::cout << "Creating file: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    int ibin_pt = 2;

    TH1D *h_stat_unc_rel = (TH1D *) fin_stat_unc->Get(Form("h_stat_unc_rel_%d", ibin_pt))->Clone(Form("h_stat_unc_rel_%d", ibin_pt));
    h_stat_unc_rel->SetLineColor(kBlack);
    h_stat_unc_rel->SetFillStyle(3244);
    h_stat_unc_rel->SetFillColorAlpha(kBlack, 0.5);
    h_stat_unc_rel->SetMarkerStyle(1);
    h_stat_unc_rel->GetYaxis()->SetTitle("Relative uncertainty");
    h_stat_unc_rel->GetYaxis()->SetTitleOffset(2.);
    h_stat_unc_rel->SetLineWidth(3);
    h_stat_unc_rel->SetLineStyle(1);

    TH1D *h_jer_up_rel = (TH1D *) fin_jer->Get(Form("h_unc_rel_%d", ibin_pt))->Clone(Form("h_jer_up_rel_%d", ibin_pt));
    h_jer_up_rel->SetMarkerStyle(kFullTriangleUp);
    h_jer_up_rel->SetLineColor(cmsBlue);
    h_jer_up_rel->SetLineWidth(3);
    h_jer_up_rel->SetLineStyle(2);

    TH1D *h_jec_up_rel = (TH1D *) fin_jec->Get(Form("h_total_unc_rel_sym_%d", ibin_pt))->Clone(Form("h_jec_up_rel_%d", ibin_pt));
    h_jec_up_rel->SetLineColor(cmsViolet);
    h_jec_up_rel->SetMarkerStyle(kFullTriangleDown);
    h_jec_up_rel->SetLineWidth(3);
    h_jec_up_rel->SetLineStyle(3);

    TH1D *h_mc_stat_unc_up_rel = (TH1D *) fin_mc_stat_unc->Get(Form("h_mc_stat_unc_up_rel_%d", ibin_pt))->Clone(Form("h_mc_stat_unc_up_rel_%d", ibin_pt));
    h_mc_stat_unc_up_rel->SetFillStyle(3154);
    h_mc_stat_unc_up_rel->SetFillColorAlpha(kBlack,1.);
    h_mc_stat_unc_up_rel->SetLineColorAlpha(kBlack,0.);
    h_mc_stat_unc_up_rel->SetMarkerColorAlpha(kBlack,0.);

    TH1D *h_trk_inef_up_rel = (TH1D *) fin_trk_inef->Get(Form("h_inef_unc_rel_%d", ibin_pt))->Clone(Form("h_trk_inef_up_rel_%d", ibin_pt));
    h_trk_inef_up_rel->SetLineColor(cmsLightBlue);
    h_trk_inef_up_rel->SetMarkerStyle(kFullCrossX);
    h_trk_inef_up_rel->SetLineWidth(3);
    h_trk_inef_up_rel->SetLineStyle(4);    

    TH1D *h_model_unc_rel = (TH1D *) fin_model_unc->Get(Form("h_model_unc_rel_%d", ibin_pt))->Clone(Form("h_model_unc_rel_%d", ibin_pt));
    // alternative model unc (independent variations)
    // TH1D *h_model_unc_rel = (TH1D *) fin_model_unc->Get("h_model_unc_herwigSum_rel")->Clone(Form("h_model_unc_rel_%d", ibin_pt));
    h_model_unc_rel->SetLineColor(cmsYellow);
    h_model_unc_rel->SetMarkerStyle(kFullStar);
    h_model_unc_rel->SetLineWidth(3);
    h_model_unc_rel->SetLineStyle(5);
    // average neighboring bins at pinching point (rg bin=5)
    std::cout << "before: " << h_model_unc_rel->GetBinContent(5) << std::endl;
    double avg = (std::abs(h_model_unc_rel->GetBinContent(4))+std::abs(h_model_unc_rel->GetBinContent(6))) / 2.;
    h_model_unc_rel->SetBinContent(5, avg);
    std::cout << "after: " << h_model_unc_rel->GetBinContent(5) << std::endl;

    TH1D *h_pythia_var_up_rel = (TH1D *) fin_pythia_var->Get(Form("h_total_unc_rel_sym_%d", ibin_pt))->Clone(Form("h_pythia_var_up_rel_%d", ibin_pt));
    h_pythia_var_up_rel->SetLineColor(cmsRed);
    h_pythia_var_up_rel->SetMarkerStyle(kFullCross);
    h_pythia_var_up_rel->SetLineWidth(3);
    h_pythia_var_up_rel->SetLineStyle(3);

    std::vector<TH1D *> histos_syst = {
        h_mc_stat_unc_up_rel,
        h_jer_up_rel,
        h_jec_up_rel,
        h_trk_inef_up_rel,
        h_model_unc_rel, 
        h_pythia_var_up_rel
    };
    
    std::vector<TH1D *> all_histos = {h_stat_unc_rel};
    all_histos.insert(all_histos.end(), histos_syst.begin(), histos_syst.end());

    // fix x axis
    int nbins_x = h_stat_unc_rel->GetNbinsX();

    int ibin_x_min = 2;
    int ibin_x_max = nbins_x;

    if (observable=="rg") ibin_x_max = nbins_x - 1;

    for (auto h : all_histos) {
        h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
    }

    // ----------- SAVE TOTAL SYST ---------------
    
    TH1D *h_syst_unc_up = (TH1D *) h_stat_unc_rel->Clone(Form("h_syst_unc_up_%d", ibin_pt));
    h_syst_unc_up->Reset();
    for (int ibin_x = 1; ibin_x <= h_syst_unc_up->GetNbinsX(); ibin_x++) {
        double syst_unc_up = 0;

        for (auto h : histos_syst) {
            double unc_h = std::abs(h->GetBinContent(ibin_x));
            h->SetBinContent(ibin_x, unc_h);
            syst_unc_up += unc_h*unc_h;            
        }

        syst_unc_up = std::sqrt(syst_unc_up);
        h_syst_unc_up->SetBinContent(ibin_x, syst_unc_up);
    }
    h_syst_unc_up->Write();

    TH1D *h_syst_unc_down = (TH1D *) h_syst_unc_up->Clone(Form("h_syst_unc_down_%d", ibin_pt));
    h_syst_unc_down->Scale(-1.);
    h_syst_unc_down->Write();

    // ------------------ SAVE TOTAL SYST+STAT -------------------

    TH1D *h_total_unc_up = (TH1D *) h_stat_unc_rel->Clone(Form("h_total_unc_up_%d", ibin_pt));
    h_total_unc_up->Reset();
    for (int ibin_x = 1; ibin_x <= h_total_unc_up->GetNbinsX(); ibin_x++) {
        double total_unc_up = 0;

        for (auto h : all_histos) {
            double unc_h = std::abs(h->GetBinContent(ibin_x));
            h->SetBinContent(ibin_x, unc_h);
            total_unc_up += unc_h*unc_h;            
        }

        total_unc_up = std::sqrt(total_unc_up);
        h_total_unc_up->SetBinContent(ibin_x, total_unc_up);
    }
    h_total_unc_up->Write();

    TH1D *h_total_unc_down = (TH1D *) h_total_unc_up->Clone(Form("h_total_unc_down_%d", ibin_pt));
    h_total_unc_down->Scale(-1.);
    h_total_unc_down->Write();


    //-------------------- LEGENDS -----------

    // For lines/symbols : top left, 2 cols
    
    // TLegend *leg = new TLegend(0.2, 0.62, 0.5, 0.87); // log scale
    TLegend *leg = new TLegend(0.4, 0.65, 0.85, 0.85); // abs scale
    if (observable=="zpt") leg = new TLegend(0.45, 0.5, 0.75, 0.75); // abs scale
    leg->SetNColumns(2);
    leg->SetColumnSeparation(0.25); 
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.25);

    leg->AddEntry(h_stat_unc_rel, "Statistical", "f");
    leg->AddEntry(h_mc_stat_unc_up_rel, "Response matrix stats.", "f");
    leg->AddEntry(h_model_unc_rel, "Shower and hadronization", "pl");
    leg->AddEntry(h_jer_up_rel, "Jet energy resolution", "pl");
    leg->AddEntry(h_pythia_var_up_rel, "FSR and ISR scale", "pl");
    leg->AddEntry(h_jec_up_rel, "Jet energy scale", "pl");
    leg->AddEntry(h_trk_inef_up_rel, "Tracking efficiency", "pl");

    // for log
    // double ymin=0.001;
    // double ymax=1.;

    // for abs
    double ymin=0.;
    double ymax=0.2; // rg
    if (observable=="zg") ymax = 0.1;

    for (auto h : {h_total_unc_up}) {
        h->GetYaxis()->SetRangeUser(ymin, ymax);
        h->SetFillColorAlpha(kGray,0.1);
        h->SetLineColor(kGray);
        h->SetMarkerColor(kGray);
        h->SetMarkerStyle(1);
        h->SetFillStyle(1001);
        h->GetXaxis()->SetTitle(xlabel);
        // h->Draw("hist same");
    }

    for (auto h : {h_stat_unc_rel,
                    }) {
        h->GetYaxis()->SetRangeUser(ymin, ymax);
        // h->SetFillColor(h->GetLineColor());
        h->SetFillColorAlpha(h->GetLineColor(), 0.1);
        h->SetFillStyle(1001);
        h->SetLineWidth(0);
        h->SetLineColorAlpha(kBlack,0);
        h->SetMarkerColor(h->GetLineColor());
        h->GetXaxis()->SetTitle(xlabel);
        h->GetXaxis()->SetTitleOffset(1.2);
        h->Draw("hist same");
    }

    TLine *line = new TLine(h_jer_up_rel->GetXaxis()->GetBinLowEdge(ibin_x_min), 0, h_jer_up_rel->GetXaxis()->GetBinUpEdge(ibin_x_max), 0);
    // line->SetLineStyle(kDashed);
    // line->Draw();
    h_mc_stat_unc_up_rel->Draw("hist same");
    for (int i=1; i<histos_syst.size(); i++) {
        TH1D *h = histos_syst[i];
        h->GetYaxis()->SetRangeUser(ymin, ymax);
        h->SetMarkerColor(h->GetLineColor());
        // h->SetLineWidth(1);
        // h->SetLineStyle(kDashed);
        h->SetMarkerSize(2);
        h->GetYaxis()->SetTitle("Relative uncertainty");
        h->GetYaxis()->SetTitleOffset(2);
        h->Draw("pl hist same");
    }
    // h_stat_unc_rel->Draw("pe2 same");
    leg->Draw();
    drawHeaderUnc();

    for (int ibin_x=1; ibin_x<ibin_x_max; ibin_x++) {
        double x = h_jer_up_rel->GetXaxis()->GetBinUpEdge(ibin_x);
        TLine *line_x = new TLine(x, ymin, x, ymax);
        line_x->SetLineColor(kGray);
        line_x->SetLineStyle(kDashed);
        // line_x->Draw();
    }

    double x1 = h_jer_up_rel->GetXaxis()->GetBinLowEdge(ibin_x_max);
    double x2 = h_jer_up_rel->GetXaxis()->GetBinUpEdge(ibin_x_max);
    double y1 = ymin;
    double y2 = ymax;
    TPaveText *overflow = new TPaveText(x1, y1, x2, y2);
    overflow->SetBorderSize(1);
    overflow->SetFillStyle(3644);
    auto overflow_txt = overflow->AddText("overflow");
    overflow_txt->SetTextAngle(90);
    overflow_txt->SetTextSize(20);
    // if (observable=="rg") overflow->Draw();

    // ----------------- JET INFO ------------

    // bottom, 1 col
    // TLatex *jet_info = new TLatex;
    // jet_info->SetNDC();
    // jet_info->SetTextSize(20);
    // if (observable!="zpt") {
    //     TLatex *jet_info = new TLatex;
    //     jet_info->SetNDC();
    //     jet_info->SetTextSize(20);
    //     jet_info->DrawLatex(0.2, 0.33, "anti-k_{T}, R=0.4 single b jets");
    //     jet_info->DrawLatex(0.2, 0.28, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    //     jet_info->DrawLatex(0.2, 0.23, "Soft drop (charged particles)");
    //     jet_info->DrawLatex(0.2, 0.18, "z_{cut}=0.1, #beta=0, k_{T} > 1 GeV");
    //     jet_info->Draw();
    // } else {
    //     jet_info->DrawLatex(0.2, 0.81, "anti-k_{T}, R=0.4 single b jets");
    //     jet_info->DrawLatex(0.2, 0.76, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    // }

    // top, 1 col 
    TLatex *jet_info = new TLatex;
    jet_info->SetNDC();
    jet_info->SetTextSize(text_size);
    // if (observable=="rg") {
        jet_info->SetTextAlign(12);
        jet_info->DrawLatex(0.12, 0.8, "anti-k_{T}, R = 0.4 inclusive jets");
        jet_info->DrawLatex(0.12, 0.74, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
        jet_info->DrawLatex(0.12, 0.68, "Soft drop (charged particles)");
        jet_info->DrawLatex(0.12, 0.62, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");
    // } else {
    //     jet_info->SetTextAlign(10);
    //     jet_info->DrawLatex(0.53, 0.8, "anti-k_{T}, R=0.4 inclusive jets");
    //     jet_info->DrawLatex(0.53, 0.74, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    //     jet_info->DrawLatex(0.53, 0.68, "Soft drop (charged particles)");
    //     jet_info->DrawLatex(0.53, 0.62, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV");
    // }


    // bottom, 2 cols 
    // TLatex *jet_info = new TLatex;
    // jet_info->SetNDC();
    // jet_info->SetTextSize(20);
    // if (observable!="zpt") {
    //     TLatex *jet_info = new TLatex;
    //     jet_info->SetNDC();
    //     jet_info->SetTextSize(20);
    //     jet_info->DrawLatex(0.2, 0.24, "anti-k_{T}, R=0.4 inclusive jets");
    //     jet_info->DrawLatex(0.2, 0.19, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    //     jet_info->DrawLatex(0.55, 0.24, "Charged soft drop");
    //     jet_info->DrawLatex(0.55, 0.19, "z_{cut}=0.1, #beta=0, k_{T} > 1 GeV");
    //     jet_info->Draw();
    // } else {
    //     jet_info->DrawLatex(0.2, 0.81, "anti-k_{T}, R=0.4 inclusive jets");
    //     jet_info->DrawLatex(0.2, 0.76, "100 < p_{T}^{jet} < 120 GeV, |#eta^{jet}| < 2");
    // }

    if (observable=="rg") {
        auto axis5 = new TGaxis(2.1, -0.045, 0. ,-0.045, 0.048982571, 0.4, 510,"NIGS-");
        axis5->SetTitle("R_{g}");
        axis5->CenterTitle();
        axis5->SetTitleFont(43);
        axis5->SetTitleSize(text_size);
        axis5->SetTitleOffset(0.9);
        axis5->SetLabelFont(43);
        axis5->SetLabelSize(text_size);
        axis5->SetLabelOffset(0.08);
        axis5->SetTickSize(0.02);
        axis5->SetMoreLogLabels(); // add the secondary tick labels
        axis5->SetNoExponent();
        axis5->Draw("same");
    }


    c_unc->Draw();
    // c_unc->SetLogy();
    c_unc->Print("plots_an/total_unc_incl_"+observable+".pdf");
    c_unc->Print("plots_an/total_unc_incl_"+observable+".png");
}