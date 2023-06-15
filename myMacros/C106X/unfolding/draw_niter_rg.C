// #include "../../../../RooUnfold/src/RooUnfoldResponse.h"
// #include "../../../../RooUnfold/src/RooUnfoldBayes.h"
// #include "../../../../RooUnfold/src/RooUnfoldInvert.h"
#include "../hist_utils.h"

void draw_niter_rg()
{
    // ---- Setup 
    gSystem->Load("libRooUnfold.so");
    gStyle->SetPalette(kDarkBodyRadiator);
    auto palette = TColor::GetPalette();

    Double_t ptMin = 150.;
    Double_t ptMax = 200.;

    Float_t text_size = 26.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size);
    gStyle->SetLabelSize(text_size, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");

    // ---- Grab histos
    TString sample = "bjet";
    TString label = "aggrTMVA_withNb";
    TString fname = "./histos/qcd_" + sample + "_" + label + "_unfold_2d.root";

    TFile *fin = new TFile(fname);
    TH2D *h_data_reco_rgpt = (TH2D *) fin->Get("h_data_reco_rgpt");
    TH2D *h_data_true_rgpt = (TH2D *) fin->Get("h_data_true_rgpt");
    TH2D *h_mc_true_rgpt = (TH2D *) fin->Get("h_mc_true_rgpt");
    TH2D *h_mc_reco_rgpt = (TH2D *) fin->Get("h_mc_reco_rgpt");
    TH2D *h_mc_fake_rgpt = (TH2D *) fin->Get("h_mc_fake_rgpt");
    RooUnfoldResponse *response_rgpt = (RooUnfoldResponse *) fin->Get("response_rgpt");
    

    THStack *hStack_unfolded_rg = new THStack("hStack_unfolded_rg", "unfolded/true");
    hStack_unfolded_rg->SetTitle("; ln(0.4/R_{g}); 1/N_{jets} dN/d(ln(0.4/R_{g}))");

    THStack *hStack_refolded_rg = new THStack("hStack_refolded_rg", "refolded/data");
    hStack_refolded_rg->SetTitle("; ln(0.4/R_{g}); 1/N_{jets} dN/d(ln(0.4/R_{g}))");

    TLegend *leg_unfolded_rg = new TLegend(0.4, 0.7, 0.6, 0.9);
    leg_unfolded_rg->SetFillStyle(0);
    leg_unfolded_rg->SetBorderSize(0);
    leg_unfolded_rg->SetMargin(0.15);
    leg_unfolded_rg->SetHeader("unfolded / true");

    TLegend *leg_refolded_rg = new TLegend(0.6, 0.7, 0.8, 0.9);
    leg_refolded_rg->SetFillStyle(0);
    leg_refolded_rg->SetBorderSize(0);
    leg_refolded_rg->SetMargin(0.15);
    leg_refolded_rg->SetHeader("re-folded / reco");

    // ---- Unfold
    int i_palette = 0;
    std::vector iterations = {1, 5, 10, 15, 20, 30, 50, 100};
    for (Int_t niter : iterations) {
        RooUnfoldBayes unfold(response_rgpt, h_data_reco_rgpt, niter);
        // TH2D *h_data_reco_rgpt_unfolded = (TH2D *) unfold.Hunfold();
        TH2D *h_data_reco_rgpt_unfolded = (TH2D *) unfold.Hreco();

        // ---- Fold back
        TH2D *h_data_reco_rgpt_refolded = (TH2D *) response_rgpt->ApplyToTruth(h_data_reco_rgpt_unfolded, "h_data_reco_rgpt_refolded");

        // ---- Make projections
        Int_t iptmin = h_data_reco_rgpt->GetYaxis()->FindBin(ptMin);
        Int_t iptmax = h_data_reco_rgpt->GetYaxis()->FindBin(ptMax) - 1;
        Double_t ptMin_real = h_data_reco_rgpt->GetYaxis()->GetBinLowEdge(iptmin);
        Double_t ptMax_real = h_data_reco_rgpt->GetYaxis()->GetBinUpEdge(iptmax);

        TH1D *h_data_reco_rg = (TH1D *) h_data_reco_rgpt->ProjectionX("h_data_reco_rg", iptmin, iptmax);
        TH1D *h_data_true_rg = (TH1D *) h_data_true_rgpt->ProjectionX("h_data_true_rg", iptmin, iptmax);
        TH1D *h_data_reco_rg_unfolded = (TH1D *) h_data_reco_rgpt_unfolded->ProjectionX("h_data_reco_rg_unfolded", iptmin, iptmax);
        TH1D *h_data_reco_rg_refolded = (TH1D *) h_data_reco_rgpt_refolded->ProjectionX("h_data_reco_rg_refolded", iptmin, iptmax);
        // TH1D *h_mc_true_rg = (TH1D *) h_mc_true_rgpt->ProjectionX("h_mc_true_rg", iptmin, iptmax);
        // TH1D *h_mc_reco_rg = (TH1D *) h_mc_reco_rgpt->ProjectionX("h_mc_reco_rg", iptmin, iptmax); 
        TH1D *h_mc_fake_rg = (TH1D *) h_mc_fake_rgpt->ProjectionX("h_mc_fake_rg", iptmin, iptmax);

        // ---- Normalize
        Int_t nbins = h_data_reco_rg->GetNbinsX();
        Int_t ibin_min = 1; // underflow already included in the histo
        Int_t ibin_max = nbins; // overflow already included in the histo
        for (auto h : {h_data_reco_rg, h_data_true_rg,
                    h_data_reco_rg_unfolded, h_data_reco_rg_refolded,
                    // h_mc_true_rg, h_mc_reco_rg
                    }) {
                        normalize_histo(h, ibin_min, ibin_max);
                        // h->GetXaxis()->SetRange(2, nbins - 1);
                        // h->SetBinContent(1, h->GetBinContent(1) / 10);
                    }

        // ---- Make ratios
        TH1D *h_unfolded_true = (TH1D *) h_data_reco_rg_unfolded->Clone("h_unfolded_true");
        h_unfolded_true->Divide(h_data_true_rg);
        format_histo(h_unfolded_true, palette.At(i_palette*palette.GetSize()/iterations.size()));
        hStack_unfolded_rg->Add(h_unfolded_true);
        leg_unfolded_rg->AddEntry(h_unfolded_true, Form("%d iterations", niter), "l");

        TH1D *h_refolded_reco = (TH1D *) h_data_reco_rg_refolded->Clone("h_refolded_reco");
        h_refolded_reco->Add(h_mc_fake_rg); // u = A^-1(data-fakes) => data = Au+fakes
        h_refolded_reco->Divide(h_data_reco_rg);
        format_histo(h_refolded_reco, palette.At(i_palette*palette.GetSize()/iterations.size()));
        hStack_refolded_rg->Add(h_refolded_reco);
        leg_refolded_rg->AddEntry(h_refolded_reco, Form("%d iterations", niter), "l");

        TString jet_selection = Form("%.0f < p_{T}^{jet} < %.0f (GeV), k_{T} > 1 GeV, b tagged jets", ptMin_real, ptMax_real);
        i_palette++;
    }

    

    TPaveText *underflow_text = new TPaveText(-0.275, 0.02, 0.05, 0.07);
    underflow_text->SetTextSize(text_size);
    underflow_text->SetFillStyle(0);
    underflow_text->SetBorderSize(0);
    underflow_text->AddText("underflow");

    TPaveText *overflow_text = new TPaveText(2.2, 0.02, 2.4, 0.07);
    overflow_text->SetTextSize(text_size);
    overflow_text->SetFillStyle(0);
    overflow_text->SetBorderSize(0);
    overflow_text->AddText("overflow");

    TCanvas *c_unfolded_true = new TCanvas("c_unfolded_true", "", 1200, 1000);
    hStack_unfolded_rg->Draw("nostack hist");
    underflow_text->Draw();
    overflow_text->Draw();
    leg_unfolded_rg->Draw();
    c_unfolded_true->Draw();

    TCanvas *c_refolded_true = new TCanvas("c_refolded_true", "", 1200, 1000);
    hStack_refolded_rg->Draw("nostack hist");
    underflow_text->Draw();
    overflow_text->Draw();
    leg_refolded_rg->Draw();
    c_refolded_true->Draw();
}