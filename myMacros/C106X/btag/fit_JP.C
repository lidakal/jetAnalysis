void fit_JP(TString observable="rg", bool tagged=false)
{
    TString label = "aggrTMVA_inclusive";
    label += (tagged ? "_tagged" : "");

    TString fin_name = "histos/data_" + label + "_histograms_for_fit_JP.root";
    std::cout << "fin: " << fin_name << std::endl;
    TString fout_name = "histos/" + observable + "_fit_result_JP_" + label + ".root";

    // Load data histogram
    TFile *fin_data = new TFile(fin_name);
    TH3D *h_data = (TH3D *) fin_data->Get("h_data_"+observable)->Clone("h_data");

    // Load dijet histograms
    TFile *fin_dijet = new TFile("histos/dijet_" + label + "_histograms_for_fit_JP.root");
    TH3D *h_dijet_b_training = (TH3D *) fin_dijet->Get("h_b_"+observable+"_training")->Clone("h_dijet_b_training");
    TH3D *h_dijet_b_testing = (TH3D *) fin_dijet->Get("h_b_"+observable+"_testing")->Clone("h_dijet_b_testing");
    TH3D *h_dijet_bb_training = (TH3D *) fin_dijet->Get("h_bb_"+observable+"_training")->Clone("h_dijet_bb_training");
    TH3D *h_dijet_bb_testing = (TH3D *) fin_dijet->Get("h_bb_"+observable+"_testing")->Clone("h_dijet_bb_testing");
    TH3D *h_dijet_c_training = (TH3D *) fin_dijet->Get("h_c_"+observable+"_training")->Clone("h_dijet_c_training");
    TH3D *h_dijet_c_testing = (TH3D *) fin_dijet->Get("h_c_"+observable+"_testing")->Clone("h_dijet_c_testing");
    TH3D *h_dijet_l_training = (TH3D *) fin_dijet->Get("h_l_"+observable+"_training")->Clone("h_dijet_l_training");
    TH3D *h_dijet_l_testing = (TH3D *) fin_dijet->Get("h_l_"+observable+"_testing")->Clone("h_dijet_l_testing");

    // Load bjet histograms
    TFile *fin_bjet = new TFile("histos/bjet_" + label + "_histograms_for_fit_JP.root");
    TH3D *h_bjet_b_training = (TH3D *) fin_bjet->Get("h_b_"+observable+"_training")->Clone("h_bjet_b_training");
    TH3D *h_bjet_b_testing = (TH3D *) fin_bjet->Get("h_b_"+observable+"_testing")->Clone("h_bjet_b_testing");
    TH3D *h_bjet_bb_training = (TH3D *) fin_bjet->Get("h_bb_"+observable+"_training")->Clone("h_bjet_bb_training");
    TH3D *h_bjet_bb_testing = (TH3D *) fin_bjet->Get("h_bb_"+observable+"_testing")->Clone("h_bjet_bb_testing");
    TH3D *h_bjet_c_training = (TH3D *) fin_bjet->Get("h_c_"+observable+"_training")->Clone("h_bjet_c_training");
    TH3D *h_bjet_c_testing = (TH3D *) fin_bjet->Get("h_c_"+observable+"_testing")->Clone("h_bjet_c_testing");
    TH3D *h_bjet_l_training = (TH3D *) fin_bjet->Get("h_l_"+observable+"_training")->Clone("h_bjet_l_training");
    TH3D *h_bjet_l_testing = (TH3D *) fin_bjet->Get("h_l_"+observable+"_testing")->Clone("h_bjet_l_testing");

    // Load mujet histograms
    // TFile *fin_mujet = new TFile("histos/mujet_" + label + "_histograms_for_fit_JP.root");
    // TH3D *h_mujet_b_training = (TH3D *) fin_mujet->Get("h_b_"+observable+"_training")->Clone("h_mujet_b_training");
    // TH3D *h_mujet_b_testing = (TH3D *) fin_mujet->Get("h_b_"+observable+"_testing")->Clone("h_mujet_b_testing");
    // TH3D *h_mujet_bb_training = (TH3D *) fin_mujet->Get("h_bb_"+observable+"_training")->Clone("h_mujet_bb_training");
    // TH3D *h_mujet_bb_testing = (TH3D *) fin_mujet->Get("h_bb_"+observable+"_testing")->Clone("h_mujet_bb_testing");
    // TH3D *h_mujet_c_training = (TH3D *) fin_mujet->Get("h_c_"+observable+"_training")->Clone("h_mujet_c_training");
    // TH3D *h_mujet_c_testing = (TH3D *) fin_mujet->Get("h_c_"+observable+"_testing")->Clone("h_mujet_c_testing");
    // TH3D *h_mujet_l_training = (TH3D *) fin_mujet->Get("h_l_"+observable+"_training")->Clone("h_mujet_l_training");
    // TH3D *h_mujet_l_testing = (TH3D *) fin_mujet->Get("h_l_"+observable+"_testing")->Clone("h_mujet_l_testing");

    // Create the templates 
    TH3D *h_bbb = (TH3D *) h_dijet_b_training->Clone("h_bbb");
    h_bbb->Add(h_dijet_b_testing);
    h_bbb->Add(h_dijet_bb_training);
    h_bbb->Add(h_dijet_bb_testing);
    h_bbb->Add(h_bjet_b_training);
    h_bbb->Add(h_bjet_b_testing);
    h_bbb->Add(h_bjet_bb_training);
    h_bbb->Add(h_bjet_bb_testing);
    // h_bbb->Add(h_mujet_b_training);
    // h_bbb->Add(h_mujet_b_testing);
    // h_bbb->Add(h_mujet_bb_training);
    // h_bbb->Add(h_mujet_bb_testing);

    TH3D *h_c = (TH3D *) h_dijet_c_training->Clone("h_c");
    h_c->Add(h_dijet_c_testing);
    // h_c->Add(h_mujet_c_training);
    // h_c->Add(h_mujet_c_testing);

    TH3D *h_l = (TH3D *) h_dijet_l_training->Clone("h_l");
    h_l->Add(h_dijet_l_testing);

    for (auto h : {h_data, h_bbb, h_c, h_l}) {
        // h->RebinX(h->GetNbinsX()); // rebin only in x
        // std::cout << "before rebin: " << h->Integral(1, h->GetNbinsX(), h->GetNbinsY()-1, h->GetNbinsY(), 1, 3) << std::endl;
        // std::cout << "nbinsy = " << h->GetNbinsY() << std::endl;
        if (tagged) h->RebinY(3);
        else h->RebinY(3);
        // std::cout << "after rebin: " << h->Integral(1, h->GetNbinsX(), h->GetNbinsY(), h->GetNbinsY(), 1, 3) << std::endl;
        // std::cout << "nbinsy = " << h->GetNbinsY() << std::endl;
    }

    // Create histograms to fill with fractions
    TH2D *h_bbb_f = (TH2D *) h_bbb->Project3D("zx")->Clone("h_bbb_f");
    h_bbb_f->Reset();
    h_bbb_f->SetTitle("b+bb fraction");

    TH2D *h_c_f = (TH2D *) h_bbb_f->Clone("h_c_f");
    h_c_f->SetTitle("c fraction");

    TH2D *h_l_f = (TH2D *) h_bbb_f->Clone("h_l_f");
    h_l_f->SetTitle("l fraction");

    // Loop over jet pt+observable bins
    int nbins_x = h_bbb->GetNbinsX();
    int nbins_jp = h_bbb->GetNbinsY();
    int nbins_pt = h_bbb->GetNbinsZ();

    std::vector<std::pair<int, int>> non_converge_bins;
    std::vector<double> unc;

    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        for (int ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
            // debug 
            // if (ibin_pt != 2) continue;
            // if (ibin_x != 4) continue;

            std::cout << "\n\nBeginning fit for ibint_pt=" << ibin_pt << ", ibin_x=" << ibin_x <<  "\n\n" << std::endl;

            // Make projections 
            TH1D *h_data_1d = (TH1D *) h_data->ProjectionY(Form("h_data_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bbb_1d = (TH1D *) h_bbb->ProjectionY(Form("h_bbb_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_c_1d = (TH1D *) h_c->ProjectionY(Form("h_c_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_l_1d = (TH1D *) h_l->ProjectionY(Form("h_l_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            // Get MC fractions as initial values 
            TH1D *h_dijet_b_training_1d = (TH1D *) h_dijet_b_training->ProjectionY(Form("h_dijet_b_training_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_bb_training_1d = (TH1D *) h_dijet_bb_training->ProjectionY(Form("h_dijet_bb_training_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_c_training_1d = (TH1D *) h_dijet_c_training->ProjectionY(Form("h_dijet_c_training_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_l_training_1d = (TH1D *) h_dijet_l_training->ProjectionY(Form("h_dijet_l_training_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            double int0 = h_dijet_b_training_1d->Integral();
            double int1 = h_dijet_bb_training_1d->Integral();
            double int2 = h_dijet_c_training_1d->Integral();
            double int3 = h_dijet_l_training_1d->Integral();
            double total = int0+int1+int2+int3;

            // TCanvas *c_tmp = new TCanvas("c_tmp", "", 800, 600);
            // h_data_1d->Draw();

            // std::cout << " h_data_1d->Integral(1,nbins_jp) = " << h_data_1d->Integral(1,nbins_jp) << std::endl;
            // std::cout << " h_bbb_1d->Integral(1,nbins_jp) = " << h_bbb_1d->Integral(1,nbins_jp) << std::endl;
            // std::cout << " h_c_1d->Integral(1,nbins_jp) = " << h_c_1d->Integral(1,nbins_jp) << std::endl;
            // std::cout << " h_l_1d->Integral(1,nbins_jp) = " << h_l_1d->Integral(1,nbins_jp) << std::endl;
            

            // Create the observable
            Double_t min_jp = h_data_1d->GetXaxis()->GetBinLowEdge(1);
            // Double_t min_jp = 0.;
            Double_t max_jp = h_data_1d->GetXaxis()->GetBinUpEdge(nbins_jp);
            RooRealVar jp(Form("jp_%d_%d", ibin_pt, ibin_x), "jp", min_jp, max_jp);
            jp.setBins(nbins_jp);

            // Create the RooDataHist object for the observed data + templates
            RooDataHist *dh_data_1d = new RooDataHist(Form("dh_data_1d_%d_%d", ibin_pt, ibin_x), "dh_data_1d", jp, RooFit::Import(*h_data_1d));
            RooDataHist *dh_bbb_1d = new RooDataHist(Form("dh_bbb_1d_%d_%d", ibin_pt, ibin_x), "dh_bbb_1d", jp, RooFit::Import(*h_bbb_1d));
            RooDataHist *dh_c_1d = new RooDataHist(Form("dh_c_1d_%d_%d", ibin_pt, ibin_x), "dh_c_1d", jp, RooFit::Import(*h_c_1d));
            RooDataHist *dh_l_1d = new RooDataHist(Form("dh_l_1d_%d_%d", ibin_pt, ibin_x), "dh_l_1d", jp, RooFit::Import(*h_l_1d));
 
            // Create the RooHistPdf objects for the template PDFs
            RooHistPdf bbb_template(Form("bbb_template_%d_%d", ibin_pt, ibin_x), "bbb_template", jp, *dh_bbb_1d);
            RooHistPdf c_template(Form("c_template_%d_%d", ibin_pt, ibin_x), "c_template", jp, *dh_c_1d);
            RooHistPdf l_template(Form("l_template_%d_%d", ibin_pt, ibin_x), "l_template", jp, *dh_l_1d);
            RooArgList template_list(bbb_template, c_template, l_template, Form("template_list_%d_%d", ibin_pt, ibin_x));

            // Create the RooRealVar for the fit parameter (e.g., fraction of template A)
            RooRealVar bbb_fraction_val(Form("bbb_fraction_val_%d_%d", ibin_pt, ibin_x),"bbb_fraction_val",0.9,0.,1.);
            RooRealVar c_fraction_val(Form("c_fraction_val_%d_%d", ibin_pt, ibin_x),"c_fraction_val",0.1,0.,1.); // c=(1-bbb)*c'
            RooArgList coeff_list(bbb_fraction_val, c_fraction_val, Form("coeff_list_%d_%d", ibin_pt, ibin_x));
            // l=1-bbb-c

            // Create the composite PDF using a linear combination of the template PDFs
            RooAddPdf model0(Form("model0_%d_%d", ibin_pt, ibin_x), "model0", template_list, coeff_list, true);
            RooFitResult* result = model0.fitTo(*dh_data_1d, RooFit::SumW2Error(true), RooFit::Save(), RooFit::CloneData(true), RooFit::PrintLevel(-1), RooFit::Strategy(1), RooFit::Minos(false)); // result is already given a unique name
            Int_t status = result->status();
            result->Print();

            std::cout << "covariance matrix:" << std::endl;
            (result->covarianceMatrix().Print());


            // Get the fitted parameter values
            double a = bbb_fraction_val.getValV();
            double da = bbb_fraction_val.getError();

            double b = c_fraction_val.getValV();
            double db = c_fraction_val.getError();

            if (status != 0) {
                std::cout << "\n\n\n\n!!!Fitting for ipt = " << ibin_pt 
                          << ", ix = " << ibin_x 
                          << " did not converge\n\n\n\n" << std::endl;
                non_converge_bins.push_back(std::pair<int, int>(ibin_pt, ibin_x));
                continue;
            }

            Double_t p0, p1, p2, errP0, errP1, errP2;
            p0 = a;
            p1 = (1-a)*b;
            p2 = (1-a)*(1-b);

            errP0 = da;
            errP1 = std::sqrt((b*b*da*da) + ((1-a)*(1-a)*db*db));
            errP2 = std::sqrt(((1-b)*(1-b)*da*da)+((1-a)*(1-a)*db*db)); // assuming non correlation

            h_bbb_f->SetBinContent(ibin_x, ibin_pt, p0);
            h_bbb_f->SetBinError(ibin_x, ibin_pt, errP0);

            h_c_f->SetBinContent(ibin_x, ibin_pt, p1);
            h_c_f->SetBinError(ibin_x, ibin_pt, errP1);

            h_l_f->SetBinContent(ibin_x, ibin_pt, p2);
            h_l_f->SetBinError(ibin_x, ibin_pt, errP2);

            // if (ibin_x<nbins_x) 
                unc.push_back(errP0/p0);
        } // observable bin loop
    } // pt bin loop

    std::cout << *max_element(unc.begin(), unc.end()) << std::endl;

    std::cout << "Fit did not converged for bins:" << std::endl;
    for (auto p : non_converge_bins) {
        std::cout << "- (" << p.first << ", " << p.second << ")" << std::endl;
    }

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_bbb_f, h_c_f, h_l_f}) {
        h->Write();
    }
    for (auto h : {h_bbb, h_c, h_l, h_data}) {
        h->Write();
    }

    // Calculate purity in MC
    TH3D *h_dijet_bbb = (TH3D *) h_dijet_b_testing->Clone("h_dijet_bbb");
    h_dijet_bbb->Add(h_dijet_bb_testing);
    h_dijet_bbb->Add(h_dijet_b_training);
    h_dijet_bbb->Add(h_dijet_bb_training);

    TH3D *h_dijet_all = (TH3D *) h_dijet_bbb->Clone("h_dijet_all");
    h_dijet_all->Add(h_dijet_c_testing);
    h_dijet_all->Add(h_dijet_c_training);
    h_dijet_all->Add(h_dijet_l_testing);
    h_dijet_all->Add(h_dijet_l_training);

    h_dijet_bbb->GetYaxis()->SetRange(1, h_dijet_bbb->GetNbinsY());
    h_dijet_all->GetYaxis()->SetRange(1, h_dijet_all->GetNbinsY());

    TH2D *h_dijet_bbb_2d = (TH2D *) h_dijet_bbb->Project3D("zx"); // h_b includes bjet entries
    TH2D *h_dijet_c_2d = (TH2D *) h_c->Project3D("zx");
    TH2D *h_dijet_l_2d = (TH2D *) h_l->Project3D("zx");
    TH2D *h_dijet_all_2d = (TH2D *) h_dijet_all->Project3D("zx");

    // h_dijet_bbb_2d->Rebin(h_dijet_bbb_2d->GetNbinsX()); // rebin only in x
    // h_dijet_all_2d->Rebin(h_dijet_all_2d->GetNbinsX()); // rebin only in x

    TH2D *h_bbb_f_mc = (TH2D *) h_dijet_bbb_2d->Clone("h_bbb_f_mc");
    h_bbb_f_mc->Reset();
    h_bbb_f_mc->Divide(h_dijet_bbb_2d, h_dijet_all_2d, 1., 1., "b");

    TH2D *h_c_f_mc = (TH2D *) h_dijet_c_2d->Clone("h_c_f_mc");
    h_c_f_mc->Reset();
    h_c_f_mc->Divide(h_dijet_c_2d, h_dijet_all_2d, 1., 1., "b");

    TH2D *h_l_f_mc = (TH2D *) h_dijet_l_2d->Clone("h_l_f_mc");
    h_l_f_mc->Reset();
    h_l_f_mc->Divide(h_dijet_l_2d, h_dijet_all_2d, 1., 1., "b");

    h_bbb_f_mc->Write();
    h_c_f_mc->Write();
    h_l_f_mc->Write();

    fout->Close();
}