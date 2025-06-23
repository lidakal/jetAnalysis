#include "../cms_palette.h"

void drawHeader(void) {
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


Int_t myPalette(Int_t i) {
    // blue and purple
    // std::vector<std::string> hexColors = {
    //         "#832db6", "#7746bc", "#6b60c2", "#6079c7", "#5492cd",
    //         "#489bd2", "#3fa3d4", "#3f9dd1", "#3f96ce", "#3f90da"
    //     };

    // blue purple and orange
    std::vector<std::string> hexColors = {
        "#3f90da", "#557dcf", "#6a6ac4", "#7f57ba", "#832db6",
        "#9c40a3", "#b55390", "#ce667d", "#e7796b", "#e76300"
    };

    std::vector<Int_t> palette;
    for (const auto& hex : hexColors) {
        palette.push_back(TColor::GetColor(hex.c_str()));
    }

    return palette[i];
}

double rmsValue(std::vector<double> arr)
{
    double square = 0;
    double mean = 0.0, root = 0.0;

    int n = arr.size();
 
    // Calculate square.
    for (int i = 0; i < n; i++) {
        square += pow(arr[i], 2);
    }
 
    // Calculate Mean.
    mean = (square / (double)(n));
 
    // Calculate Root.
    root = sqrt(mean);
 
    return root;
}

double stdValue(std::vector<double> arr, double mean) {
    int n = arr.size();

    double standardDeviation = 0.;
    for(int i = 0; i < n; ++i) {
        standardDeviation += pow(arr[i] - mean, 2);
    }
    return sqrt(standardDeviation / n);
}

void mc_stat_unc(TString observable="rg")
{
    // Setup plot
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z_{b,ch}";
    TString ylabel;
    if (observable=="rg") ylabel = "1/N dN/dln(R/R_{g})";
    else if (observable=="zg") ylabel = "1/N dN/dz_{g}";
    else if (observable=="zpt") ylabel = "1/N dN/dz_{b,ch}";

    // JK RESAMPLING METHOD
    TString sample = "dijet_PF40";
    TString label = "aggrTMVA_inclusive";
    bool is_inclusive = label.Contains("inclusive");

    TString suffix = (is_inclusive) ? "" : "_withSF";

    TString fin_name = "../unfolding/histos/"+sample+"_"+label+"_unfolded_histograms_"+observable+"_jer_nom_jec_nom"+suffix+".root";
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);
    
    TH2D *h_nom = (TH2D *) fin->Get("h_data_unfolded");
    TH2D *h_jk_0, 
        *h_jk_1,
        *h_jk_2,
        *h_jk_3,
        *h_jk_4,
        *h_jk_5,
        *h_jk_6,
        *h_jk_7,
        *h_jk_8,
        *h_jk_9;

    std::vector<TH2D *> histos_jk = {
        h_jk_0,
        h_jk_1,
        h_jk_2,
        h_jk_3,
        h_jk_4,
        h_jk_5,
        h_jk_6,
        h_jk_7,
        h_jk_8,
        h_jk_9,
    };

    for (int i=0; i<10; i++) {
        histos_jk[i] = (TH2D *) fin->Get(Form("h_fully_corrected_jk_%d",i))->Clone(Form("h_jk_%d",i));
    }

    // Make projections 
    Int_t ibin_pt = 2;
    double pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
    double pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);
    TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX("h_nom_1d", ibin_pt, ibin_pt);
    TH1D *h_jk_0_1d, 
        *h_jk_1_1d,
        *h_jk_2_1d,
        *h_jk_3_1d,
        *h_jk_4_1d,
        *h_jk_5_1d,
        *h_jk_6_1d,
        *h_jk_7_1d,
        *h_jk_8_1d,
        *h_jk_9_1d;

    std::vector<TH1D *> histos_jk_1d = {
        h_jk_0_1d,
        h_jk_1_1d,
        h_jk_2_1d,
        h_jk_3_1d,
        h_jk_4_1d,
        h_jk_5_1d,
        h_jk_6_1d,
        h_jk_7_1d,
        h_jk_8_1d,
        h_jk_9_1d,
    };

    for (int i=0; i<10; i++) {
        histos_jk_1d[i] = (TH1D *) histos_jk[i]->ProjectionX(Form("h_jk_%d_1d",i), ibin_pt, ibin_pt);
    }

    // Normalize
    int nbins_x = h_nom->GetNbinsX();
    int ibin_x_min = 1;
    int ibin_x_max = nbins_x;
    if (observable!="zpt") ibin_x_min = 2;
    if (observable=="rg") ibin_x_max = nbins_x - 1;

    h_nom_1d->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
    h_nom_1d->Scale(1/h_nom_1d->Integral(), "width");
    for (int i=0; i<10; i++) {
        histos_jk_1d[i]->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        histos_jk_1d[i]->Scale(1/histos_jk_1d[i]->Integral(), "width");
    }

    // Format histograms
    h_nom_1d->SetLineColor(kBlack);
    h_nom_1d->SetMarkerColor(kBlack);
    h_nom_1d->SetMarkerStyle(kFullCrossX);
    h_nom_1d->SetMarkerSize(2);

    for (int i=0; i<10; i++) {
        histos_jk_1d[i]->SetLineColor(myPalette(i)); 
        histos_jk_1d[i]->SetLineWidth(2);
    }

    // Calculate uncertainties
    TH1D *h_unc_up = (TH1D *) h_nom_1d->Clone("h_unc_up");
    h_unc_up->Reset();
    for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
        std::vector<double> jk_contents;
        for (int i=0; i<10; i++) {
            jk_contents.push_back(histos_jk_1d[i]->GetBinContent(ibin_x));
        }
        // use nominal value as mean
        double nom_val = h_nom_1d->GetBinContent(ibin_x); 
        double std_val = stdValue(jk_contents, nom_val);
        std_val *= 10./9.;

        h_unc_up->SetBinContent(ibin_x, std_val);
    }
    TH1D *h_unc_up_rel = (TH1D *) h_unc_up->Clone("h_unc_up_rel");
    h_unc_up_rel->Divide(h_nom_1d);

    TH1D *h_unc_rel_sym_up = (TH1D *) h_unc_up_rel->Clone("h_unc_rel_sym_up"); 
    h_unc_rel_sym_up->SetFillColor(kGray);
    h_unc_rel_sym_up->SetFillStyle(1001);
    h_unc_rel_sym_up->SetLineColor(kGray);
    TH1D *h_unc_rel_sym_down = (TH1D *) h_unc_rel_sym_up->Clone("h_unc_rel_sym_down");
    h_unc_rel_sym_down->Scale(-1.);

    TH1D *h_jk_0_rel_unc, 
        *h_jk_1_rel_unc,
        *h_jk_2_rel_unc,
        *h_jk_3_rel_unc,
        *h_jk_4_rel_unc,
        *h_jk_5_rel_unc,
        *h_jk_6_rel_unc,
        *h_jk_7_rel_unc,
        *h_jk_8_rel_unc,
        *h_jk_9_rel_unc;

    std::vector<TH1D *> histos_jk_rel_unc = {
        h_jk_0_rel_unc,
        h_jk_1_rel_unc,
        h_jk_2_rel_unc,
        h_jk_3_rel_unc,
        h_jk_4_rel_unc,
        h_jk_5_rel_unc,
        h_jk_6_rel_unc,
        h_jk_7_rel_unc,
        h_jk_8_rel_unc,
        h_jk_9_rel_unc,
    };
    for (int i=0; i<10; i++) {
        histos_jk_rel_unc[i] = (TH1D *) histos_jk_1d[i]->Clone(Form("h_jk_%d_rel_unc",i));
        histos_jk_rel_unc[i]->Add(h_nom_1d, -1.);
        histos_jk_rel_unc[i]->Divide(h_nom_1d);
    }

    // Legend 
    TLegend *leg = new TLegend(0.5, 0.2, 0.9, 0.5);
    if (observable=="zpt") leg = new TLegend(0.2, 0.3, 0.6, 0.6);
    leg->SetNColumns(2);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.15);
    leg->AddEntry(h_nom_1d, "Nominal", "pe1");  
    TH1D *h_ghost = (TH1D *) h_nom_1d->Clone("h_ghost");
    h_ghost->SetLineColor(kWhite);
    leg->AddEntry(h_ghost, " ", "l");
    for (int i=0; i<10; i++) {
        leg->AddEntry(histos_jk_1d[i], Form("Variation %d", i+1), "l");
    }
    
    // Draw
    TCanvas *c_unc = new TCanvas("c_unc", "", 700, 600);
    TPad *top_pad = new TPad("top_pad", "", 0., 0.33, 1., 1.);
    TPad *bottom_pad = new TPad("top_pad", "", 0., 0., 1., 0.33);

    top_pad->SetLeftMargin(0.15);
    top_pad->SetRightMargin(0.05);
    top_pad->SetTopMargin(0.09);
    top_pad->SetBottomMargin(0.03);
    bottom_pad->SetLeftMargin(0.15);
    bottom_pad->SetRightMargin(0.05);
    bottom_pad->SetTopMargin(0.04);
    bottom_pad->SetBottomMargin(0.35);

    top_pad->cd();
    h_nom_1d->GetYaxis()->SetTitle(ylabel);
    h_nom_1d->GetYaxis()->SetTitleSize(text_size);
    h_nom_1d->GetYaxis()->SetLabelSize(text_size-4);
    h_nom_1d->GetYaxis()->SetTitleOffset(1.3);
    if (!is_inclusive&&observable=="rg") h_nom_1d->GetYaxis()->SetRangeUser(0., 3.);
    else if (is_inclusive&&observable=="rg") h_nom_1d->GetYaxis()->SetRangeUser(0., 5.);
    else if (!is_inclusive&&observable=="zg") h_nom_1d->GetYaxis()->SetRangeUser(0., 12.);
    else if (is_inclusive&&observable=="zg") h_nom_1d->GetYaxis()->SetRangeUser(0., 15.);
    else if (observable=="zpt") h_nom_1d->GetYaxis()->SetRangeUser(0., 4.);
    h_nom_1d->GetXaxis()->SetLabelOffset(10.); // kick it out of existence
    h_nom_1d->GetXaxis()->SetTitleOffset(10.); // kick it out of existence
    h_nom_1d->Draw("pe1");
    for (int i=0; i<10; i++) {
        histos_jk_1d[i]->Draw("hist same");
    }
    h_nom_1d->Draw("pe1 same");
    drawHeader();
    leg->Draw();
    // Jets text
    if (observable!="zpt") {
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(32);
        if (is_inclusive) jet_info->DrawLatex(0.9, 0.83, "anti-k_{T}, R = 0.4 inclusive jets"); 
        else jet_info->DrawLatex(0.9, 0.83, "anti-k_{T}, R = 0.4 b jets");
        jet_info->DrawLatex(0.9, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
        jet_info->DrawLatex(0.9, 0.67, "Soft drop (charged particles)");
        jet_info->DrawLatex(0.9, 0.59, "z_{cut} = 0.1, #beta = 0, k_{T} > 1 GeV/c");
    } else {
        TLatex *jet_info = new TLatex;
        jet_info->SetNDC();
        jet_info->SetTextSize(text_size-4);
        jet_info->SetTextAlign(12);
        jet_info->DrawLatex(0.2, 0.83, "anti-k_{T}, R = 0.4 b jets");
        jet_info->DrawLatex(0.2, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
    }


    bottom_pad->cd();
    h_unc_rel_sym_up->GetXaxis()->SetTitle(xlabel);
    h_unc_rel_sym_up->GetXaxis()->SetTitleSize(text_size);
    h_unc_rel_sym_up->GetXaxis()->SetLabelSize(text_size-4);
    h_unc_rel_sym_up->GetXaxis()->SetTitleOffset(3.);
    h_unc_rel_sym_up->GetYaxis()->SetTitle("#splitline{Relative}{uncertainty}");
    h_unc_rel_sym_up->GetYaxis()->SetTitleSize(text_size);
    h_unc_rel_sym_up->GetYaxis()->SetLabelSize(text_size-4);
    h_unc_rel_sym_up->GetYaxis()->SetTitleOffset(1.4);
    if (is_inclusive) h_unc_rel_sym_up->GetYaxis()->SetRangeUser(-0.02, 0.02);
    else if (observable=="rg") h_unc_rel_sym_up->GetYaxis()->SetRangeUser(-0.6, 0.6);
    else if (observable=="zg") h_unc_rel_sym_up->GetYaxis()->SetRangeUser(-0.2, 0.2);
    else if (observable=="zpt") h_unc_rel_sym_up->GetYaxis()->SetRangeUser(-0.2, 0.2);
    h_unc_rel_sym_up->GetYaxis()->SetNdivisions(-4);
    h_unc_rel_sym_up->Draw("hist");
    h_unc_rel_sym_down->Draw("hist same");
    for (int i=0; i<10; i++) {
        histos_jk_rel_unc[i]->Draw("hist same");
    }

    c_unc->cd();
    top_pad->Draw();
    bottom_pad->Draw();
    
    c_unc->Draw();
    c_unc->Print("../plots_thesis/"+sample+"_"+label+"_mc_stat_unc_"+observable+".pdf");
    
    // Save histograms 
    TString suffix_out = (is_inclusive) ? "inclusive" : "XXT";
    TString fout_name = "histos/" + observable + "_mc_stat_unc_" + suffix_out + ".root";
    TFile *fout = new TFile(fout_name, "RECREATE");
    h_nom_1d->Write("h_nom_1d");
    for (int i=0; i<10; i++) {
        histos_jk_1d[i]->Write(); 
    }    
    h_unc_up->Write("h_unc_up");
    h_unc_up_rel->Write("h_unc_up_rel");
    h_unc_rel_sym_up->Write("h_unc_rel_sym_up");
    h_unc_rel_sym_down->Write("h_unc_rel_sym_down");
    fout->Close();

}
