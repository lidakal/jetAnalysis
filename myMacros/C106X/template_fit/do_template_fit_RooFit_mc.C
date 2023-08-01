#include "Purity.h"

void do_template_fit_RooFit_mc()
{
    // Setup 
    TString label_out = "pseudo_data";
    TString fout_name = "histos/fitted_parameters_RooFit_" + label_out + ".root";

    // Load dijet MC
    TString fin_mc_dijet_name = "./histos/dijet_aggrTMVA_withHLT_v2_histograms.root";
    TFile *fin_mc_dijet = new TFile(fin_mc_dijet_name);

    TH3D *h_sig_training_dijet = (TH3D *) fin_mc_dijet->Get("h_sig_training")->Clone("h_sig_training_dijet");
    TH3D *h_bkg_bb_training_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_bb_training")->Clone("h_bkg_bb_training_dijet");
    TH3D *h_bkg_rest_training_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_rest_training")->Clone("h_bkg_rest_training_dijet");

    TH3D *h_sig_testing_dijet = (TH3D *) fin_mc_dijet->Get("h_sig_testing")->Clone("h_sig_testing_dijet");
    TH3D *h_bkg_bb_testing_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_bb_testing")->Clone("h_bkg_bb_testing_dijet");
    TH3D *h_bkg_rest_testing_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_rest_testing")->Clone("h_bkg_rest_testing_dijet");

    // Load bjet MC 
    TString fin_mc_bjet_name = "./histos/bjet_aggrTMVA_withHLT_histograms.root";
    TFile *fin_mc_bjet = new TFile(fin_mc_bjet_name);

    TH3D *h_sig_training_bjet = (TH3D *) fin_mc_bjet->Get("h_sig_training")->Clone("h_sig_training_bjet");
    TH3D *h_bkg_bb_training_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_bb_training")->Clone("h_bkg_bb_training_bjet");
    TH3D *h_bkg_rest_training_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_rest_training")->Clone("h_bkg_rest_training_bjet");

    TH3D *h_sig_testing_bjet = (TH3D *) fin_mc_bjet->Get("h_sig_testing")->Clone("h_sig_testing_bjet");
    TH3D *h_bkg_bb_testing_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_bb_testing")->Clone("h_bkg_bb_testing_bjet");
    TH3D *h_bkg_rest_testing_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_rest_testing")->Clone("h_bkg_rest_testing_bjet");

    // Fix statistical uncertainties on all histograms
    for (auto h : {
                   h_sig_training_dijet, h_sig_testing_dijet,
                   h_bkg_bb_training_dijet, h_bkg_bb_testing_dijet,
                   h_bkg_rest_training_dijet, h_bkg_rest_testing_dijet,
                   h_sig_training_bjet, h_sig_testing_bjet,
                   h_bkg_bb_training_bjet, h_bkg_bb_testing_bjet,
                   h_bkg_rest_training_bjet, h_bkg_rest_testing_bjet,
                   }) {
                    h->Sumw2();
                   }

    // Calculate true fractions from dijet MC for the training sample
    TH3D *h_all_training_dijet = (TH3D *) h_sig_training_dijet->Clone("h_all_training_dijet");
    h_all_training_dijet->Add(h_bkg_bb_training_dijet);
    h_all_training_dijet->Add(h_bkg_rest_training_dijet);
    
    TH2D *h_all_training_dijet_2d = (TH2D *) h_all_training_dijet->Project3D("zx")->Clone("h_all_training_dijet_2d");
    TH2D *h_sig_training_dijet_2d = (TH2D *) h_sig_training_dijet->Project3D("zx")->Clone("h_sig_training_dijet_2d");
    TH2D *h_bkg_bb_training_dijet_2d = (TH2D *) h_bkg_bb_training_dijet->Project3D("zx")->Clone("h_bkg_bb_training_dijet_2d");
    TH2D *h_bkg_rest_training_dijet_2d = (TH2D *) h_bkg_rest_training_dijet->Project3D("zx")->Clone("h_bkg_rest_training_dijet_2d");

    TH2D *h_sig_fraction_true_training = (TH2D *) h_sig_training_dijet_2d->Clone("h_sig_fraction_true_training");
    h_sig_fraction_true_training->Divide(h_all_training_dijet_2d);
    TH2D *h_bkg_bb_fraction_true_training = (TH2D *) h_bkg_bb_training_dijet_2d->Clone("h_bkg_bb_fraction_true_training");
    h_bkg_bb_fraction_true_training->Divide(h_all_training_dijet_2d);
    TH2D *h_bkg_rest_fraction_true_training = (TH2D *) h_bkg_rest_training_dijet_2d->Clone("h_bkg_rest_fraction_true_training");
    h_bkg_rest_fraction_true_training->Divide(h_all_training_dijet_2d);

    // Calculate true fractions from dijet MC for the testing sample
    TH3D *h_all_testing_dijet = (TH3D *) h_sig_testing_dijet->Clone("h_all_testing_dijet");
    h_all_testing_dijet->Add(h_bkg_bb_testing_dijet);
    h_all_testing_dijet->Add(h_bkg_rest_testing_dijet);
    
    TH2D *h_all_testing_dijet_2d = (TH2D *) h_all_testing_dijet->Project3D("zx")->Clone("h_all_testing_dijet_2d");
    TH2D *h_sig_testing_dijet_2d = (TH2D *) h_sig_testing_dijet->Project3D("zx")->Clone("h_sig_testing_dijet_2d");
    TH2D *h_bkg_bb_testing_dijet_2d = (TH2D *) h_bkg_bb_testing_dijet->Project3D("zx")->Clone("h_bkg_bb_testing_dijet_2d");
    TH2D *h_bkg_rest_testing_dijet_2d = (TH2D *) h_bkg_rest_testing_dijet->Project3D("zx")->Clone("h_bkg_rest_testing_dijet_2d");

    TH2D *h_sig_fraction_true_testing = (TH2D *) h_sig_testing_dijet_2d->Clone("h_sig_fraction_true_testing");
    h_sig_fraction_true_testing->Divide(h_all_testing_dijet_2d);
    TH2D *h_bkg_bb_fraction_true_testing = (TH2D *) h_bkg_bb_testing_dijet_2d->Clone("h_bkg_bb_fraction_true_testing");
    h_bkg_bb_fraction_true_testing->Divide(h_all_testing_dijet_2d);
    TH2D *h_bkg_rest_fraction_true_testing = (TH2D *) h_bkg_rest_testing_dijet_2d->Clone("h_bkg_rest_fraction_true_testing");
    h_bkg_rest_fraction_true_testing->Divide(h_all_testing_dijet_2d);


    // Merge MC after making projections -> it's easier

    // Get bins + create output histograms
    Int_t nbins_rg = h_sig_training_dijet->GetNbinsX();
    Int_t nbins_mb = h_sig_training_dijet->GetNbinsY();
    Int_t nbins_pt = h_sig_training_dijet->GetNbinsZ();

    TH2D *h_sig_fraction = (TH2D *) h_sig_training_dijet->Project3D("zx")->Clone("h_sig_fraction");
    h_sig_fraction->SetTitle("x=rg, y=jtpt, fraction / error");
    h_sig_fraction->Reset();

    TH2D *h_bkg_bb_fraction = (TH2D *) h_sig_fraction->Clone("h_bkg_bb_fraction");
    TH2D *h_bkg_rest_fraction = (TH2D *) h_sig_fraction->Clone("h_bkg_rest_fraction");

    TH2D *h_sig_fraction_error = (TH2D *) h_sig_fraction->Clone("h_sig_fraction_error");
    TH2D *h_bkg_bb_fraction_error = (TH2D *) h_sig_fraction->Clone("h_bkg_bb_fraction_error");
    TH2D *h_bkg_rest_fraction_error = (TH2D *) h_sig_fraction->Clone("h_bkg_rest_fraction_error");

    std::vector<std::pair<int, int>> non_converge_bins;

    for (Int_t ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            // if (ibin_pt != 2) continue;
            // if (ibin_rg != 3) continue;

            // Make projections
            TH1D *h_sig_training_dijet_mb = (TH1D *) h_sig_training_dijet->ProjectionY(Form("h_sig_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_dijet_mb = (TH1D *) h_bkg_bb_training_dijet->ProjectionY(Form("h_bkg_bb_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_dijet_mb = (TH1D *) h_bkg_rest_training_dijet->ProjectionY(Form("h_bkg_rest_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            TH1D *h_sig_testing_dijet_mb = (TH1D *) h_sig_testing_dijet->ProjectionY(Form("h_sig_testing_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_dijet_mb = (TH1D *) h_bkg_bb_testing_dijet->ProjectionY(Form("h_bkg_bb_testing_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_dijet_mb = (TH1D *) h_bkg_rest_testing_dijet->ProjectionY(Form("h_bkg_rest_testing_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            TH1D *h_sig_training_bjet_mb = (TH1D *) h_sig_training_bjet->ProjectionY(Form("h_sig_training_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_bjet_mb = (TH1D *) h_bkg_bb_training_bjet->ProjectionY(Form("h_bkg_bb_training_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_bjet_mb = (TH1D *) h_bkg_rest_training_bjet->ProjectionY(Form("h_bkg_rest_training_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            TH1D *h_sig_testing_bjet_mb = (TH1D *) h_sig_testing_bjet->ProjectionY(Form("h_sig_testing_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_bjet_mb = (TH1D *) h_bkg_bb_testing_bjet->ProjectionY(Form("h_bkg_bb_testing_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_bjet_mb = (TH1D *) h_bkg_rest_testing_bjet->ProjectionY(Form("h_bkg_rest_testing_bjet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            // Compile dijet + bjet training - no need to add them all together
            TH1D *h_sig_training_mb = (TH1D *) h_sig_training_dijet_mb->Clone("h_sig_training_mb");
            h_sig_training_mb->Add(h_sig_training_bjet_mb);

            TH1D *h_bkg_bb_training_mb = (TH1D *) h_bkg_bb_training_dijet_mb->Clone("h_bkg_bb_training_mb");
            h_bkg_bb_training_mb->Add(h_bkg_bb_training_bjet_mb);

            TH1D *h_bkg_rest_training_mb = (TH1D *) h_bkg_rest_training_dijet_mb->Clone("h_bkg_rest_training_mb");
            h_bkg_rest_training_mb->Add(h_bkg_rest_training_bjet_mb);

            // Compile dijet + bjet testing - add them all together with original fractions
            double int1 = h_sig_testing_dijet_mb->Integral(1, nbins_mb);
            double int2 = h_bkg_bb_testing_dijet_mb->Integral(1, nbins_mb);
            double int3 = h_bkg_rest_testing_dijet_mb->Integral(1, nbins_mb);
            double total = int1 + int2 + int3;
            double c1 = int1 / total;
            double c2 = int2 / total;
            double c3 = int3 / total;
            // h_testing_mb = total * (c1 * h_sig + c2 * h_bkg_bb + c3 * h_bkg_rest)
            TH1D *h_sig_testing_mb = (TH1D *) h_sig_testing_dijet_mb->Clone("h_sig_testing_mb");
            h_sig_testing_mb->Add(h_sig_testing_bjet_mb);
            h_sig_testing_mb->Scale(1 / h_sig_testing_mb->Integral(1, nbins_mb));

            TH1D *h_bkg_bb_testing_mb = (TH1D *) h_bkg_bb_testing_dijet_mb->Clone("h_bkg_bb_testing_mb");
            h_bkg_bb_testing_mb->Add(h_bkg_bb_testing_bjet_mb);
            h_bkg_bb_testing_mb->Scale(1 / h_bkg_bb_testing_mb->Integral(1, nbins_mb));

            TH1D *h_bkg_rest_testing_mb = (TH1D *) h_bkg_rest_testing_dijet_mb->Clone("h_bkg_rest_testing_mb");
            h_bkg_rest_testing_mb->Add(h_bkg_rest_testing_bjet_mb);
            h_bkg_rest_testing_mb->Scale(1 / h_bkg_rest_testing_mb->Integral(1, nbins_mb));

            TH1D *h_testing_mb = (TH1D *) h_sig_testing_mb->Clone("h_testing_mb");
            h_testing_mb->Scale(c1);
            h_testing_mb->Add(h_bkg_bb_testing_mb, c2);
            h_testing_mb->Add(h_bkg_rest_testing_mb, c3);

            h_testing_mb->Scale(total);

            // ***** Case 0 - 'Binned fitting' *****

            // Create the observable
            Double_t min_mb = h_testing_mb->GetXaxis()->GetBinLowEdge(1);
            Double_t max_mb = h_testing_mb->GetXaxis()->GetBinUpEdge(nbins_mb);
            RooRealVar mb("mb", "mb", min_mb, max_mb);
            mb.setBins(nbins_mb);

            // h_data_mb->Sumw2();
            // Normalize distributions
            // for (auto h : {
            //                 // h_data_mb, 
            //                 h_sig_mb, h_bkg_bb_mb, h_bkg_rest_mb
            //                 }) {
            //     h->Sumw2();
                // h->Scale(1/h->Integral());
            // }

            // Create the RooDataHist object for the observed data + templates
            RooDataHist *dh_testing_mb = new RooDataHist("dh_testing_mb", "dh_testing_mb", mb, RooFit::Import(*h_testing_mb));
            RooDataHist *dh_sig_training_mb = new RooDataHist("dh_sig_training_mb", "dh_sig_training_mb", mb, RooFit::Import(*h_sig_training_mb));
            RooDataHist *dh_bkg_bb_training_mb = new RooDataHist("dh_bkg_bb_training_mb", "dh_bkg_bb_training_mb", mb, RooFit::Import(*h_bkg_bb_training_mb));
            RooDataHist *dh_bkg_rest_training_mb = new RooDataHist("dh_bkg_rest_training_mb", "dh_bkg_rest_training_mb", mb, RooFit::Import(*h_bkg_rest_training_mb));
 
            // Create the RooHistPdf objects for the template PDFs
            RooHistPdf sig_template("sig_template", "sig_template", mb, *dh_sig_training_mb);
            RooHistPdf bkg_bb_template("bkg_bb_template", "bkg_bb_template", mb, *dh_bkg_bb_training_mb);
            RooHistPdf bkg_rest_template("bkg_rest_template", "bkg_rest_template", mb, *dh_bkg_rest_training_mb);
            RooArgList template_list(sig_template, bkg_bb_template, bkg_rest_template);

            // Create the RooRealVar for the fit parameter (e.g., fraction of template A)
            RooRealVar sig_fraction_val("sig_fraction_val","sig_fraction_val",c1,0.,1.);
            RooRealVar bkg_bb_fraction_val("bkg_bb_fraction_val","bkg_bb_fraction_val",(1-c1)*c2,0.,1.);
            // RooRealVar bkg_rest_fraction_val("bkg_rest_fraction_val","bkg_rest_fraction_val",bkg_rest_fraction_true,0.,1.);
            // bkg_rest_fraction = 1 - sig_fraction - bkg_bb_fraction
            RooArgList coeff_list(sig_fraction_val, bkg_bb_fraction_val);
            // RooArgList coeff_list(sig_fraction_val, bkg_bb_fraction_val, bkg_rest_fraction_val);          

            // Create the composite PDF using a linear combination of the template PDFs
            RooAddPdf model0("model0", "model0", template_list, coeff_list, true);
            RooFitResult *result = model0.fitTo(*dh_testing_mb, RooFit::SumW2Error(true), RooFit::Save());
            // RooFitResult *result = model0.fitTo(*dh_testing_mb, RooFit::Save());
            Int_t status = result->status();
            result->Print();

            // Get the fitted parameter values
            Double_t sig_fraction = sig_fraction_val.getValV();
            // Double_t bkg_bb_fraction = bkg_bb_fraction_val.getValV();
            // Double_t bkg_rest_fraction = 1. - sig_fraction - bkg_bb_fraction;
            Double_t bkg_bb_fraction = (1-sig_fraction)*bkg_bb_fraction_val.getValV();
            Double_t bkg_rest_fraction = 1. - sig_fraction - bkg_bb_fraction;

            if (status != 0) {
                std::cout << "\n\n!!!Fitting for ipt = " << ibin_pt 
                          << ", irg = " << ibin_rg 
                          << " did not converge\n\n" << std::endl;
                non_converge_bins.push_back(std::pair<int, int>(ibin_pt, ibin_rg));
            }

            Double_t p0, p1, p2, errP0, errP1, errP2;
            p0 = sig_fraction;
            p1 = bkg_bb_fraction;
            p2 = bkg_rest_fraction;

            errP0 = sig_fraction_val.getError();
            // std::cout << "errP0 = " << errP0 << std::endl;

            // if (status == 0) {                       // check on fit status
                // fit->GetResult( 0, p0, errP0);
                // fit->GetResult( 1, p1, errP1);
                // fit->GetResult( 2, p2, errP2);

                h_sig_fraction->SetBinContent(ibin_rg, ibin_pt, p0);
                h_sig_fraction_error->SetBinContent(ibin_rg, ibin_pt, errP0);
                h_bkg_bb_fraction->SetBinContent(ibin_rg, ibin_pt, p1);
                h_bkg_bb_fraction_error->SetBinContent(ibin_rg, ibin_pt, errP1);
                h_bkg_rest_fraction->SetBinContent(ibin_rg, ibin_pt, p2);
                h_bkg_rest_fraction_error->SetBinContent(ibin_rg, ibin_pt, errP2);

                // plot as: sig->Scale(ndata*p0/(integral*width))
                bool make_plots = false;
                if (make_plots) {
                    Double_t pt_min = h_sig_training_dijet->GetZaxis()->GetBinLowEdge(ibin_pt);
                    Double_t pt_max = h_sig_training_dijet->GetZaxis()->GetBinUpEdge(ibin_pt);

                    Double_t rg_min = h_sig_training_dijet->GetXaxis()->GetBinLowEdge(ibin_rg);
                    Double_t rg_max = h_sig_training_dijet->GetXaxis()->GetBinUpEdge(ibin_rg);

                    Double_t bin_width = h_testing_mb->GetBinWidth(1);
                    Double_t ndata = h_testing_mb->Integral(1, nbins_mb);
                    Double_t int0fit = h_sig_training_mb->Integral(1, nbins_mb);
                    Double_t int1fit = h_bkg_bb_training_mb->Integral(1, nbins_mb);
                    Double_t int2fit = h_bkg_rest_training_mb->Integral(1, nbins_mb);

                    h_sig_training_mb->Scale(ndata*p0/(int0fit*bin_width));
                    h_bkg_bb_training_mb->Scale(ndata*p1/(int1fit*bin_width));
                    h_bkg_rest_training_mb->Scale(ndata*p2/(int2fit*bin_width));

                    h_testing_mb->SetMarkerColor(kBlack);
                    h_sig_training_mb->SetFillColor(kRed-7);
                    h_bkg_bb_training_mb->SetFillColor(kBlue-3);
                    h_bkg_rest_training_mb->SetFillColor(kGreen-6);
                    h_sig_training_mb->SetFillStyle(1001);
                    h_bkg_bb_training_mb->SetFillStyle(1001);
                    h_bkg_rest_training_mb->SetFillStyle(1001);

                    THStack *hStack_mb = new THStack("hStack_mb", "");
                    hStack_mb->Add(h_sig_training_mb);
                    hStack_mb->Add(h_bkg_bb_training_mb);
                    hStack_mb->Add(h_bkg_rest_training_mb);

                    TCanvas *c_mb = new TCanvas("c_mb", "", 1000, 800);
                    hStack_mb->Draw("hist");
                    h_testing_mb->Draw("pe1 same");
                    c_mb->Draw();
                }                
            // }
        } // rg loop 
    } // pt loop

    for (auto p : non_converge_bins) {
        std::cout << "Fit did not converge for (" << p.first << ", " << p.second << ")" << std::endl;
    }

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");
    for (auto h : {
                   h_sig_training_dijet, h_sig_testing_dijet,
                   h_bkg_bb_training_dijet, h_bkg_bb_testing_dijet,
                   h_bkg_rest_training_dijet, h_bkg_rest_testing_dijet,
                   h_sig_training_bjet, h_sig_testing_bjet,
                   h_bkg_bb_training_bjet, h_bkg_bb_testing_bjet,
                   h_bkg_rest_training_bjet, h_bkg_rest_testing_bjet,
                   }) {
                    h->Write();
                   }
    for (auto h : {h_sig_fraction, h_sig_fraction_error,
                   h_bkg_bb_fraction, h_bkg_bb_fraction_error,
                   h_bkg_rest_fraction, h_bkg_rest_fraction_error,
                   h_sig_fraction_true_training,
                   h_bkg_bb_fraction_true_training, 
                   h_bkg_rest_fraction_true_training,
                   h_sig_fraction_true_testing,
                   h_bkg_bb_fraction_true_testing, 
                   h_bkg_rest_fraction_true_testing,
                   }) {
                    h->Write();
    }
        
    fout->Close();
}