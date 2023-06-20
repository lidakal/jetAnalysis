#include "Purity.h"

void do_template_fit_fractionFitter()
{
    // Setup 
    TString label_out = "data";

    // Load training MC
    TString sample = "dijet";
    TString label = "aggrTMVA_withNb";
    TString fin_mc_name = "./histos/qcd_" + sample + "_" + label + "_data_for_template_fit.root";
    TString rootf_out = "histos/qcd_" + sample + "_" + label + "_fitted_parameters_" + label_out + ".root";
    TFile *fin_mc = new TFile(fin_mc_name);

    TH3D *hsig = (TH3D *) fin_mc->Get("hsig");
    TH3D *hbkg_bb = (TH3D *) fin_mc->Get("hbkg_bb");
    TH3D *hbkg_rest = (TH3D *) fin_mc->Get("hbkg_rest");

    // Load data
    TString label_data = "aggrTMVA_v2";
    TString fin_name_data = "./histos/data_" + label_data + "_raw_full.root";
    TFile *fin_data = new TFile(fin_name_data);
    TH3D *hdata = (TH3D *) fin_data->Get("hdata");

    // Load pseudo data
    TH3D *hdata_mc = (TH3D *) fin_mc->Get("hdata_mc");
    TH3D *hdata_sig = (TH3D *) fin_mc->Get("hdata_sig");
    TH3D *hdata_bkg_bb = (TH3D *) fin_mc->Get("hdata_bkg_bb");
    TH3D *hdata_bkg_rest = (TH3D *) fin_mc->Get("hdata_bkg_rest");

    if (!label_out.Contains("pseudo")) {
        // if running on data, combine training + testing MC
        hsig->Add(hdata_sig);
        hbkg_bb->Add(hdata_bkg_bb);
        hbkg_rest->Add(hdata_bkg_rest);
    }

    // TH3D *hdata = (TH3D *) hdata_mc->Clone("hdata");

    // debug
    // TH3D *hdata = (TH3D *) hsig->Clone("hdata");
    // hdata->Add(hbkg_bb);
    // hdata->Add(hbkg_rest);

    // Get bins + create output histogram
    Int_t nbins_rg = hdata->GetNbinsX();
    Int_t nbins_pt = hdata->GetNbinsZ();

    TH2D *h_sig_fraction = (TH2D *) hdata->Project3D("zx");
    h_sig_fraction->SetName("h_sig_fraction");
    h_sig_fraction->Reset();

    TH2D *h_bkg_bb_fraction = (TH2D *) h_sig_fraction->Clone("h_bkg_bb_fraction");
    TH2D *h_bkg_rest_fraction = (TH2D *) h_sig_fraction->Clone("h_bkg_rest_fraction");

    TH2D *h_sig_fraction_error = (TH2D *) h_sig_fraction->Clone("h_sig_fraction_error");
    TH2D *h_bkg_bb_fraction_error = (TH2D *) h_sig_fraction->Clone("h_bkg_bb_fraction_error");
    TH2D *h_bkg_rest_fraction_error = (TH2D *) h_sig_fraction->Clone("h_bkg_rest_fraction_error");

    // true fractions
    TH2D *h_sig_fraction_true = (TH2D *) h_sig_fraction->Clone("h_sig_fraction_true");
    TH2D *h_bkg_bb_fraction_true = (TH2D *) h_sig_fraction->Clone("h_bkg_bb_fraction_true");
    TH2D *h_bkg_rest_fraction_true = (TH2D *) h_sig_fraction->Clone("h_bkg_rest_fraction_true");

    const int divisions = 1;

    for (Int_t ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            // if (ibin_pt != 2) continue;
            // if (ibin_rg != 6) continue;

            // Calculate true fractions based on pseudo data
            TH1D *hdata_mc_mB = (TH1D *) hdata_mc->ProjectionY(Form("hdata_mc_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hdata_sig_mB = (TH1D *) hdata_sig->ProjectionY(Form("hdata_sig_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hdata_bkg_bb_mB = (TH1D *) hdata_bkg_bb->ProjectionY(Form("hdata_bkg_bb_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hdata_bkg_rest_mB = (TH1D *) hdata_bkg_rest->ProjectionY(Form("hdata_bkg_rest_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            Int_t nbins_mB = hdata_mc_mB->GetNbinsX();

            Double_t sig_fraction_true = hdata_sig_mB->Integral(1, nbins_mB) / hdata_mc_mB->Integral(1, nbins_mB);
            h_sig_fraction_true->SetBinContent(ibin_rg, ibin_pt, sig_fraction_true);
            Double_t bkg_bb_fraction_true = hdata_bkg_bb_mB->Integral(1, nbins_mB) / hdata_mc_mB->Integral(1, nbins_mB);
            h_bkg_bb_fraction_true->SetBinContent(ibin_rg, ibin_pt, bkg_bb_fraction_true);
            Double_t bkg_rest_fraction_true = hdata_bkg_rest_mB->Integral(1, nbins_mB) / hdata_mc_mB->Integral(1, nbins_mB);
            h_bkg_rest_fraction_true->SetBinContent(ibin_rg, ibin_pt, bkg_rest_fraction_true);

            // Do the fit using training MC 
            TH1D *hdata_mB = (TH1D *) hdata->ProjectionY(Form("hdata_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hsig_mB = (TH1D *) hsig->ProjectionY(Form("hsig_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hbkg_bb_mB = (TH1D *) hbkg_bb->ProjectionY(Form("hbkg_bb_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *hbkg_rest_mB = (TH1D *) hbkg_rest->ProjectionY(Form("hbkg_rest_mB_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            TObjArray *mc = new TObjArray(3);        // MC histograms are put in this array
            mc->Add(hsig_mB);
            mc->Add(hbkg_bb_mB);
            mc->Add(hbkg_rest_mB);
            TFractionFitter* fit = new TFractionFitter(hdata_mB, mc); // initialise
            // One parameter (fraction) per MC histogram
            fit->Constrain(0,0.0,1.0);               // constrain fraction 1 to be between 0 and 1
            fit->Constrain(1,0.0,1.0);               // constrain fraction 2 to be between 0 and 1
            fit->Constrain(2,0.0,1.0);               // constrain fraction 1 to be between 0 and 1
            //fit->SetRangeX(1,15);                    // use only the first 15 bins in the fit

            ROOT::Fit::Fitter *fitter = fit->GetFitter();
            ROOT::Fit::FitConfig fitConfig = fitter->Config();
            const Double_t init_values[3] = {sig_fraction_true, bkg_bb_fraction_true, bkg_rest_fraction_true};
            const Double_t steps[3] = {1e-3, 1e-3, 1e-3};
            fitConfig.SetParamsSettings(3, init_values, steps);
            // fitConfig.SetParamsSettings(1, &bkg_bb_fraction_true, 0.001);
            // fitConfig.SetParamsSettings(2, &bkg_rest_fraction_true, 0.001);

            Int_t status = fit->Fit();               // perform the fit
            cout << "fit status: " << status << endl;
            if (status != 0) 
                std::cout << "\n\n!!!Fitting for ipt = " << ibin_pt 
                          << ", irg = " << ibin_rg 
                          << " did not converge\n\n" << std::endl;

            Double_t p0, p1, p2, errP0, errP1, errP2;
            // if (status == 0) {                       // check on fit status
                fit->GetResult( 0, p0, errP0);
                fit->GetResult( 1, p1, errP1);
                fit->GetResult( 2, p2, errP2);

                h_sig_fraction->SetBinContent(ibin_rg, ibin_pt, p0);
                h_sig_fraction_error->SetBinContent(ibin_rg, ibin_pt, errP0);
                h_bkg_bb_fraction->SetBinContent(ibin_rg, ibin_pt, p1);
                h_bkg_bb_fraction_error->SetBinContent(ibin_rg, ibin_pt, errP1);
                h_bkg_rest_fraction->SetBinContent(ibin_rg, ibin_pt, p2);
                h_bkg_rest_fraction_error->SetBinContent(ibin_rg, ibin_pt, errP2);

                // plot as: sig->Scale(ndata*p0/(integral*width))
                bool make_plots = false;
                if (make_plots) {
                    Double_t pt_min = hdata->GetZaxis()->GetBinLowEdge(ibin_pt);
                    Double_t pt_max = hdata->GetZaxis()->GetBinUpEdge(ibin_pt);

                    Double_t rg_min = hdata->GetXaxis()->GetBinLowEdge(ibin_rg);
                    Double_t rg_max = hdata->GetXaxis()->GetBinUpEdge(ibin_rg);

                    Double_t bin_width = hdata_mB->GetBinWidth(1);
                    Double_t ndata = hdata_mB->Integral(1, nbins_mB);
                    Double_t int0 = hsig_mB->Integral(1, nbins_mB);
                    Double_t int1 = hbkg_bb_mB->Integral(1, nbins_mB);
                    Double_t int2 = hbkg_rest_mB->Integral(1, nbins_mB);

                    hsig_mB->Scale(ndata*p0/(int0*bin_width));
                    hbkg_bb_mB->Scale(ndata*p1/(int1*bin_width));
                    hbkg_rest_mB->Scale(ndata*p2/(int2*bin_width));

                    hdata_mB->SetMarkerColor(kBlack);
                    hsig_mB->SetFillColor(kRed-7);
                    hbkg_bb_mB->SetFillColor(kBlue-3);
                    hbkg_rest_mB->SetFillColor(kGreen-6);
                    hsig_mB->SetFillStyle(1001);
                    hbkg_bb_mB->SetFillStyle(1001);
                    hbkg_rest_mB->SetFillStyle(1001);

                    THStack *hStack_mB = new THStack("hStack_mB", "");
                    hStack_mB->Add(hsig_mB);
                    hStack_mB->Add(hbkg_bb_mB);
                    hStack_mB->Add(hbkg_rest_mB);

                    TCanvas *c_mB = new TCanvas("c_mb", "", 1000, 800);
                    hStack_mB->Draw("hist");
                    hdata_mB->Draw("pe1 same");
                    c_mB->Draw();
                }                
            // }
        } // rg loop 
    } // pt loop

    std::cout << "Creating file " << rootf_out << std::endl;
    TFile *fout = new TFile(rootf_out, "recreate");
    for (auto h : {hdata, // raw data
                   hsig, hbkg_bb, hbkg_rest, // training MC
                   hdata_sig, hdata_bkg_bb, hdata_bkg_rest // pseudo data
                   }) {
        h->Write();
    }
    for (auto h : {h_sig_fraction, h_sig_fraction_error,
                   h_bkg_bb_fraction, h_bkg_bb_fraction_error,
                   h_bkg_rest_fraction, h_bkg_rest_fraction_error,
                   h_sig_fraction_true,
                   h_bkg_bb_fraction_true, 
                   h_bkg_rest_fraction_true
                   }) {
        h->Write();
    }
        

    fout->Close();
}