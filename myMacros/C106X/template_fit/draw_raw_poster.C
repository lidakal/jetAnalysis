#include "../myPalette.h"

void draw_raw_poster()
{
    Float_t font_size = 32.;
    gStyle->SetLegendTextSize(font_size);
    gStyle->SetTextSize(font_size);
    gStyle->SetLabelSize(font_size, "XYZ");
    gStyle->SetTitleSize(font_size, "XYZ");
    gStyle->SetErrorX(0.5);
    // mykGreen = kTeal+4;


    // Load data btag
    TString label_data_btag = "aggrTMVA_v2";
    TString fin_name_data_btag = "./histos/data_" + label_data_btag + "_histograms.root";
    TFile *fin_data_btag = new TFile(fin_name_data_btag);
    TH3D *h_data_btag = (TH3D *) fin_data_btag->Get("h_data")->Clone("h_data_btag");

    // Load data inclusive
    TString label_data_incl = "aggrTMVA_v2_inclusive";
    TString fin_name_data_incl = "./histos/data_" + label_data_incl + "_histograms.root";
    TFile *fin_data_incl = new TFile(fin_name_data_incl);
    TH3D *h_data_incl = (TH3D *) fin_data_incl->Get("h_data")->Clone("h_data_incl");

    // Load dijet MC btag
    TString fin_name_mc_dijet_btag = "./histos/dijet_aggrTMVA_withHLT_v2_histograms.root";
    TFile *fin_mc_dijet_btag = new TFile(fin_name_mc_dijet_btag);

    TH3D *h_sig_training_dijet_btag = (TH3D *) fin_mc_dijet_btag->Get("h_sig_training")->Clone("h_sig_training_dijet_btag");
    TH3D *h_bkg_bb_training_dijet_btag = (TH3D *) fin_mc_dijet_btag->Get("h_bkg_bb_training")->Clone("h_bkg_bb_training_dijet_btag");
    TH3D *h_bkg_rest_training_dijet_btag = (TH3D *) fin_mc_dijet_btag->Get("h_bkg_rest_training")->Clone("h_bkg_rest_training_dijet_btag");

    TH3D *h_sig_testing_dijet_btag = (TH3D *) fin_mc_dijet_btag->Get("h_sig_testing")->Clone("h_sig_testing_dijet_btag");
    TH3D *h_bkg_bb_testing_dijet_btag = (TH3D *) fin_mc_dijet_btag->Get("h_bkg_bb_testing")->Clone("h_bkg_bb_testing_dijet_btag");
    TH3D *h_bkg_rest_testing_dijet_btag = (TH3D *) fin_mc_dijet_btag->Get("h_bkg_rest_testing")->Clone("h_bkg_rest_testing_dijet_btag");

    // Load dijet MC inclusive
    TString fin_name_mc_dijet_incl = "./histos/dijet_aggrTMVA_withHLT_v2_inclusive_histograms.root";
    TFile *fin_mc_dijet_incl = new TFile(fin_name_mc_dijet_incl);

    TH3D *h_sig_training_dijet_incl = (TH3D *) fin_mc_dijet_incl->Get("h_sig_training")->Clone("h_sig_training_dijet_incl");
    TH3D *h_bkg_bb_training_dijet_incl = (TH3D *) fin_mc_dijet_incl->Get("h_bkg_bb_training")->Clone("h_bkg_bb_training_dijet_incl");
    TH3D *h_bkg_rest_training_dijet_incl = (TH3D *) fin_mc_dijet_incl->Get("h_bkg_rest_training")->Clone("h_bkg_rest_training_dijet_incl");

    TH3D *h_sig_testing_dijet_incl = (TH3D *) fin_mc_dijet_incl->Get("h_sig_testing")->Clone("h_sig_testing_dijet_incl");
    TH3D *h_bkg_bb_testing_dijet_incl = (TH3D *) fin_mc_dijet_incl->Get("h_bkg_bb_testing")->Clone("h_bkg_bb_testing_dijet_incl");
    TH3D *h_bkg_rest_testing_dijet_incl = (TH3D *) fin_mc_dijet_incl->Get("h_bkg_rest_testing")->Clone("h_bkg_rest_testing_dijet_incl");

    // Compile MC reco
    TH3D *h_mc_btag = (TH3D *) h_sig_training_dijet_btag->Clone("h_mc");
    h_mc_btag->Sumw2();
    for (auto h : {h_sig_testing_dijet_btag,
                   h_bkg_bb_training_dijet_btag, h_bkg_bb_testing_dijet_btag,
                   h_bkg_rest_training_dijet_btag, h_bkg_rest_testing_dijet_btag}) {
                    h_mc_btag->Add(h);
                   } 

    TH3D *h_mc_incl = (TH3D *) h_sig_training_dijet_incl->Clone("h_mc");
    h_mc_incl->Sumw2();
    for (auto h : {h_sig_testing_dijet_incl,
                   h_bkg_bb_training_dijet_incl, h_bkg_bb_testing_dijet_incl,
                   h_bkg_rest_training_dijet_incl, h_bkg_rest_testing_dijet_incl}) {
                    h_mc_incl->Add(h);
                   } 
    
    // Make projections 
    Int_t nbins_rg = h_data_btag->GetNbinsX();
    Int_t nbins_mb = h_data_btag->GetNbinsY();
    Int_t nbins_pt = h_data_btag->GetNbinsZ();

    TH1D *h_data_btag_rg = (TH1D *) h_data_btag->ProjectionX("h_data_btag_rg", 1, nbins_mb, 1, nbins_pt);
    TH1D *h_data_btag_mb = (TH1D *) h_data_btag->ProjectionY("h_data_btag_mb", 1, nbins_rg, 1, nbins_pt);
    TH1D *h_data_btag_pt = (TH1D *) h_data_btag->ProjectionZ("h_data_btag_pt", 1, nbins_rg, 1, nbins_mb);

    TH1D *h_data_incl_rg = (TH1D *) h_data_incl->ProjectionX("h_data_incl_rg", 1, nbins_mb, 1, nbins_pt);
    TH1D *h_data_incl_mb = (TH1D *) h_data_incl->ProjectionY("h_data_incl_mb", 1, nbins_rg, 1, nbins_pt);
    TH1D *h_data_incl_pt = (TH1D *) h_data_incl->ProjectionZ("h_data_incl_pt", 1, nbins_rg, 1, nbins_mb);

    TH1D *h_mc_btag_rg = (TH1D *) h_mc_btag->ProjectionX("h_mc_btag_rg", 1, nbins_mb, 1, nbins_pt);
    TH1D *h_mc_btag_mb = (TH1D *) h_mc_btag->ProjectionY("h_mc_btag_mb", 1, nbins_rg, 1, nbins_pt);
    TH1D *h_mc_btag_pt = (TH1D *) h_mc_btag->ProjectionZ("h_mc_btag_pt", 1, nbins_rg, 1, nbins_mb);

    TH1D *h_mc_incl_rg = (TH1D *) h_mc_incl->ProjectionX("h_mc_incl_rg", 1, nbins_mb, 1, nbins_pt);
    TH1D *h_mc_incl_mb = (TH1D *) h_mc_incl->ProjectionY("h_mc_incl_mb", 1, nbins_rg, 1, nbins_pt);
    TH1D *h_mc_incl_pt = (TH1D *) h_mc_incl->ProjectionZ("h_mc_incl_pt", 1, nbins_rg, 1, nbins_mb);

    // Get choose bin range in rg
    for (auto h : {h_data_btag_rg, h_data_incl_rg,
                   h_mc_btag_rg, h_mc_incl_rg}) {
                    h->GetXaxis()->SetRange(2, 7);
                   }

    // Normalize histograms
    for (auto h : {h_data_btag_rg, h_data_btag_mb, h_data_btag_pt,
                   h_data_incl_rg, h_data_incl_mb, h_data_incl_pt,
                   h_mc_btag_rg, h_mc_btag_mb, h_mc_btag_pt,
                   h_mc_incl_rg, h_mc_incl_mb, h_mc_incl_pt,
                   }) {
                    h->Scale(1/h->Integral("width"));
                    // std::cout << "error bin 3 = " << h->GetBinError(3) << std::endl;
                   }

    // Fix untagged bin in rg 
    h_data_btag_rg->SetBinContent(1, h_data_btag_rg->GetBinContent(1) / 10);
    h_data_incl_rg->SetBinContent(1, h_data_incl_rg->GetBinContent(1) / 10);
    h_mc_btag_rg->SetBinContent(1, h_mc_btag_rg->GetBinContent(1) / 10);
    h_mc_incl_rg->SetBinContent(1, h_mc_btag_rg->GetBinContent(1) / 10);

    // Format + draw histograms 
    THStack *h_rg = new THStack("h_rg", "");
    h_rg->SetTitle("; ln(0.4/R_{g}); 1/N_{jets} dN / d(ln(0.4/R_{g}))");
    // h_rg->SetMaximum(0.5);

    TLegend *leg_rg = new TLegend(0.5, 0.65, 0.95, 0.85); 
    leg_rg->SetFillStyle(0);

    THStack *h_mb = new THStack("h_mb", "");
    h_mb->SetTitle("; m_{B}^{ch}; 1/N_{jets} dN / d(m_{B}^{ch})");

    TLegend *leg_mb = new TLegend(0.5, 0.65, 0.9, 0.85); 
    leg_mb->SetFillStyle(0);

    THStack *h_pt = new THStack("h_pt", "");
    h_pt->SetTitle("; p_{T}^{jet}; 1/N_{jets} dN / d(p_{T}^{jet})");

    TLegend *leg_pt = new TLegend(0.5, 0.65, 0.9, 0.85); 
    leg_pt->SetFillStyle(0);

    TCanvas *c_rg = new TCanvas("c_rg", "", 1200, 1000);
    TPad *pad1_rg = new TPad("pad1_rg", "", 0., 0., 1., 0.3);
    TPad *pad2_rg = new TPad("pad2_rg", "", 0., 0.3, 1., 1.);

    TCanvas *c_mb = new TCanvas("c_mb", "", 1200, 1000);
    TPad *pad1_mb = new TPad("pad1_mb", "", 0., 0., 1., 0.3);
    TPad *pad2_mb = new TPad("pad2_mb", "", 0., 0.3, 1., 1.);

    TCanvas *c_pt = new TCanvas("c_pt", "", 1200, 1000);
    TPad *pad1_pt = new TPad("pad1_pt", "", 0., 0., 1., 0.3);
    TPad *pad2_pt = new TPad("pad2_pt", "", 0., 0.3, 1., 1.);

    // Make decorations
    TPaveText *info_top_left = new TPaveText(0., 1., 0.4, 1.1, "nb ndc");
    info_top_left->SetTextSize(font_size);
    info_top_left->SetFillStyle(0);
    info_top_left->SetLineWidth(0);
    info_top_left->AddText("#bf{CMS} #it{Internal}");

    TPaveText *info_top_right = new TPaveText(1.3, 1., 1.8, 1.1, "nb ndc");
    info_top_right->SetTextSize(font_size);
    info_top_right->SetFillStyle(0);
    info_top_right->SetLineWidth(0);
    info_top_right->AddText("#sqrt{s} = 5.02 TeV #it{pp}");

    TPaveText *info_jets = new TPaveText(-0.4, 0.47, 0.05, 0.49, "nb ndc");
    info_jets->SetTextSize(font_size);
    info_jets->SetFillStyle(0);
    info_jets->SetLineWidth(0);
    // info_jets->AddText("b-tagged b-jets ");
    // info_jets->AddText("80 < #it{p}_{T}^{jet} < 100 GeV");
    info_jets->AddText("-2 < #it{#eta}^{jet} < 2, #it{k}_{T} > 1 GeV");
    // info_jets->AddText("");
    // info_jets->AddText("#it{k}_{T} > 1 GeV");

    std::vector<THStack *> stacks = {h_rg, h_mb, h_pt};
    std::vector<TLegend *> legends = {leg_rg, leg_mb, leg_pt};
    std::vector<TH1D *> data_btag_hists = {h_data_btag_rg, h_data_btag_mb, h_data_btag_pt};
    std::vector<TH1D *> data_incl_hists = {h_data_incl_rg, h_data_incl_mb, h_data_incl_pt};
    std::vector<TH1D *> mc_btag_hists = {h_mc_btag_rg, h_mc_btag_mb, h_mc_btag_pt};
    std::vector<TH1D *> mc_incl_hists = {h_mc_incl_rg, h_mc_incl_mb, h_mc_incl_pt};
    std::vector<TCanvas *> canvases = {c_rg, c_mb, c_pt};
    std::vector<TPad *> pads1 = {pad1_rg, pad1_mb, pad1_pt};
    std::vector<TPad *> pads2 = {pad2_rg, pad2_mb, pad2_pt};
    std::vector<TString> xtitles = {"ln(0.4/R_{g})", "m_{B}^{ch}", "p_{T}^{jet}"};

    for (size_t i = 0; i < stacks.size(); i++) {
        data_incl_hists[i]->SetMarkerStyle(kOpenCircle);
        data_incl_hists[i]->SetMarkerSize(2);
        data_incl_hists[i]->SetMarkerColor(mykBlue);
        data_incl_hists[i]->SetLineColor(mykBlue);
        stacks[i]->Add(data_incl_hists[i], "pe");
        legends[i]->AddEntry(data_incl_hists[i], "inclusive raw data", "pe");

        // data_btag_hists[i]->SetMarkerStyle(kFullDiamond);
        // data_btag_hists[i]->SetMarkerSize(3);
        // data_btag_hists[i]->SetMarkerColor(mykBlue);
        // data_btag_hists[i]->SetLineColor(mykBlue);
        data_btag_hists[i]->SetMarkerStyle(kFullCircle);
        data_btag_hists[i]->SetMarkerSize(2);
        data_btag_hists[i]->SetMarkerColor(kBlack);
        data_btag_hists[i]->SetLineColor(kBlack);
        stacks[i]->Add(data_btag_hists[i], "pe");
        legends[i]->AddEntry(data_btag_hists[i], "b-tagged raw data", "pe");

        mc_incl_hists[i]->SetMarkerStyle(kOpenTriangleUp);
        mc_incl_hists[i]->SetMarkerSize(2);
        mc_incl_hists[i]->SetMarkerColor(mykBlue);
        mc_incl_hists[i]->SetLineColor(mykBlue);
        stacks[i]->Add(mc_incl_hists[i], "pe");
        legends[i]->AddEntry(mc_incl_hists[i], "inclusive MC", "pe");

        mc_btag_hists[i]->SetMarkerStyle(kFullTriangleUp);
        mc_btag_hists[i]->SetMarkerSize(2);
        mc_btag_hists[i]->SetMarkerColor(kBlack);
        mc_btag_hists[i]->SetLineColor(kBlack);
        stacks[i]->Add(mc_btag_hists[i], "pe");
        legends[i]->AddEntry(mc_btag_hists[i], "b-tagged MC", "pe");

        // Find maximum for plotting 
        double ymax = 0.;
        for (auto h : {data_incl_hists[i], data_btag_hists[i],
                       mc_incl_hists[i], mc_btag_hists[i]}) {
            double hmax = h->GetMaximum();
            if (hmax > ymax) ymax = hmax;
        }

        // RATIO PLOTS
        THStack *h_ratio = new THStack("h_ratio", "");
        std::string xtitle = data_btag_hists[i]->GetXaxis()->GetTitle();
        h_ratio->SetTitle(Form("; %s; b-tagged / inclusive", xtitle.c_str()));

        TH1D *h_data_ratio = (TH1D *) data_btag_hists[i]->Clone("h_data_ratio");
        h_data_ratio->Divide(data_incl_hists[i]);
        h_data_ratio->SetLineColor(data_btag_hists[i]->GetLineColor());
        h_ratio->Add(h_data_ratio, "pe1");

        TH1D *h_mc_ratio = (TH1D *) mc_btag_hists[i]->Clone("h_mc_ratio");
        h_mc_ratio->Divide(mc_incl_hists[i]);
        h_mc_ratio->SetLineColor(mc_btag_hists[i]->GetLineColor());
        h_ratio->Add(h_mc_ratio, "pe1");

        // Find maximum for plotting 
        double ymin_ratio = 1.;
        double ymax_ratio = 0.;
        for (auto h : {h_data_ratio, h_mc_ratio}) {
            double hmax = h->GetMaximum();
            double hmin = h->GetMinimum();
            if (hmax > ymax_ratio) ymax_ratio = hmax;
            if (hmin < ymin_ratio) ymin_ratio = hmin;
        }

        pads2[i]->SetBottomMargin(0.02);
        pads1[i]->SetTopMargin(0.02);
        pads1[i]->SetBottomMargin(0.4);

        pads1[i]->SetRightMargin(0.05);
        pads2[i]->SetRightMargin(0.05);

        pads1[i]->SetLeftMargin(0.15);
        pads2[i]->SetLeftMargin(0.15);
        
        pads2[i]->cd();
        // stacks[i]->Draw("nostack");
        double xLabelSize = data_incl_hists[i]->GetXaxis()->GetLabelSize();
        for (auto h : {data_incl_hists[i], data_btag_hists[i],
                       mc_incl_hists[i], mc_btag_hists[i]}) {
            h->Draw("pe1 same");

            h->GetXaxis()->SetLabelSize(0.);
            
            TString ytitle = "1/N_{2-prong jets} dN/d(" + xtitles[i] + ")";
            h->GetYaxis()->SetTitle(ytitle);
            h->GetYaxis()->SetRangeUser(0., 1.1 * ymax);

        }
        legends[i]->Draw();
        info_top_left->Draw();
        info_top_right->Draw();
        

        pads1[i]->cd();
        for (auto h : {h_data_ratio, h_mc_ratio}) {
            h->Draw("pe1 same");

            h->GetXaxis()->SetLabelSize(xLabelSize);
            h->GetXaxis()->SetTitle(xtitles[i]);
            h->GetXaxis()->SetTitleOffset(4.);

            h->GetYaxis()->SetNdivisions(6);
            h->GetYaxis()->SetRangeUser(0.95 * ymin_ratio, 1.05 * ymax_ratio);
            h->GetYaxis()->SetTitle("b-tag/inclusive");
        }
        // h_ratio->Draw("nostack");


        canvases[i]->cd();
        
        pads2[i]->Draw();
        pads1[i]->Draw();
        TString canvas_name = "./plots/raw_" + TString(canvases[i]->GetName()) + ".png";
        canvases[i]->Print(canvas_name);
    }
}