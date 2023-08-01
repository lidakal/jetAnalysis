void create_cfit_input()
{
    // Load data
    TString label_data = "aggrTMVA_v2";
    TString fin_name_data = "./histos/data_" + label_data + "_histograms.root";
    TFile *fin_data = new TFile(fin_name_data);
    TH3D *h_data = (TH3D *) fin_data->Get("h_data");

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
    Int_t nbins_rg = h_data->GetNbinsX();
    Int_t nbins_mb = h_data->GetNbinsY();
    Int_t nbins_pt = h_data->GetNbinsZ();

    TString fout_name = "./histos/cfit_input.root";
    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_sig_fraction_true_training, h_sig_fraction_true_testing,
                   h_bkg_bb_fraction_true_training, h_bkg_bb_fraction_true_testing,
                   h_bkg_rest_fraction_true_training, h_bkg_rest_fraction_true_testing}) {
        // h->SetMarkerSize(800);
        h->Write();
    }

    for (Int_t ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        for (Int_t ibin_rg = 1; ibin_rg <= nbins_rg; ibin_rg++) {
            // if (ibin_pt != 2) continue;
            // if (ibin_rg != 3) continue;

            // Make projections
            TH1D *h_data_mb = (TH1D *) h_data->ProjectionY(Form("h_data_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

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
            TH1D *h_sig_training_mb = (TH1D *) h_sig_training_dijet_mb->Clone(Form("h_sig_training_mb_%d_%d", ibin_pt, ibin_rg));
            h_sig_training_mb->Add(h_sig_training_bjet_mb);

            TH1D *h_bkg_bb_training_mb = (TH1D *) h_bkg_bb_training_dijet_mb->Clone(Form("h_bkg_bb_training_mb_%d_%d", ibin_pt, ibin_rg));
            h_bkg_bb_training_mb->Add(h_bkg_bb_training_bjet_mb);

            TH1D *h_bkg_rest_training_mb = (TH1D *) h_bkg_rest_training_dijet_mb->Clone(Form("h_bkg_rest_training_mb_%d_%d", ibin_pt, ibin_rg));
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
            TH1D *h_sig_testing_mb = (TH1D *) h_sig_testing_dijet_mb->Clone(Form("h_sig_testing_mb_%d_%d", ibin_pt, ibin_rg));
            h_sig_testing_mb->Add(h_sig_testing_bjet_mb);
            h_sig_testing_mb->Scale(1 / h_sig_testing_mb->Integral(1, nbins_mb));

            TH1D *h_bkg_bb_testing_mb = (TH1D *) h_bkg_bb_testing_dijet_mb->Clone(Form("h_bkg_bb_testing_mb_%d_%d", ibin_pt, ibin_rg));
            h_bkg_bb_testing_mb->Add(h_bkg_bb_testing_bjet_mb);
            h_bkg_bb_testing_mb->Scale(1 / h_bkg_bb_testing_mb->Integral(1, nbins_mb));

            TH1D *h_bkg_rest_testing_mb = (TH1D *) h_bkg_rest_testing_dijet_mb->Clone(Form("h_bkg_rest_testing_mb_%d_%d", ibin_pt, ibin_rg));
            h_bkg_rest_testing_mb->Add(h_bkg_rest_testing_bjet_mb);
            h_bkg_rest_testing_mb->Scale(1 / h_bkg_rest_testing_mb->Integral(1, nbins_mb));

            TH1D *h_testing_mb = (TH1D *) h_sig_testing_mb->Clone(Form("h_testing_mb_%d_%d", ibin_pt, ibin_rg));
            h_testing_mb->Scale(c1);
            h_testing_mb->Add(h_bkg_bb_testing_mb, c2);
            h_testing_mb->Add(h_bkg_rest_testing_mb, c3);

            h_testing_mb->Scale(total);

            for (auto h : {h_data_mb, h_testing_mb, h_sig_training_mb, h_bkg_bb_training_mb, h_bkg_rest_training_mb}) {
                h->Write();
            }
        }
    }
    fout->Close();
    delete fout;
}