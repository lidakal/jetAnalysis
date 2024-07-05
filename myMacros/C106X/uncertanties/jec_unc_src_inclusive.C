#include "../draw_utils.h"

const std::vector<TString> srcnames_incl = {
    "AbsoluteStat", "AbsoluteScale", "AbsoluteFlavMap", "AbsoluteMPFBias",
    "Fragmentation", "SinglePionECAL", "SinglePionHCAL", "TimePtEta",
    "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF", "RelativePtBB", 
    "RelativePtEC1", "RelativePtEC2", "RelativePtHF", "RelativeBal", 
    "RelativeSample", "RelativeFSR", "RelativeStatFSR", "RelativeStatEC", 
    "PileUpDataMC", "PileUpPtRef", "PileUpPtBB", "PileUpPtEC1", 
    "PileUpPtEC2", "PileUpPtHF", "FlavorQCD", "TimeRunF"
};

const int iflav = 26;

void jec_unc_src_inclusive(TString observable="rg")
{
    // Setup 
    Float_t text_size = 20.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetErrorX(0.5);

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z^{ch}";
    TString ylabel = "1/N dN/d" + xlabel;

    TFile *fin_nom = new TFile("../unfolding/histos/dijet_PF40_aggrTMVA_inclusive_unfolded_histograms_"+observable+"_jer_nom_jec_nom.root");
    TH2D *h_nom = (TH2D *) fin_nom->Get("h_data_unfolded")->Clone("h_nom");
    TFile *fin_up = new TFile("../unfolding/histos/dijet_PF40_aggrTMVA_inclusive_unfolded_histograms_"+observable+"_jer_nom_jec_up_JECsrc.root");
    TFile *fin_down = new TFile("../unfolding/histos/dijet_PF40_aggrTMVA_inclusive_unfolded_histograms_"+observable+"_jer_nom_jec_down_JECsrc.root");

    int nbins_x = h_nom->GetNbinsX();
    int nbins_pt = h_nom->GetNbinsY();

    // ---------- SELECT HISTOGRAMS -----------

    int ibin_pt = 2;
    double pt_min = h_nom->GetYaxis()->GetBinLowEdge(ibin_pt);
    double pt_max = h_nom->GetYaxis()->GetBinUpEdge(ibin_pt);

    int ibin_x_min = 1;
    int ibin_x_max = nbins_x;

    if (observable!="zpt") ibin_x_min = 2;
    if (observable=="rg") ibin_x_max = nbins_x - 1;

    TH1D *h_nom_1d = (TH1D *) h_nom->ProjectionX(Form("h_nom_1d_%d", ibin_pt), ibin_pt, ibin_pt);
    h_nom_1d->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
    h_nom_1d->Scale(1/h_nom_1d->Integral(), "width");

    std::vector<TH1D *> histos_var_up;
    std::vector<TH1D *> histos_var_down;
    std::vector<TH1D *> histos_unc_rel_sym;
    std::vector<TH1D *> histos_unc_rel_nosym_up;
    std::vector<TH1D *> histos_unc_rel_nosym_down;

    for (int isrc=0; isrc<28; isrc++) {
        TH2D *h_up = (TH2D *) fin_up->Get(Form("h%d_data_unfolded",isrc))->Clone(Form("h%d_up",isrc));
        TH2D *h_down = (TH2D *) fin_down->Get(Form("h%d_data_unfolded",isrc))->Clone(Form("h%d_down",isrc));

        // Make projections 
        TH1D *h_up_1d = (TH1D *) h_up->ProjectionX(Form("h%d_up_1d_%d", isrc, ibin_pt), ibin_pt, ibin_pt);
        TH1D *h_down_1d = (TH1D *) h_down->ProjectionX(Form("h%d_down_1d_%d", isrc, ibin_pt), ibin_pt, ibin_pt);
        
        // Normalize 
        for (auto h : {h_up_1d, h_down_1d}) {
            h->GetXaxis()->SetRange(ibin_x_min, ibin_x_max);
            h->Scale(1/h->Integral(), "width");
        }

        histos_var_up.push_back(h_up_1d);
        histos_var_down.push_back(h_down_1d);

        // Extract uncertainties from normalized histograms
        TH1D * h_unc_up = (TH1D *) h_up_1d->Clone(Form("h%d_unc_up_%d", isrc, ibin_pt));
        h_unc_up->Add(h_nom_1d, -1); // up-nom
        TH1D * h_unc_down = (TH1D *) h_down_1d->Clone(Form("h%d_unc_down_%d", isrc, ibin_pt));
        h_unc_down->Add(h_nom_1d, -1); // down-nom

        // Extract relative uncertainties
        TH1D *h_unc_rel_up = (TH1D *) h_unc_up->Clone(Form("h%d_unc_rel_up_%d", isrc, ibin_pt));
        h_unc_rel_up->Divide(h_nom_1d);

        TH1D *h_unc_rel_down = (TH1D *) h_unc_down->Clone(Form("h%d_unc_rel_down_%d", isrc, ibin_pt));
        h_unc_rel_down->Divide(h_nom_1d);

        histos_unc_rel_nosym_up.push_back(h_unc_rel_up);
        histos_unc_rel_nosym_down.push_back(h_unc_rel_down);

        // Extract symmetrized relative uncertainty like in top-17-013
        TH1D *h_unc_rel_sym = (TH1D *) h_unc_rel_up->Clone(Form("h%d_unc_rel_sym",isrc));
        for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
            double maxUnc = std::max(std::abs(h_unc_rel_up->GetBinContent(ibin_x)), std::abs(h_unc_rel_down->GetBinContent(ibin_x)));
            h_unc_rel_sym->SetBinContent(ibin_x, maxUnc);
        }
        histos_unc_rel_sym.push_back(h_unc_rel_sym);
    }

    // ----------- EXTRACT TOTAL RELATIVE UNCERTAINTY -------------
    TH1D *h_total_unc_rel_sym = (TH1D *) h_nom_1d->Clone(Form("h_total_unc_rel_sym_%d", ibin_pt));
    h_total_unc_rel_sym->Reset();
    TH1D *h_total_unc_rel_sym_noflav = (TH1D *) h_total_unc_rel_sym->Clone(Form("h_total_unc_rel_sym_noflav_%d", ibin_pt));
    TH1D *h_total_unc_rel_nosym_up = (TH1D *) h_total_unc_rel_sym->Clone(Form("h_total_unc_rel_nosym_up_%d", ibin_pt));
    TH1D *h_total_unc_rel_nosym_down = (TH1D *) h_total_unc_rel_sym->Clone(Form("h_total_unc_rel_nosym_down_%d", ibin_pt));

    for (int ibin_x=ibin_x_min; ibin_x<=ibin_x_max; ibin_x++) {
        double sum2_sym = 0.;
        double sum2_sym_noflav = 0.;
        double sum2_nosym_up = 0.;
        double sum2_nosym_down = 0.;

        for (int isrc=0; isrc<28; isrc++) {
            double unc_rel_sym = histos_unc_rel_sym[isrc]->GetBinContent(ibin_x);
            double unc_rel_nosym_up = histos_unc_rel_nosym_up[isrc]->GetBinContent(ibin_x);
            double unc_rel_nosym_down = histos_unc_rel_nosym_down[isrc]->GetBinContent(ibin_x);

            sum2_sym += unc_rel_sym*unc_rel_sym;
            if (isrc!=iflav) sum2_sym_noflav += unc_rel_sym*unc_rel_sym;

            if (unc_rel_nosym_up>0) sum2_nosym_up += unc_rel_nosym_up*unc_rel_nosym_up;
            else sum2_nosym_down += unc_rel_nosym_up*unc_rel_nosym_up;

            if (unc_rel_nosym_down>0) sum2_nosym_up += unc_rel_nosym_down*unc_rel_nosym_down;
            else sum2_nosym_down += unc_rel_nosym_down*unc_rel_nosym_down;
        }

        h_total_unc_rel_sym->SetBinContent(ibin_x, std::sqrt(sum2_sym));
        h_total_unc_rel_sym_noflav->SetBinContent(ibin_x, std::sqrt(sum2_sym_noflav));
        h_total_unc_rel_nosym_up->SetBinContent(ibin_x, std::sqrt(sum2_nosym_up));
        h_total_unc_rel_nosym_down->SetBinContent(ibin_x, std::sqrt(sum2_nosym_down));        
    }

    // ------------------ WRITE HISTOGRAMS TO OUTPUT FILE ------------------
    TString fout_name = "./histos/"+observable+"_jec_unc_src_inclusive.root";
    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    h_total_unc_rel_sym->Write();
    h_total_unc_rel_sym_noflav->Write();
    h_total_unc_rel_nosym_up->Write();
    h_total_unc_rel_nosym_down->Write();

    TH1D *h_flav_unc_rel_sym = (TH1D *)  histos_unc_rel_sym[iflav]->Clone(Form("h_flav_unc_rel_sym_%d", ibin_pt));
    h_flav_unc_rel_sym->Write();

    // ----------- SKIP PLOTTING --------------
    if (false) return;

    int icolor_all=0;
    int icolor_limited=0;
    auto colors_all = TColor::GetPalette();
    std::vector<int> colors_limited = {kRed, kGreen, kBlue, kOrange, kViolet};

    // ----------- PLOT VAR+REL UNC -----------
    TCanvas *c_jec = new TCanvas("c_jec", "", 1200, 1000);
    TPad *bottom_pad = new TPad("bottom_pad", "", 0., 0., 0.8, 0.25);
    TPad *top_pad = new TPad("top_pad", "", 0., 0.25, 0.8, 1.);
    TPad *right_pad = new TPad("right_pad" ,"", 0.8, 0., 1., 1.);

    TLegend *leg_jec = new TLegend(0., 0.05, 1., 0.95);
    leg_jec->SetFillStyle(0);
    leg_jec->SetBorderSize(0);
    leg_jec->SetMargin(0.15);
    // leg_jec->SetHeader(Form("%.0f < p_{T}^{jet} < %.0f (GeV)", pt_min, pt_max));
    
    top_pad->cd();

    h_nom_1d->SetMarkerStyle(kFullCircle);
    h_nom_1d->SetMarkerColor(kBlack); 
    h_nom_1d->SetLineColor(kBlack);
    h_nom_1d->GetYaxis()->SetTitle(ylabel);
    h_nom_1d->GetYaxis()->SetTitleOffset(2.);
    leg_jec->AddEntry(h_nom_1d, "nominal", "pe1");
    h_nom_1d->Draw("pe1");

    for (int isrc=0; isrc<28; isrc++) {
        if (isrc==0||isrc==2||isrc==9||isrc==10||isrc==13||isrc==14||isrc==24||isrc==25) continue; // give 0
        int color = colors_all[icolor_all++];

        // color leading sources
        if (isrc==26) color = kRed;
        if (isrc==27) color = kOrange;
        
        histos_var_up[isrc]->SetMarkerStyle(kFullTriangleUp); 
        histos_var_up[isrc]->SetMarkerColor(color);
        histos_var_up[isrc]->SetLineColor(color);
        leg_jec->AddEntry(histos_var_up[isrc], Form("%s up",srcnames_incl[isrc].Data()), "pe1");
        histos_var_up[isrc]->Draw("pe1 same");

        histos_var_down[isrc]->SetMarkerStyle(kFullTriangleDown); 
        histos_var_down[isrc]->SetMarkerColor(color);
        histos_var_down[isrc]->SetLineColor(color);
        leg_jec->AddEntry(histos_var_down[isrc], Form("%s down",srcnames_incl[isrc].Data()), "pe1");
        histos_var_down[isrc]->Draw("pe1 same");

        bottom_pad->cd();
        bottom_pad->SetTopMargin(0.01);
        bottom_pad->SetBottomMargin(0.3);
        
        histos_unc_rel_nosym_up[isrc]->GetYaxis()->SetTitle("(var-nom)/nom");
        histos_unc_rel_nosym_up[isrc]->GetYaxis()->SetTitleOffset(2.);
        histos_unc_rel_nosym_up[isrc]->GetYaxis()->SetRangeUser(-0.03, 0.03);
        histos_unc_rel_nosym_up[isrc]->GetXaxis()->SetTitle(xlabel);
        histos_unc_rel_nosym_up[isrc]->GetXaxis()->SetTitleOffset(3.);
        histos_unc_rel_nosym_up[isrc]->SetMarkerStyle(kFullTriangleUp); 
        histos_unc_rel_nosym_up[isrc]->SetMarkerColor(color);
        histos_unc_rel_nosym_up[isrc]->SetLineColor(color);
        histos_unc_rel_nosym_up[isrc]->Draw("pe1 l same");

        histos_unc_rel_nosym_down[isrc]->SetMarkerStyle(kFullTriangleDown); 
        histos_unc_rel_nosym_down[isrc]->SetMarkerColor(color);
        histos_unc_rel_nosym_down[isrc]->SetLineColor(color);
        histos_unc_rel_nosym_down[isrc]->Draw("pe1 l same");
    }

    right_pad->cd();
    leg_jec->Draw();
    
    c_jec->cd();
    top_pad->Draw();
    bottom_pad->Draw();
    right_pad->Draw();

}