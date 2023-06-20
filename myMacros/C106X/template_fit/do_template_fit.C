#include "Purity.h"

void do_template_fit()
{
    // Setup 

    // Load MC
    TString sample = "dijet";
    TString label = "aggrTMVA_withNb";
    TString fin_mc_name = "./histos/qcd_" + sample + "_" + label + "_data_for_template_fit.root";
    // TString rootf_out = "histos/qcd_" + sample + "_" + label + "_signal_fraction.root";
    TFile *fin_mc = new TFile(fin_mc_name);

    TH3D *hsig = (TH3D *) fin_mc->Get("hsig");
    TH3D *hbkg_bb = (TH3D *) fin_mc->Get("hbkg_bb");
    TH3D *hbkg_rest = (TH3D *) fin_mc->Get("hbkg_rest");
    TH3D *hbkg = (TH3D *) hbkg_bb->Clone("hbkg");
    hbkg->Add(hbkg_rest);

    // Load data
    TH3D *hdata = (TH3D *) fin_mc->Get("hdata_mc");

    // Get bins + create output histograms
    Int_t nbins_rg = hdata->GetNbinsX();
    Int_t nbins_pt = hdata->GetNbinsZ();

    TH2D *h_signal_fraction = (TH2D *) hdata->Project3D("zx");
    h_signal_fraction->SetName("h_signal_fraction");

    TH2D *h_total_normalization = (TH2D *) hdata->Project3D("zx");
    h_total_normalization->SetName("h_total_normalization");

    const int divisions = 1;

    for (Int_t ibin_pt = 0; ibin_pt <= nbins_pt + 1; ibin_pt++) {
        for (Int_t ibin_rg = 0; ibin_rg <= nbins_rg + 1; ibin_rg++) {
            h_signal_fraction->SetBinContent(ibin_rg, ibin_pt, 0);
            if (ibin_pt == 0 || ibin_rg == 0) continue;
            if (ibin_pt == nbins_pt + 1 || ibin_rg == nbins_rg + 1) continue;

            if (ibin_pt != 2) continue;
            if (ibin_rg != 3) continue;

            Double_t pt_min = hdata->GetZaxis()->GetBinLowEdge(ibin_pt);
            Double_t pt_max = hdata->GetZaxis()->GetBinUpEdge(ibin_pt);

            Double_t rg_min = hdata->GetXaxis()->GetBinLowEdge(ibin_rg);
            Double_t rg_max = hdata->GetXaxis()->GetBinUpEdge(ibin_rg);

            TH1D *hdata_mB = (TH1D *) hdata->ProjectionY(Form("hdata_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hsig_mB = (TH1D *) hsig->ProjectionY(Form("hsig_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hbkg_mB = (TH1D *) hbkg->ProjectionY(Form("hbkg_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            float range_low =  0.;
            float range_high = 0.2;
            float sieie_cut =  0.2; 

            Templates* templates = new Templates(hdata_mB, hsig_mB, hbkg_mB, divisions);
            Purity* purity = new Purity();
            purity->fit(templates, hdata_mB, range_low, range_high, sieie_cut);

            std::cout << "nsig: " << purity->nsig << std::endl;
            std::cout << "purity: " << purity->purity << std::endl;
            std::cout << "chisq: " << purity->chisq << std::endl;
            std::cout << "ndf: " << purity->ndf << std::endl;

            // TF1* f = new TF1("f", templates, &Templates::evaluate, range_low, range_high, 3);
            // f->SetParameters(hdata_mB->Integral(1, hdata_mB->GetNbinsX()), 0.25, 0.0);
            // f->SetParLimits(1, 0, 1);
            // f->FixParameter(2, 0.0);
            // // f->SetParLimits(2, -0.0001, 0.0001);

            // hdata_mB->Fit("f", "WL 0 Q", "", range_low, range_high);
            // hdata_mB->Fit("f", "WL 0 Q", "", range_low, range_high);
            // hdata_mB->Fit("f", "WL M 0", "", range_low, range_high);

            // std::cout<<ibin_rg<<"  Parameter 0 ="<<f->GetParameter(0)<<std::endl;
            // std::cout<<ibin_rg<<"  Parameter 1 ="<<f->GetParameter(1)<<std::endl;
            // std::cout<<ibin_rg<<"  Parameter 2 ="<<f->GetParameter(2)<<std::endl;

            // h_total_normalization->SetBinContent(ibin_rg, ibin_pt, f->GetParameter(0));
            // h_signal_fraction->SetBinContent(ibin_rg, ibin_pt, f->GetParameter(1));
        } // rg loop 
    } // pt loop

    // TFile *fout = new TFile(rootf_out, "recreate");
    // for (auto h : {hdata, hsig, hbkg}) {
    //     h->Write();
    // }
    // for (auto h : {h_signal_fraction, h_total_normalization}) {
    //     h->Write();
    // }
    // fout->Close();
}