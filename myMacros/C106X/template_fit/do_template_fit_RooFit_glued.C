#include "Purity.h"

void do_template_fit_RooFit_glued(TString observable="rg", TString jer_opt="nom", TString jec_opt="nom")
{
    // glue light+c to sig 

    // Setup 
    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;
    TString sample = "aggrTMVA_XXT";

    // Load data
    TString fin_name_data = "./histos/data_PF40to100_" + sample + "_histograms.root";
    TFile *fin_data = new TFile(fin_name_data);
    TH3D *h_data = (TH3D *) fin_data->Get("h_data_" + observable)->Clone("h_data");

    // Load dijet MC
    TString fin_mc_dijet_name = "./histos/dijet_PF40_" + sample + "_histograms" + suffix + ".root";
    TFile *fin_mc_dijet = new TFile(fin_mc_dijet_name);

    TH3D *h_sig_training_dijet = (TH3D *) fin_mc_dijet->Get("h_sig_" + observable + "_training")->Clone("h_sig_training_dijet");
    TH3D *h_bkg_bb_training_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_bb_" + observable + "_training")->Clone("h_bkg_bb_training_dijet");
    TH3D *h_bkg_rest_training_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_rest_" + observable + "_training")->Clone("h_bkg_rest_training_dijet");

    TH3D *h_sig_testing_dijet = (TH3D *) fin_mc_dijet->Get("h_sig_" + observable + "_testing")->Clone("h_sig_testing_dijet");
    TH3D *h_bkg_bb_testing_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_bb_" + observable + "_testing")->Clone("h_bkg_bb_testing_dijet");
    TH3D *h_bkg_rest_testing_dijet = (TH3D *) fin_mc_dijet->Get("h_bkg_rest_" + observable + "_testing")->Clone("h_bkg_rest_testing_dijet");

    // Load bjet MC 
    TString fin_mc_bjet_name = "./histos/bjet_PF40_" + sample + "_histograms" + suffix + ".root";
    TFile *fin_mc_bjet = new TFile(fin_mc_bjet_name);

    TH3D *h_sig_training_bjet = (TH3D *) fin_mc_bjet->Get("h_sig_" + observable + "_training")->Clone("h_sig_training_bjet");
    TH3D *h_bkg_bb_training_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_bb_" + observable + "_training")->Clone("h_bkg_bb_training_bjet");
    TH3D *h_bkg_rest_training_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_rest_" + observable + "_training")->Clone("h_bkg_rest_training_bjet");

    TH3D *h_sig_testing_bjet = (TH3D *) fin_mc_bjet->Get("h_sig_" + observable + "_testing")->Clone("h_sig_testing_bjet");
    TH3D *h_bkg_bb_testing_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_bb_" + observable + "_testing")->Clone("h_bkg_bb_testing_bjet");
    TH3D *h_bkg_rest_testing_bjet = (TH3D *) fin_mc_bjet->Get("h_bkg_rest_" + observable + "_testing")->Clone("h_bkg_rest_testing_bjet");

    // Fix statistical uncertainties on all histograms
    for (auto h : {h_data, 
                   h_sig_training_dijet, h_sig_testing_dijet,
                   h_bkg_bb_training_dijet, h_bkg_bb_testing_dijet,
                   h_bkg_rest_training_dijet, h_bkg_rest_testing_dijet,
                   h_sig_training_bjet, h_sig_testing_bjet,
                   h_bkg_bb_training_bjet, h_bkg_bb_testing_bjet,
                   h_bkg_rest_training_bjet, h_bkg_rest_testing_bjet,
                   }) {
                    h->Sumw2();
                   }

    // Calculate true fractions from dijet MC
    TH3D *h_all_training_dijet = (TH3D *) h_sig_training_dijet->Clone("h_all_training_dijet");
    h_all_training_dijet->Add(h_bkg_bb_training_dijet);
    h_all_training_dijet->Add(h_bkg_rest_training_dijet);
    
    TH2D *h_all_training_dijet_2d = (TH2D *) h_all_training_dijet->Project3D("zx")->Clone("h_all_training_dijet_2d");
    TH2D *h_sig_training_dijet_2d = (TH2D *) h_sig_training_dijet->Project3D("zx")->Clone("h_sig_training_dijet_2d");
    TH2D *h_bkg_bb_training_dijet_2d = (TH2D *) h_bkg_bb_training_dijet->Project3D("zx")->Clone("h_bkg_bb_training_dijet_2d");
    TH2D *h_bkg_rest_training_dijet_2d = (TH2D *) h_bkg_rest_training_dijet->Project3D("zx")->Clone("h_bkg_rest_training_dijet_2d");

    TH2D *h_sig_fraction_true_training = (TH2D *) h_sig_training_dijet_2d->Clone("h_sig_fraction_true_training");
    h_sig_fraction_true_training->Divide(h_sig_fraction_true_training, h_all_training_dijet_2d, 1., 1., "b");
    TH2D *h_bkg_bb_fraction_true_training = (TH2D *) h_bkg_bb_training_dijet_2d->Clone("h_bkg_bb_fraction_true_training");
    h_bkg_bb_fraction_true_training->Divide(h_bkg_bb_fraction_true_training, h_all_training_dijet_2d, 1., 1., "b");
    TH2D *h_bkg_rest_fraction_true_training = (TH2D *) h_bkg_rest_training_dijet_2d->Clone("h_bkg_rest_fraction_true_training");
    h_bkg_rest_fraction_true_training->Divide(h_bkg_rest_fraction_true_training, h_all_training_dijet_2d, 1., 1., "b");

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
    Int_t nbins_x = h_data->GetNbinsX();
    Int_t nbins_mb = h_data->GetNbinsY();
    Int_t nbins_pt = h_data->GetNbinsZ();

    TH2D *h_sig_fraction = (TH2D *) h_data->Project3D("zx")->Clone("h_sig_fraction");
    h_sig_fraction->SetTitle("x=observable, y=jtpt, fraction / error");
    h_sig_fraction->Reset();

    TH2D *h_bkg_bb_fraction = (TH2D *) h_sig_fraction->Clone("h_bkg_bb_fraction");
    TH2D *h_bkg_rest_fraction = (TH2D *) h_sig_fraction->Clone("h_bkg_rest_fraction");

    TH2D *h_sig_fraction_error = (TH2D *) h_sig_fraction->Clone("h_sig_fraction_error");
    TH2D *h_bkg_bb_fraction_error = (TH2D *) h_sig_fraction->Clone("h_bkg_bb_fraction_error");
    TH2D *h_bkg_rest_fraction_error = (TH2D *) h_sig_fraction->Clone("h_bkg_rest_fraction_error");

    std::vector<std::pair<int, int>> non_converge_bins;
    std::vector<std::pair<std::pair<int, int>, double>> large_edms; 

    for (Int_t ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        for (Int_t ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
            // if (ibin_pt != 2) continue;
            // if (ibin_x != 7) continue;

            std::cout << "\n\n\n\n -------- Beginning the fit of (" << ibin_pt << ", " << ibin_x << ")\n\n\n\n" << std::endl; 

            // Make projections
            TH1D *h_data_mb = (TH1D *) h_data->ProjectionY(Form("h_data_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            TH1D *h_sig_training_dijet_mb = (TH1D *) h_sig_training_dijet->ProjectionY(Form("h_sig_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_dijet_mb = (TH1D *) h_bkg_bb_training_dijet->ProjectionY(Form("h_bkg_bb_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_dijet_mb = (TH1D *) h_bkg_rest_training_dijet->ProjectionY(Form("h_bkg_rest_training_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            TH1D *h_sig_testing_dijet_mb = (TH1D *) h_sig_testing_dijet->ProjectionY(Form("h_sig_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_dijet_mb = (TH1D *) h_bkg_bb_testing_dijet->ProjectionY(Form("h_bkg_bb_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_dijet_mb = (TH1D *) h_bkg_rest_testing_dijet->ProjectionY(Form("h_bkg_rest_testing_dijet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            TH1D *h_sig_training_bjet_mb = (TH1D *) h_sig_training_bjet->ProjectionY(Form("h_sig_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_bjet_mb = (TH1D *) h_bkg_bb_training_bjet->ProjectionY(Form("h_bkg_bb_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_bjet_mb = (TH1D *) h_bkg_rest_training_bjet->ProjectionY(Form("h_bkg_rest_training_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            TH1D *h_sig_testing_bjet_mb = (TH1D *) h_sig_testing_bjet->ProjectionY(Form("h_sig_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_testing_bjet_mb = (TH1D *) h_bkg_bb_testing_bjet->ProjectionY(Form("h_bkg_bb_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_testing_bjet_mb = (TH1D *) h_bkg_rest_testing_bjet->ProjectionY(Form("h_bkg_rest_testing_bjet_mb_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            // Calculate true fractions to be used as initial values for the fit
            double int0 = h_sig_training_dijet_mb->Integral(1, nbins_mb)+h_sig_testing_dijet_mb->Integral(1, nbins_mb);
            double int1 = h_bkg_bb_training_dijet_mb->Integral(1, nbins_mb)+h_bkg_bb_testing_dijet_mb->Integral(1, nbins_mb);
            double int2 = h_bkg_rest_training_dijet_mb->Integral(1, nbins_mb)+h_bkg_rest_testing_dijet_mb->Integral(1, nbins_mb);

            std::cout << "int0=" << int0 << std::endl;
            std::cout << "int1=" << int1 << std::endl;
            std::cout << "int2=" << int2 << std::endl;


            double sig_fraction_true = int0 / (int0 + int1 + int2); 
            double bkg_bb_fraction_true = int1 / (int0 + int1 + int2);
            std::cout << "sig_fraction_true=" << sig_fraction_true << std::endl;
            std::cout << "bkg_bb_fraction_true=" << bkg_bb_fraction_true << std::endl;

            // Compile dijet + bjet training and testing - no need to add them all together
            TH1D *h_sig_mb = (TH1D *) h_sig_training_dijet_mb->Clone(Form("h_sig_mb_%d_%d", ibin_pt, ibin_x));
            h_sig_mb->Add(h_sig_testing_dijet_mb);
            h_sig_mb->Add(h_sig_training_bjet_mb);
            h_sig_mb->Add(h_sig_testing_bjet_mb);
            h_sig_mb->Scale(1/h_sig_mb->Integral(1, nbins_mb));

            TH1D *h_bkg_bb_mb = (TH1D *) h_bkg_bb_training_dijet_mb->Clone(Form("h_bkg_bb_mb_%d_%d", ibin_pt, ibin_x));
            h_bkg_bb_mb->Add(h_bkg_bb_testing_dijet_mb);
            h_bkg_bb_mb->Add(h_bkg_bb_training_bjet_mb);
            h_bkg_bb_mb->Add(h_bkg_bb_testing_bjet_mb);
            h_bkg_bb_mb->Scale(1/h_bkg_bb_mb->Integral(1, nbins_mb));

            TH1D *h_bkg_rest_mb = (TH1D *) h_bkg_rest_training_dijet_mb->Clone(Form("h_bkg_rest_mb_%d_%d", ibin_pt, ibin_x));
            h_bkg_rest_mb->Add(h_bkg_rest_testing_dijet_mb);
            h_bkg_rest_mb->Add(h_bkg_rest_training_bjet_mb);
            h_bkg_rest_mb->Add(h_bkg_rest_testing_bjet_mb);
            h_bkg_rest_mb->Scale(1/h_bkg_rest_mb->Integral(1, nbins_mb));
            std::cout << "h_bkg_rest_mb->GetEntries()=" << h_bkg_rest_mb->GetEntries() << std::endl; 

            // add together light+c to sig with nominal MC ratio
            // a sig + b bb + c light = n, a+b+c=1, a+c=1-b
            // c'=c/(a+c)=(1-a-b)/(1-b)
            // a'=a/(a+c)=a/(1-b)=1-c'
            double cl_frac = (1 - sig_fraction_true - bkg_bb_fraction_true) / (1 - bkg_bb_fraction_true); // nominal
            // cl_frac = 2*cl_frac; // systematic
            // cl_frac = 0.; // systematic
            double sig_frac = 1 - cl_frac;
            h_sig_mb->Add(h_sig_mb, h_bkg_rest_mb, sig_frac, 1-sig_frac);

            // ***** Case 0 - 'Binned fitting' *****

            // Create the observable
            Double_t min_mb = h_data_mb->GetXaxis()->GetBinLowEdge(1);
            Double_t max_mb = h_data_mb->GetXaxis()->GetBinUpEdge(nbins_mb);
            RooRealVar mb(Form("mb_%d_%d", ibin_pt, ibin_x), "mb", min_mb, max_mb);
            mb.setBins(nbins_mb);

            // Create the RooDataHist object for the observed data + templates
            RooDataHist *dh_data_mb = new RooDataHist(Form("dh_data_mb_%d_%d", ibin_pt, ibin_x), "dh_data_mb", mb, RooFit::Import(*h_data_mb));
            RooDataHist *dh_sig_mb = new RooDataHist(Form("dh_sig_mb_%d_%d", ibin_pt, ibin_x), "dh_sig_mb", mb, RooFit::Import(*h_sig_mb));
            RooDataHist *dh_bkg_mb = new RooDataHist(Form("h_bkg_mb_%d_%d", ibin_pt, ibin_x), "dh_bkg_mb", mb, RooFit::Import(*h_bkg_bb_mb));
 
            // Create the RooHistPdf objects for the template PDFs
            RooHistPdf sig_template(Form("sig_template_%d_%d", ibin_pt, ibin_x), "sig_template", mb, *dh_sig_mb);
            RooHistPdf bkg_template(Form("bkg_template_%d_%d", ibin_pt, ibin_x), "bkg_template", mb, *dh_bkg_mb);
            RooArgList template_list(sig_template, bkg_template, Form("template_list_%d_%d", ibin_pt, ibin_x));

            // Create the RooRealVar for the fit parameter (e.g., fraction of template A)
            // debug 
            // sig_fraction_true = 0.5;
            RooRealVar sig_fraction_val(Form("sig_fraction_val_%d_%d", ibin_pt, ibin_x),"sig_fraction_val",1-bkg_bb_fraction_true,0.,1.);

            // Create the composite PDF using a linear combination of the template PDFs
            RooAddPdf model0(Form("model0_%d_%d", ibin_pt, ibin_x), "model0", template_list, sig_fraction_val, true);
            RooFitResult* result = model0.fitTo(*dh_data_mb, RooFit::SumW2Error(true), RooFit::Save(), RooFit::CloneData(true), RooFit::PrintLevel(2), RooFit::Strategy(1), RooFit::Minos(false)); // result is already given a unique name
            Int_t status = result->status();
            result->Print();

            std::cout << "covariance matrix:" << std::endl;
            (result->covarianceMatrix().Print());

            // if (result->edm()>1e-3) {
            //     std::pair<int, int> bin(ibin_pt, ibin_x);
            //     std::pair<std::pair<int, int>, double> edm(bin, result->edm());
            //     large_edms.push_back(edm);
            // }

            // Get the fitted parameter values
            double a = sig_fraction_val.getValV();
            double da = sig_fraction_val.getError();

            if (status != 0) {
                std::cout << "\n\n\n\n!!!Fitting for ipt = " << ibin_pt 
                          << ", ix = " << ibin_x 
                          << " did not converge\n\n\n\n" << std::endl;
                non_converge_bins.push_back(std::pair<int, int>(ibin_pt, ibin_x));
                continue;
            }

            Double_t p0, p1, p2, errP0, errP1, errP2;
            p0 = a*sig_frac;
            p1 = 1-a;
            p2 = a*(1-sig_frac);

            errP0 = da*sig_frac;
            errP1 = da;
            errP2 = da*(1-sig_frac);
            // std::cout << "errP0 = " << errP0 << std::endl;
            std::cout << "a'/a = " << p0/sig_fraction_true << std::endl;
            std::cout << "c'/c = " << p2/(1-sig_fraction_true-bkg_bb_fraction_true) << std::endl;
            std::cout << "c=" << (1-sig_fraction_true-bkg_bb_fraction_true) << ", c'=" << p2 << std::endl;
            std::cout << "a=" << sig_fraction_true << ", a'=" << p0 << std::endl;
            std::cout << "b=" << bkg_bb_fraction_true << ", b'=" << p1 << std::endl;

            h_sig_fraction->SetBinContent(ibin_x, ibin_pt, p0);
            h_sig_fraction->SetBinError(ibin_x, ibin_pt, errP0);
            h_sig_fraction_error->SetBinContent(ibin_x, ibin_pt, errP0);
            h_bkg_bb_fraction->SetBinContent(ibin_x, ibin_pt, p1);
            h_bkg_bb_fraction->SetBinError(ibin_x, ibin_pt, errP1);
            h_bkg_bb_fraction_error->SetBinContent(ibin_x, ibin_pt, errP1);
            h_bkg_rest_fraction->SetBinContent(ibin_x, ibin_pt, p2);
            h_bkg_rest_fraction->SetBinError(ibin_x, ibin_pt, errP2);
            h_bkg_rest_fraction_error->SetBinContent(ibin_x, ibin_pt, errP2);

        } // observable bin loop 
    } // jet pt loop

    for (auto p : non_converge_bins) {
        std::cout << "Fit did not converge for (" << p.first << ", " << p.second << ")" << std::endl;
    }

    std::cout << "bins with large edm:" << std::endl;
    for (auto p : large_edms) {
        std::cout << "(" << (p.first).first << ", " << (p.first).second << ") = " << p.second << std::endl;
    }

    TString fout_name = "histos/fitted_parameters_RooFit_data_" + sample + "_" + observable + "_glued" + suffix + ".root";
    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");
    for (auto h : {
                   h_data,
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
                //    h_sig_fraction_true_testing,
                //    h_bkg_bb_fraction_true_testing, 
                //    h_bkg_rest_fraction_true_testing,
                   }) {
                    h->Write();
    }
        
    fout->Close();
}