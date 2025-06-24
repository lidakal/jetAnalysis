const std::vector<TString> srcnames_bjet = {
    "AbsoluteStat", "AbsoluteScale", "AbsoluteFlavMap", "AbsoluteMPFBias", 
    "Fragmentation", "SinglePionECAL", "SinglePionHCAL", "TimePtEta", 
    "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF", "RelativePtBB", 
    "RelativePtEC1", "RelativePtEC2", "RelativePtHF", "RelativeBal", 
    "RelativeSample", "RelativeFSR", "RelativeStatFSR", "RelativeStatEC", 
    "PileUpDataMC", "PileUpPtRef", "PileUpPtBB", "PileUpPtEC1", 
    "PileUpPtEC2", "PileUpPtHF", "FlavorPureBottom", "TimeRunF"
};

const std::vector<TString> srcnames_incl = {
    "AbsoluteStat", "AbsoluteScale", "AbsoluteFlavMap", "AbsoluteMPFBias",
    "Fragmentation", "SinglePionECAL", "SinglePionHCAL", "TimePtEta",
    "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF", "RelativePtBB", 
    "RelativePtEC1", "RelativePtEC2", "RelativePtHF", "RelativeBal", 
    "RelativeSample", "RelativeFSR", "RelativeStatFSR", "RelativeStatEC", 
    "PileUpDataMC", "PileUpPtRef", "PileUpPtBB", "PileUpPtEC1", 
    "PileUpPtEC2", "PileUpPtHF", "FlavorQCD", "TimeRunF"
};

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
    std::vector<std::string> hexColors = {
        "#3f90da", "#4983d6", "#5376d2", "#5d69ce", "#675cca", "#714fc6", "#7b42c2", // Row 1
        "#832db6", "#9031a8", "#9d359a", "#aa398c", "#b73d7e", "#c44170", "#d14562", // Row 2
        "#dc4a7e", "#e54c96", "#ed4ead", "#f250b8", "#f653c1", "#f956c9", "#fb59cf", // Row 3 (new pinks)
        "#f15e09", "#f26006", "#f36303", "#f46600", "#f36500", "#f26400", "#e76300"  // Row 4
    };

    std::vector<Int_t> palette;
    for (const auto& hex : hexColors) {
        palette.push_back(TColor::GetColor(hex.c_str()));
    }

    return palette[i];
}

const int iflav = 26;

