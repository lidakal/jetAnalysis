#include "../../../../CFIT/cfit.h"

void do_template_fit_cfit()
{
    gROOT->SetBatch();
    gSystem->Load("../../../../CFIT/libCFIT.so");

    // Setup 
    TString label_out = "pseudo_data";
    bool mc = false; if (label_out.Contains("pseudo")) mc = true;
    TString fout_name = "histos/fitted_parameters_cfit_" + label_out + ".root";

    // Load histograms
    TString fin_name = "./histos/cfit_input.root";
    TFile *fin = new TFile(fin_name);

    TH2D *h_sig_fraction_true_training = (TH2D *) fin->Get("h_sig_fraction_true_training");
    TH2D *h_sig_fraction_true_testing = (TH2D *) fin->Get("h_sig_fraction_true_testing");

    // Get bins + create output histograms
    Int_t nbins_rg = h_sig_fraction_true_training->GetNbinsX();
    Int_t nbins_pt = h_sig_fraction_true_training->GetNbinsY();

    TH2D *h_sig_fraction = (TH2D *) h_sig_fraction_true_training->Clone("h_sig_fraction");
    h_sig_fraction->Reset();

    TH2D *h_bkg_bb_fraction = (TH2D *) h_sig_fraction->Clone("h_bkg_bb_fraction");
    TH2D *h_bkg_rest_fraction = (TH2D *) h_sig_fraction->Clone("h_bkg_rest_fraction");

    TH2D *h_sig_fraction_error = (TH2D *) h_sig_fraction->Clone("h_sig_fraction_error");
    TH2D *h_bkg_bb_fraction_error = (TH2D *) h_sig_fraction->Clone("h_bkg_bb_fraction_error");
    TH2D *h_bkg_rest_fraction_error = (TH2D *) h_sig_fraction->Clone("h_bkg_rest_fraction_error");

    std::vector<std::pair<int, int>> non_converge_bins;

    for (Int_t ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            // if (ibin_pt != 3) continue;
            // if (ibin_rg != 1) continue;

            // CFIT template fit
            float par[100];
            float err[100];

            CFIT::cfit *cf = new CFIT::cfit("Fit variable");
            cf->ProducePlots(true); // need to define "tag"
            cf->SetInputFile("./histos/cfit_input.root");

            TH1D *h_sig_training_mb = (TH1D *) fin->Get(Form("h_sig_training_mb_%d_%d", ibin_pt, ibin_rg));
            TH1D *h_bkg_bb_training_mb = (TH1D *) fin->Get(Form("h_bkg_bb_training_mb_%d_%d", ibin_pt, ibin_rg));
            TH1D *h_bkg_rest_training_mb = (TH1D *) fin->Get(Form("h_bkg_rest_training_mb_%d_%d", ibin_pt, ibin_rg));
            std::vector<TH1D *> templates = {h_sig_training_mb, h_bkg_bb_training_mb, h_bkg_rest_training_mb};

            TH1D *h_data;
            if (mc) {
                cf->SetData(Form("h_testing_mb_%d_%d", ibin_pt, ibin_rg));
                h_data = (TH1D *) fin->Get(Form("h_testing_mb_%d_%d", ibin_pt, ibin_rg));
            } else {
                cf->SetData(Form("h_data_mb_%d_%d", ibin_pt, ibin_rg));
                h_data = (TH1D *) fin->Get(Form("h_data_mb_%d_%d", ibin_pt, ibin_rg));
            }
            cf->AddTemplate("sig", Form("h_sig_training_mb_%d_%d", ibin_pt, ibin_rg),2);
            cf->AddTemplate("bkg_bb", Form("h_bkg_bb_training_mb_%d_%d", ibin_pt, ibin_rg),3);
            cf->AddTemplate("bkg_rest", Form("h_bkg_rest_training_mb_%d_%d", ibin_pt, ibin_rg),4);

            cf->Run();

            // std::cout << "\nibin_pt = " << ibin_pt << ", ibin_rg = " << ibin_rg << std::endl;

            float chi2 = cf->GetChisq();
            // std::cout << "chi2=" << chi2 << std::endl;

            double total = 0;
            int nPar = cf->GetNPar();
            for (int i = 0; i < nPar; i++)
                {
                    par[i] = cf->GetPar(i);
                    err[i] = cf->GetParErr(i);	
                    // std::cout << par[i] << " +- " << err[i] << ", err/parr = " << err[i]/par[i] << std::endl;
                    total += par[i] * templates[i]->Integral();
                    
                }
            // std::cout << "ndata = " << h_data->Integral() << std::endl;
            // std::cout << "total/ndata = " << total / h_data->Integral() << std::endl;

            // Note: the parameters from cfit are what to multiply the templates with
            //       ie. c1N1 + c2N2 + c3N3 = N
            //       we want the template fractions, so 
            //       sig_fraction = c1N1/N, etc

            double sig_fraction = par[0] * templates[0]->Integral() / h_data->Integral();
            double bkg_bb_fraction = par[1] * templates[1]->Integral() / h_data->Integral();
            double bkg_rest_fraction = par[2] * templates[2]->Integral() / h_data->Integral();

            double sig_fraction_low = (par[0] - err[0]) * templates[0]->Integral() / h_data->Integral();
            double sig_fraction_high = (par[0] + err[0]) * templates[0]->Integral() / h_data->Integral();
            double sigma_low = sig_fraction - sig_fraction_low;
            double sigma_high = - sig_fraction + sig_fraction_high;

            // std::cout << "signal fraction = " << sig_fraction << " + " << sigma_high << " - " << sigma_low << std::endl;

            delete cf;

            // if (status != 0) {
            //     std::cout << "\n\n!!!Fitting for ipt = " << ibin_pt 
            //               << ", irg = " << ibin_rg 
            //               << " did not converge\n\n" << std::endl;
            //     non_converge_bins.push_back(std::pair<int, int>(ibin_pt, ibin_rg));
            // }

            Double_t p0, p1, p2, errP0, errP1, errP2;
            p0 = sig_fraction;
            p1 = bkg_bb_fraction;
            p2 = bkg_rest_fraction;

            errP0 = sigma_low;
            // std::cout << "errP0 = " << errP0 << std::endl;

            // if (status == 0) {                       // check on fit status
                // fit->GetResult( 0, p0, errP0);
                // fit->GetResult( 1, p1, errP1);
                // fit->GetResult( 2, p2, errP2);

                h_sig_fraction->SetBinContent(ibin_rg, ibin_pt, p0);
                h_sig_fraction_error->SetBinContent(ibin_rg, ibin_pt, errP0);
                h_bkg_bb_fraction->SetBinContent(ibin_rg, ibin_pt, p1);
                // h_bkg_bb_fraction_error->SetBinContent(ibin_rg, ibin_pt, errP1);
                h_bkg_rest_fraction->SetBinContent(ibin_rg, ibin_pt, p2);
                // h_bkg_rest_fraction_error->SetBinContent(ibin_rg, ibin_pt, errP2);

                // plot as: sig->Scale(ndata*p0/(integral*width))
                // bool make_plots = false;
                // if (make_plots) {
                //     Double_t pt_min = h_data->GetZaxis()->GetBinLowEdge(ibin_pt);
                //     Double_t pt_max = h_data->GetZaxis()->GetBinUpEdge(ibin_pt);

                //     Double_t rg_min = h_data->GetXaxis()->GetBinLowEdge(ibin_rg);
                //     Double_t rg_max = h_data->GetXaxis()->GetBinUpEdge(ibin_rg);

                //     Double_t bin_width = h_data_mb->GetBinWidth(1);
                //     Double_t ndata = h_data_mb->Integral(1, nbins_mb);
                //     Double_t int0 = h_sig_mb->Integral(1, nbins_mb);
                //     Double_t int1 = h_bkg_bb_mb->Integral(1, nbins_mb);
                //     Double_t int2 = h_bkg_rest_mb->Integral(1, nbins_mb);

                //     h_sig_mb->Scale(ndata*p0/(int0*bin_width));
                //     h_bkg_bb_mb->Scale(ndata*p1/(int1*bin_width));
                //     h_bkg_rest_mb->Scale(ndata*p2/(int2*bin_width));

                //     h_data_mb->SetMarkerColor(kBlack);
                //     h_sig_mb->SetFillColor(kRed-7);
                //     h_bkg_bb_mb->SetFillColor(kBlue-3);
                //     h_bkg_rest_mb->SetFillColor(kGreen-6);
                //     h_sig_mb->SetFillStyle(1001);
                //     h_bkg_bb_mb->SetFillStyle(1001);
                //     h_bkg_rest_mb->SetFillStyle(1001);

                //     THStack *hStack_mb = new THStack("hStack_mb", "");
                //     hStack_mb->Add(h_sig_mb);
                //     hStack_mb->Add(h_bkg_bb_mb);
                //     hStack_mb->Add(h_bkg_rest_mb);

                //     TCanvas *c_mb = new TCanvas("c_mb", "", 1000, 800);
                //     hStack_mb->Draw("hist");
                //     h_data_mb->Draw("pe1 same");
                //     c_mb->Draw();
                // }                
            // }
        } // rg loop 
    } // pt loop

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");
    // for (auto h : {
    //                h_data,
    //                h_sig_training_dijet, h_sig_testing_dijet,
    //                h_bkg_bb_training_dijet, h_bkg_bb_testing_dijet,
    //                h_bkg_rest_training_dijet, h_bkg_rest_testing_dijet,
    //                h_sig_training_bjet, h_sig_testing_bjet,
    //                h_bkg_bb_training_bjet, h_bkg_bb_testing_bjet,
    //                h_bkg_rest_training_bjet, h_bkg_rest_testing_bjet,
    //                }) {
    //                 h->Write();
    //                }
    for (auto h : {h_sig_fraction, h_sig_fraction_error,
                   h_bkg_bb_fraction, //h_bkg_bb_fraction_error,
                   h_bkg_rest_fraction, //h_bkg_rest_fraction_error,
                   h_sig_fraction_true_training,
    //                h_bkg_bb_fraction_true_training, 
    //                h_bkg_rest_fraction_true_training,
                   h_sig_fraction_true_testing,
    //             //    h_bkg_bb_fraction_true_testing, 
    //             //    h_bkg_rest_fraction_true_testing,
                   }) {
                    h->Write();
    }
        
    fout->Close();
}