void jec_unc_src(TString observable="rg")
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

    TString sample = "dijet_PF40";
    TString label = "aggrTMVA_inclusive";
    bool is_inclusive = label.Contains("inclusive");

    TString suffix = (is_inclusive) ? "" : "_withSF";
    std::vector<TString> srcnames = (is_inclusive) ? srcnames_incl : srcnames_bjet;
    int nsrc = srcnames.size();

    TFile *fin_nom = new TFile("../unfolding/histos/"+sample+"_"+label+"_unfolded_histograms_"+observable+"_jer_nom_jec_nom"+suffix+".root");
    TH2D *h_nom = (TH2D *) fin_nom->Get("h_data_unfolded")->Clone("h_nom");
    TFile *fin_up = new TFile("../unfolding/histos/"+sample+"_"+label+"_unfolded_histograms_"+observable+"_jer_nom_jec_up_JECsrc"+suffix+".root");
    std::vector<TH2D *> histos_up;
    TFile *fin_down = new TFile("../unfolding/histos/"+sample+"_"+label+"_unfolded_histograms_"+observable+"_jer_nom_jec_down_JECsrc"+suffix+".root");
    std::vector<TH2D *> histos_down;

    for (int i=0; i<nsrc; i++) {
        TH2D *h_up = (TH2D *) fin_up->Get(Form("h%d_data_unfolded", i))->Clone(Form("h%d_up", i));
        histos_up.push_back(h_up);

        TH2D *h_down = (TH2D *) fin_down->Get(Form("h%d_data_unfolded", i))->Clone(Form("h%d_down", i));
        histos_down.push_back(h_down);
    }

    // Make projections 
    int ibin_pt = 2;
    double pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
    double pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);

    TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX("h_nom_1d", ibin_pt, ibin_pt);
    std::vector<TH1D *> histos_up_1d;
    std::vector<TH1D *> histos_down_1d;

    for (int i=0; i<nsrc; i++) {
        TH1D *h_up_1d = (TH1D *) histos_up[i]->ProjectionX(Form("h%d_up_1d", i), ibin_pt, ibin_pt);
        histos_up_1d.push_back(h_up_1d);

        TH1D *h_down_1d = (TH1D *) histos_down[i]->ProjectionX(Form("h%d_down_1d", i), ibin_pt, ibin_pt);
        histos_down_1d.push_back(h_down_1d);
    }

    // Normalize
    int nbins_x = h_nom->GetNbinsX();
    int ibin_x_min = 1;
    int ibin_x_max = nbins_x;

    if (observable!="zpt") ibin_x_min = 2;
    if (observable=="rg") ibin_x_max = nbins_x - 1;

    h_nom_1d->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
    h_nom_1d->Scale(1/h_nom_1d->Integral(), "width");

    for (int i=0; i<nsrc; i++) {
        histos_up_1d[i]->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        histos_up_1d[i]->Scale(1/histos_up_1d[i]->Integral(), "width");

        histos_down_1d[i]->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
        histos_down_1d[i]->Scale(1/histos_down_1d[i]->Integral(), "width");
    }

    // Format histograms
    h_nom_1d->SetLineColor(kBlack);
    h_nom_1d->SetMarkerColor(kBlack);
    h_nom_1d->SetMarkerStyle(kFullCrossX);
    h_nom_1d->SetMarkerSize(2);

    for (int i=0; i<nsrc; i++) {
        histos_up_1d[i]->SetLineColor(myPalette(i)); 
        histos_up_1d[i]->SetLineWidth(2);

        histos_down_1d[i]->SetLineColorAlpha(myPalette(i),0.5); 
        histos_down_1d[i]->SetLineWidth(2);
    }

    // Legend 
    TLegend *leg = new TLegend(0.2, 0.4, 0.33, 0.83);
    leg->SetNColumns(2);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetMargin(0.7);
    leg->AddEntry(h_nom_1d, "Nominal", "pe1");  
    TH1D *h_ghost = (TH1D *) h_nom_1d->Clone("h_ghost");
    h_ghost->SetLineColor(kWhite);
    h_ghost->SetMarkerColor(kWhite);
    h_ghost->SetMarkerStyle(1);
    leg->AddEntry(h_ghost, " ", "p");
    leg->AddEntry(h_ghost, " ", "p");
    leg->AddEntry(h_ghost, " ", "p");
    for (int i=0; i<nsrc; i++) {
        if (i==0||i==2||i==9||i==10||i==13||i==14||i==24||i==25) continue; // give 0
        leg->AddEntry(histos_up_1d[i], " ", "l");
        leg->AddEntry(histos_down_1d[i], " ", "l");
    }

    TText *pave1 = new TText(0.26,0.59,"Up variations"); // rg bjet
    pave1->SetNDC();
    pave1->SetTextAlign(22);
    pave1->SetTextSize(text_size-4);
    pave1->SetTextAngle(90);

    TText *pave2 = new TText(0.34,0.59,"Down variations");
    pave2->SetNDC();
    pave2->SetTextAlign(22);
    pave2->SetTextSize(text_size-4);
    pave2->SetTextAngle(90);

    // Calculate uncertainties
    std::vector<TH1D *> histos_unc_up;
    std::vector<TH1D *> histos_unc_up_rel;
    std::vector<TH1D *> histos_unc_rel_sym_up;

    std::vector<TH1D *> histos_unc_down;
    std::vector<TH1D *> histos_unc_down_rel;
    std::vector<TH1D *> histos_unc_rel_sym_down;

    for (int i=0; i<nsrc; i++) {
        TH1D * h_unc_up = (TH1D *) histos_up_1d[i]->Clone(Form("h%d_unc_up", i));
        h_unc_up->Add(h_nom_1d, -1); // up-nom
        histos_unc_up.push_back(h_unc_up);
        TH1D * h_unc_down = (TH1D *) histos_down_1d[i]->Clone(Form("h%d_unc_down", i));
        h_unc_down->Add(h_nom_1d, -1); // down-nom
        histos_unc_down.push_back(h_unc_down);

        TH1D *h_unc_up_rel = (TH1D *) h_unc_up->Clone(Form("h%d_unc_up_rel", i));
        h_unc_up_rel->Divide(h_nom_1d); // (up-nom)/nom
        histos_unc_up_rel.push_back(h_unc_up_rel);

        TH1D *h_unc_down_rel = (TH1D *) h_unc_down->Clone(Form("h%d_unc_down_rel", i));
        h_unc_down_rel->Divide(h_nom_1d); // (down-nom)/nom
        histos_unc_down_rel.push_back(h_unc_down);

        TH1D *h_unc_rel_sym = (TH1D *) h_unc_up_rel->Clone(Form("h%d_unc_rel_sym",i));
        h_unc_rel_sym->Reset();
        for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
            double maxUnc = std::max(std::abs(h_unc_up_rel->GetBinContent(ibin_x)), std::abs(h_unc_down_rel->GetBinContent(ibin_x)));
            h_unc_rel_sym->SetBinContent(ibin_x, maxUnc);
        }
        histos_unc_rel_sym_up.push_back(h_unc_rel_sym);
        TH1D *h_unc_rel_sym_down = (TH1D *) h_unc_rel_sym->Clone(Form("h%d_unc_rel_sym_down",i));
        h_unc_rel_sym_down->Scale(-1.);
        histos_unc_rel_sym_down.push_back(h_unc_rel_sym_down);
    }

    TH1D *h_total_unc_rel_sym_up = (TH1D *) h_nom_1d->Clone("h_total_unc_rel_sym");
    h_total_unc_rel_sym_up->Reset();
    for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
        double sum2_sym = 0.;
        for (int i=0; i<nsrc; i++) {
            double unc_rel_sym = histos_unc_rel_sym_up[i]->GetBinContent(ibin_x);
            sum2_sym += unc_rel_sym*unc_rel_sym;
        }
        h_total_unc_rel_sym_up->SetBinContent(ibin_x, std::sqrt(sum2_sym));
    }
    h_total_unc_rel_sym_up->SetFillColor(kGray);
    h_total_unc_rel_sym_up->SetFillStyle(1001);
    h_total_unc_rel_sym_up->SetLineColor(kGray);
    TH1D *h_total_unc_rel_sym_down = (TH1D *) h_total_unc_rel_sym_up->Clone("h_total_unc_rel_sym_down");
    h_total_unc_rel_sym_down->Scale(-1.);

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
    else if (is_inclusive&&observable=="rg") h_nom_1d->GetYaxis()->SetRangeUser(0., 2.1);
    else if (!is_inclusive&&observable=="zg") h_nom_1d->GetYaxis()->SetRangeUser(0., 13.);
    else if (is_inclusive&&observable=="zg") h_nom_1d->GetYaxis()->SetRangeUser(0., 10.);
    else if (observable=="zpt") h_nom_1d->GetYaxis()->SetRangeUser(0., 4.);
    h_nom_1d->GetXaxis()->SetLabelOffset(10.); // kick it out of existence
    h_nom_1d->GetXaxis()->SetTitleOffset(10.); // kick it out of existence
    h_nom_1d->Draw("pe1");
    for (int i=0; i<nsrc; i++) {
        if (i==0||i==2||i==9||i==10||i==13||i==14||i==24||i==25) continue; // give 0
        histos_up_1d[i]->Draw("hist same");
        histos_down_1d[i]->Draw("hist same");
    }
    h_nom_1d->Draw("pe1 same");
    drawHeader();
    leg->Draw();
    pave1->Draw();
    pave2->Draw();
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
        jet_info->SetTextAlign(32);
        jet_info->DrawLatex(0.9, 0.83, "anti-k_{T}, R = 0.4 b jets");
        jet_info->DrawLatex(0.9, 0.75, Form("%.0f < p_{T}^{jet} < %.0f GeV/c, |#eta^{jet}| < 2", pt_min, pt_max));
    }


    bottom_pad->cd();
    h_total_unc_rel_sym_up->GetXaxis()->SetTitle(xlabel);
    h_total_unc_rel_sym_up->GetXaxis()->SetTitleSize(text_size);
    h_total_unc_rel_sym_up->GetXaxis()->SetLabelSize(text_size-4);
    h_total_unc_rel_sym_up->GetXaxis()->SetTitleOffset(3.);
    h_total_unc_rel_sym_up->GetYaxis()->SetTitle("#splitline{Relative}{uncertainty}");
    h_total_unc_rel_sym_up->GetYaxis()->SetTitleSize(text_size);
    h_total_unc_rel_sym_up->GetYaxis()->SetLabelSize(text_size-4);
    h_total_unc_rel_sym_up->GetYaxis()->SetTitleOffset(1.4);
    if (is_inclusive&&observable=="rg") h_total_unc_rel_sym_up->GetYaxis()->SetRangeUser(-0.04, 0.04);
    else if (is_inclusive&&observable=="zg") h_total_unc_rel_sym_up->GetYaxis()->SetRangeUser(-0.04, 0.04);
    else if (observable=="rg") h_total_unc_rel_sym_up->GetYaxis()->SetRangeUser(-0.5, 0.5);
    else if (observable=="zg") h_total_unc_rel_sym_up->GetYaxis()->SetRangeUser(-0.1, 0.1);
    else if (observable=="zpt") h_total_unc_rel_sym_up->GetYaxis()->SetRangeUser(-0.2, 0.2);
    h_total_unc_rel_sym_up->GetYaxis()->SetNdivisions(-4);
    h_total_unc_rel_sym_up->Draw("hist");
    h_total_unc_rel_sym_down->Draw("hist same");
    for (int i=0; i<nsrc; i++) {
        if (i==0||i==2||i==9||i==10||i==13||i==14||i==24||i==25) continue; // give 0
        histos_unc_up_rel[i]->Draw("hist same");
        histos_unc_down_rel[i]->Draw("hist same");
    }

    c_unc->cd();
    top_pad->Draw();
    bottom_pad->Draw();
    
    c_unc->Draw();
    c_unc->Print("../plots_thesis/"+sample+"_"+label+"_jec_unc_src_"+observable+".pdf");

    // Write to file 
    TString suffix_out = (is_inclusive) ? "inclusive" : "XXT";
    TString fout_name = "./histos/"+observable+"_jec_unc_src_"+suffix_out+".root";
    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    h_nom_1d->Write("h_nom_1d");
    for (int i=0; i<nsrc; i++) {
        histos_up_1d[i]->Write(Form("h%d_up_1d", i));
        histos_down_1d[i]->Write(Form("h%d_down_1d", i));
        histos_unc_up[i]->Write(Form("h%d_unc_up", i));
        histos_unc_down[i]->Write(Form("h%d_unc_down", i));
        histos_unc_up_rel[i]->Write(Form("h%d_unc_up_rel", i));
        histos_unc_down_rel[i]->Write(Form("h%d_unc_down_rel", i));
        histos_unc_rel_sym_up[i]->Write(Form("h%d_unc_rel_sym_up", i));
        histos_unc_rel_sym_down[i]->Write(Form("h%d_unc_rel_sym_down", i));
    }
    h_total_unc_rel_sym_up->Write("h_total_unc_rel_sym_up");
    h_total_unc_rel_sym_down->Write("h_total_unc_rel_sym_down");
}