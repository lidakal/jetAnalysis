void fit_JP(TString observable="rg")
{
    // TString label = "aggrTMVA_inclusive";
    TString label = "aggrTMVA_inclusive";

    // TString fin_name = "histos/data_" + label + "_histograms_for_fit_JP.root";
    TString fin_name = "histos/data_PF40to100_" + label + "_histograms_for_fit_JP.root";
    // TString fin_name = "histos/HighEGJet_Mu5_AK4PFJet30_" + label + "_histograms_for_fit_JP.root";
    std::cout << "fin: " << fin_name << std::endl;
    TString fout_name = "histos/" + observable + "_fit_result_JP_" + label + ".root";

    // Load data histogram
    TFile *fin_data = new TFile(fin_name);
    TH3D *h_data = (TH3D *) fin_data->Get("h_data_"+observable)->Clone("h_data");
    TH3D *h_data_tagged = (TH3D *) fin_data->Get("h_data_"+observable+"_tagged")->Clone("h_data_tagged");

    // Load dijet histograms
    // TFile *fin_dijet = new TFile("histos/dijet_calo60_" + label + "_histograms_for_fit_JP.root");
    TFile *fin_dijet = new TFile("histos/dijet_PF40_" + label + "_histograms_for_fit_JP.root");
    // TFile *fin_dijet = new TFile("histos/dijet_Mu5_" + label + "_histograms_for_fit_JP.root");
    TH3D *h_dijet_b = (TH3D *) fin_dijet->Get("h_b_"+observable)->Clone("h_dijet_b");
    TH3D *h_dijet_b_tagged = (TH3D *) fin_dijet->Get("h_b_"+observable+"_tagged")->Clone("h_dijet_b_tagged");
    TH3D *h_dijet_bb = (TH3D *) fin_dijet->Get("h_bb_"+observable)->Clone("h_dijet_bb");
    TH3D *h_dijet_bb_tagged = (TH3D *) fin_dijet->Get("h_bb_"+observable+"_tagged")->Clone("h_dijet_bb_tagged");
    TH3D *h_dijet_c = (TH3D *) fin_dijet->Get("h_c_"+observable)->Clone("h_dijet_c");
    TH3D *h_dijet_c_tagged = (TH3D *) fin_dijet->Get("h_c_"+observable+"_tagged")->Clone("h_dijet_c_tagged");
    TH3D *h_dijet_cc = (TH3D *) fin_dijet->Get("h_cc_"+observable)->Clone("h_dijet_cc");
    TH3D *h_dijet_cc_tagged = (TH3D *) fin_dijet->Get("h_cc_"+observable+"_tagged")->Clone("h_dijet_cc_tagged");
    TH3D *h_dijet_l = (TH3D *) fin_dijet->Get("h_l_"+observable)->Clone("h_dijet_l");
    TH3D *h_dijet_l_tagged = (TH3D *) fin_dijet->Get("h_l_"+observable+"_tagged")->Clone("h_dijet_l_tagged");

    // JP Swap 
    // TFile *fin_dijet_swap = new TFile("histos/dijet_calo60_withDataJPCalibration_" + label + "_histograms_for_fit_JP.root");
    // TH3D *h_dijet_b = (TH3D *) fin_dijet->Get("h_b_"+observable)->Clone("h_dijet_b");
    // TH3D *h_dijet_b_tagged = (TH3D *) fin_dijet->Get("h_b_"+observable+"_tagged")->Clone("h_dijet_b_tagged");
    // TH3D *h_dijet_bb = (TH3D *) fin_dijet->Get("h_bb_"+observable)->Clone("h_dijet_bb");
    // TH3D *h_dijet_bb_tagged = (TH3D *) fin_dijet->Get("h_bb_"+observable+"_tagged")->Clone("h_dijet_bb_tagged");
    // TH3D *h_dijet_c = (TH3D *) fin_dijet->Get("h_c_"+observable)->Clone("h_dijet_c");
    // TH3D *h_dijet_c_tagged = (TH3D *) fin_dijet->Get("h_c_"+observable+"_tagged")->Clone("h_dijet_c_tagged");
    
    // Load bjet histograms
    // TFile *fin_bjet = new TFile("histos/bjet_calo60_" + label + "_histograms_for_fit_JP.root");
    TFile *fin_bjet = new TFile("histos/bjet_PF40_" + label + "_histograms_for_fit_JP.root");
    // TFile *fin_bjet = new TFile("histos/bjet_Mu5_" + label + "_histograms_for_fit_JP.root");
    TH3D *h_bjet_b = (TH3D *) fin_bjet->Get("h_b_"+observable)->Clone("h_bjet_b");
    TH3D *h_bjet_b_tagged = (TH3D *) fin_bjet->Get("h_b_"+observable+"_tagged")->Clone("h_bjet_b_tagged");
    TH3D *h_bjet_bb = (TH3D *) fin_bjet->Get("h_bb_"+observable)->Clone("h_bjet_bb");
    TH3D *h_bjet_bb_tagged = (TH3D *) fin_bjet->Get("h_bb_"+observable+"_tagged")->Clone("h_bjet_bb_tagged");
    
    // muon jets modification
    // TFile *fin_mujet = new TFile("histos/mujet_Mu5_" + label + "_histograms_for_fit_JP.root");
    // TH3D *h_mujet_b = (TH3D *) fin_mujet->Get("h_b_"+observable)->Clone("h_mujet_b");
    // TH3D *h_mujet_b_tagged = (TH3D *) fin_mujet->Get("h_b_"+observable+"_tagged")->Clone("h_mujet_b_tagged");
    // TH3D *h_mujet_bb = (TH3D *) fin_mujet->Get("h_bb_"+observable)->Clone("h_mujet_bb");
    // TH3D *h_mujet_bb_tagged = (TH3D *) fin_mujet->Get("h_bb_"+observable+"_tagged")->Clone("h_mujet_bb_tagged");
    // TH3D *h_mujet_c = (TH3D *) fin_mujet->Get("h_c_"+observable)->Clone("h_mujet_c");
    // TH3D *h_mujet_c_tagged = (TH3D *) fin_mujet->Get("h_c_"+observable+"_tagged")->Clone("h_mujet_c_tagged");
    // TH3D *h_mujet_cc = (TH3D *) fin_mujet->Get("h_cc_"+observable)->Clone("h_mujet_cc");
    // TH3D *h_mujet_cc_tagged = (TH3D *) fin_mujet->Get("h_cc_"+observable+"_tagged")->Clone("h_mujet_cc_tagged");


    // Create the templates 
    double gsp_frac = 1.0; // 1.5 for gsp up +50%, 0.5 for gsp down -50%
    std::cout << "GSP fraction/nom = " << gsp_frac << std::endl;

    TH3D *h_bbb = (TH3D *) h_dijet_b->Clone("h_bbb");
    h_bbb->Add(h_dijet_bb, gsp_frac); 
    h_bbb->Add(h_bjet_b);
    h_bbb->Add(h_bjet_bb, gsp_frac); 
    // mujet modification
    // h_bbb->Add(h_mujet_b);
    // h_bbb->Add(h_mujet_bb, gsp_frac);
    
    TH3D *h_ccc = (TH3D *) h_dijet_c->Clone("h_ccc");
    h_ccc->Add(h_dijet_cc, gsp_frac);
    // mujet modification
    // h_ccc->Add(h_mujet_c);
    // h_ccc->Add(h_mujet_cc, gsp_frac);

    TH3D *h_l = (TH3D *) h_dijet_l->Clone("h_l");

    TH3D *h_bbb_tagged = (TH3D *) h_dijet_b_tagged->Clone("h_bbb_tagged");
    h_bbb_tagged->Add(h_dijet_bb_tagged, gsp_frac);
    h_bbb_tagged->Add(h_bjet_b_tagged);
    h_bbb_tagged->Add(h_bjet_bb_tagged, gsp_frac);
    // mujet modification
    // h_bbb_tagged->Add(h_mujet_b_tagged);
    // h_bbb_tagged->Add(h_mujet_bb_tagged, gsp_frac);

    TH3D *h_ccc_tagged = (TH3D *) h_dijet_c_tagged->Clone("h_ccc_tagged");
    h_ccc_tagged->Add(h_dijet_cc_tagged, gsp_frac);
    // mujet modification
    // h_ccc_tagged->Add(h_mujet_c_tagged);
    // h_ccc_tagged->Add(h_mujet_cc_tagged, gsp_frac);

    TH3D *h_l_tagged = (TH3D *) h_dijet_l_tagged->Clone("h_l_tagged");

    for (auto h : {
            h_data, h_bbb, h_ccc, h_l, 
            h_dijet_b, h_dijet_bb, h_dijet_c, h_dijet_cc, h_dijet_l // for MC fractions
        }) {
        h->RebinY(6);
        h->RebinZ(3);
    }

    // std::cout << "h_data_tagged->GetNbinsY() = " << h_data_tagged->GetNbinsY() << std::endl;
    for (auto h : {
            h_data_tagged, h_bbb_tagged, h_ccc_tagged, h_l_tagged,
            h_dijet_b_tagged, h_dijet_bb_tagged, h_dijet_c_tagged, h_dijet_cc_tagged, h_dijet_l_tagged
        }) {
        // if (observable!="zg") h->RebinY(14); 
        // else h->RebinY(7);
        // h->RebinY(7);
        h->RebinZ(3);
    }

    //------------ Calculate fractions in MC
    TH3D *h_dijet_bbb = (TH3D *) h_dijet_b->Clone("h_dijet_bbb");
    h_dijet_bbb->Add(h_dijet_bb);

    TH3D *h_dijet_ccc = (TH3D *) h_dijet_c->Clone("h_dijet_ccc");
    h_dijet_ccc->Add(h_dijet_cc);

    TH3D *h_dijet_all = (TH3D *) h_dijet_bbb->Clone("h_dijet_all");
    h_dijet_all->Add(h_dijet_ccc);
    h_dijet_all->Add(h_dijet_l);

    h_dijet_bbb->GetYaxis()->SetRange(1, h_dijet_bbb->GetNbinsY());
    h_dijet_all->GetYaxis()->SetRange(1, h_dijet_all->GetNbinsY());

    TH2D *h_dijet_bbb_2d = (TH2D *) h_dijet_bbb->Project3D("zx"); // h_b includes bjet entries
    TH2D *h_dijet_ccc_2d = (TH2D *) h_dijet_ccc->Project3D("zx");
    TH2D *h_dijet_l_2d = (TH2D *) h_dijet_l->Project3D("zx");
    TH2D *h_dijet_all_2d = (TH2D *) h_dijet_all->Project3D("zx");

    TH2D *h_bbb_f_mc = (TH2D *) h_dijet_bbb_2d->Clone("h_bbb_f_mc");
    h_bbb_f_mc->Reset();
    h_bbb_f_mc->Divide(h_dijet_bbb_2d, h_dijet_all_2d, 1., 1., "b");

    TH2D *h_ccc_f_mc = (TH2D *) h_dijet_ccc_2d->Clone("h_ccc_f_mc");
    h_ccc_f_mc->Reset();
    h_ccc_f_mc->Divide(h_dijet_ccc_2d, h_dijet_all_2d, 1., 1., "b");

    TH2D *h_l_f_mc = (TH2D *) h_dijet_l_2d->Clone("h_l_f_mc");
    h_l_f_mc->Reset();
    h_l_f_mc->Divide(h_dijet_l_2d, h_dijet_all_2d, 1., 1., "b");

    TH3D *h_dijet_bbb_tagged = (TH3D *) h_dijet_b_tagged->Clone("h_dijet_bbb");
    h_dijet_bbb_tagged->Add(h_dijet_bb_tagged);

    TH3D *h_dijet_ccc_tagged = (TH3D *) h_dijet_c_tagged->Clone("h_dijet_ccc");
    h_dijet_ccc_tagged->Add(h_dijet_cc_tagged);

    TH3D *h_dijet_all_tagged = (TH3D *) h_dijet_bbb_tagged->Clone("h_dijet_all_tagged");
    h_dijet_all_tagged->Add(h_dijet_c_tagged);
    h_dijet_all_tagged->Add(h_dijet_l_tagged);

    h_dijet_bbb_tagged->GetYaxis()->SetRange(1, h_dijet_bbb_tagged->GetNbinsY());
    h_dijet_all_tagged->GetYaxis()->SetRange(1, h_dijet_all_tagged->GetNbinsY());

    TH2D *h_dijet_bbb_tagged_2d = (TH2D *) h_dijet_bbb_tagged->Project3D("zx"); // h_b includes bjet entries
    TH2D *h_dijet_ccc_tagged_2d = (TH2D *) h_dijet_ccc_tagged->Project3D("zx");
    TH2D *h_dijet_l_tagged_2d = (TH2D *) h_dijet_l_tagged->Project3D("zx");
    TH2D *h_dijet_all_tagged_2d = (TH2D *) h_dijet_all_tagged->Project3D("zx");

    TH2D *h_bbb_f_mc_tagged = (TH2D *) h_dijet_bbb_tagged_2d->Clone("h_bbb_f_mc_tagged");
    h_bbb_f_mc_tagged->Reset();
    h_bbb_f_mc_tagged->Divide(h_dijet_bbb_tagged_2d, h_dijet_all_tagged_2d, 1., 1., "b");

    TH2D *h_ccc_f_mc_tagged = (TH2D *) h_dijet_ccc_tagged_2d->Clone("h_ccc_f_mc_tagged");
    h_ccc_f_mc_tagged->Reset();
    h_ccc_f_mc_tagged->Divide(h_dijet_ccc_tagged_2d, h_dijet_all_tagged_2d, 1., 1., "b");

    TH2D *h_l_f_mc_tagged = (TH2D *) h_dijet_l_tagged_2d->Clone("h_l_f_mc_tagged");
    h_l_f_mc_tagged->Reset();
    h_l_f_mc_tagged->Divide(h_dijet_l_tagged_2d, h_dijet_all_tagged_2d, 1., 1., "b");
    //------------

    // Create histograms to fill with fractions
    TH2D *h_bbb_f = (TH2D *) h_bbb->Project3D("zx")->Clone("h_bbb_f");
    h_bbb_f->Reset();
    h_bbb_f->SetTitle("b+bb fraction");

    TH2D *h_ccc_f = (TH2D *) h_bbb_f->Clone("h_ccc_f");
    h_ccc_f->SetTitle("c fraction");

    TH2D *h_l_f = (TH2D *) h_bbb_f->Clone("h_l_f");
    h_l_f->SetTitle("l fraction");

    TH2D *h_bbb_f_tagged = (TH2D *) h_bbb_f->Clone("h_bbb_f_tagged");
    TH2D *h_ccc_f_tagged = (TH2D *) h_ccc_f->Clone("h_ccc_f_tagged");
    TH2D *h_l_f_tagged = (TH2D *) h_l_f->Clone("h_l_f_tagged");

    // Loop over jet pt+observable bins
    int nbins_x = h_bbb->GetNbinsX();
    int nbins_jp = h_bbb->GetNbinsY();
    int nbins_pt = h_bbb->GetNbinsZ();

    int nbins_jp_tagged = h_bbb_tagged->GetNbinsY();

    std::vector<std::pair<int, int>> non_converge_bins;
    std::vector<double> unc;

    std::vector<std::pair<int, int>> non_converge_bins_tagged;
    std::vector<double> unc_tagged;

    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        for (int ibin_x = 1; ibin_x <= nbins_x; ibin_x++) {
            // debug 
            // if (ibin_pt != 1) continue;
            // if (ibin_x != 1) continue;

            std::cout << "----------------------------------------------------------------------------" << std::endl;
            std::cout << "\n\nBeginning fit for ibint_pt=" << ibin_pt << ", ibin_x=" << ibin_x <<  "\n\n" << std::endl;
            std::cout << "----------------------------------------------------------------------------" << std::endl;
            
            // -------------------- //
            //    FIT INCLUSIVE     //
            // -------------------- //
            std::cout << "Fitting inclusive histograms..." << std::endl;

            // Make projections 
            TH1D *h_data_1d = (TH1D *) h_data->ProjectionY(Form("h_data_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bbb_1d = (TH1D *) h_bbb->ProjectionY(Form("h_bbb_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_ccc_1d = (TH1D *) h_ccc->ProjectionY(Form("h_ccc_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_l_1d = (TH1D *) h_l->ProjectionY(Form("h_l_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            // Get MC fractions as initial values 
            TH1D *h_dijet_b_1d = (TH1D *) h_dijet_b->ProjectionY(Form("h_dijet_b_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_bb_1d = (TH1D *) h_dijet_bb->ProjectionY(Form("h_dijet_bb_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_c_1d = (TH1D *) h_dijet_c->ProjectionY(Form("h_dijet_c_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_cc_1d = (TH1D *) h_dijet_cc->ProjectionY(Form("h_dijet_cc_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_l_1d = (TH1D *) h_dijet_l->ProjectionY(Form("h_dijet_l_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            double int0 = h_dijet_b_1d->Integral()+h_dijet_bb_1d->Integral();
            double int1 = h_dijet_c_1d->Integral()+h_dijet_cc_1d->Integral();
            double int2 = h_dijet_l_1d->Integral();
            double total = int0+int1+int2;

            std::cout << int0 << ", " << int1 << ", " << int2 << std::endl; 

            // Create the observable
            Double_t min_jp = h_data_1d->GetXaxis()->GetBinLowEdge(1);
            // Double_t min_jp = 0.;
            Double_t max_jp = h_data_1d->GetXaxis()->GetBinUpEdge(nbins_jp);
            RooRealVar jp(Form("jp_%d_%d", ibin_pt, ibin_x), "jp", min_jp, max_jp);
            jp.setBins(nbins_jp);

            // Create the RooDataHist object for the observed data + templates
            RooDataHist *dh_data_1d = new RooDataHist(Form("dh_data_1d_%d_%d", ibin_pt, ibin_x), "dh_data_1d", jp, RooFit::Import(*h_data_1d));
            RooDataHist *dh_bbb_1d = new RooDataHist(Form("dh_bbb_1d_%d_%d", ibin_pt, ibin_x), "dh_bbb_1d", jp, RooFit::Import(*h_bbb_1d));
            RooDataHist *dh_ccc_1d = new RooDataHist(Form("dh_ccc_1d_%d_%d", ibin_pt, ibin_x), "dh_ccc_1d", jp, RooFit::Import(*h_ccc_1d));
            RooDataHist *dh_l_1d = new RooDataHist(Form("dh_l_1d_%d_%d", ibin_pt, ibin_x), "dh_l_1d", jp, RooFit::Import(*h_l_1d));

            // Create the RooHistPdf objects for the template PDFs
            RooHistPdf bbb_template(Form("bbb_template_%d_%d", ibin_pt, ibin_x), "bbb_template", jp, *dh_bbb_1d);
            RooHistPdf ccc_template(Form("ccc_template_%d_%d", ibin_pt, ibin_x), "ccc_template", jp, *dh_ccc_1d);
            RooHistPdf l_template(Form("l_template_%d_%d", ibin_pt, ibin_x), "l_template", jp, *dh_l_1d);
            RooArgList template_list(bbb_template, ccc_template, l_template, Form("template_list_%d_%d", ibin_pt, ibin_x));

            // Create the RooRealVar for the fit parameter (e.g., fraction of template A)
            std::cout << "int0/total=" << int0/total << std::endl; //int1/(total-int0)
            RooRealVar bbb_fraction_val(Form("bbb_fraction_val_%d_%d", ibin_pt, ibin_x),"bbb_fraction_val",int0/total,0.,1.);
            RooRealVar ccc_fraction_val(Form("ccc_fraction_val_%d_%d", ibin_pt, ibin_x),"ccc_fraction_val",int1/(total-int0),0.,1.); 
            RooArgList coeff_list(bbb_fraction_val, ccc_fraction_val, Form("coeff_list_%d_%d", ibin_pt, ibin_x));
            // l=1-bbb-ccc

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

            double b = ccc_fraction_val.getValV();
            double db = ccc_fraction_val.getError();

            if (status != 0) {
                std::cout << "\n\n\n\n!!!Fitting for ipt = " << ibin_pt 
                        << ", ix = " << ibin_x 
                        << " did not converge\n\n\n\n" << std::endl;
                non_converge_bins.push_back(std::pair<int, int>(ibin_pt, ibin_x));
                // continue;
            }

            Double_t p0, p1, p2, errP0, errP1, errP2;
            p0 = a;
            p1 = (1-a)*b;
            p2 = (1-a)*(1-b);

            errP0 = da;
            errP1 = std::sqrt((b*b*da*da) + ((1-a)*(1-a)*db*db));
            errP2 = std::sqrt(((1-b)*(1-b)*da*da)+((1-a)*(1-a)*db*db)); // assuming no correlation

            h_bbb_f->SetBinContent(ibin_x, ibin_pt, p0);
            h_bbb_f->SetBinError(ibin_x, ibin_pt, errP0);

            h_ccc_f->SetBinContent(ibin_x, ibin_pt, p1);
            h_ccc_f->SetBinError(ibin_x, ibin_pt, errP1);

            h_l_f->SetBinContent(ibin_x, ibin_pt, p2);
            h_l_f->SetBinError(ibin_x, ibin_pt, errP2);

            // if (ibin_x<nbins_x) 
                unc.push_back(errP0/p0);
        
            // -------------------- //
            //      FIT TAGGED      //
            // -------------------- //


            // Make projections 
            TH1D *h_data_tagged_1d = (TH1D *) h_data_tagged->ProjectionY(Form("h_data_tagged_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_bbb_tagged_1d = (TH1D *) h_bbb_tagged->ProjectionY(Form("h_bbb_tagged_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_ccc_tagged_1d = (TH1D *) h_ccc_tagged->ProjectionY(Form("h_ccc_tagged_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_l_tagged_1d = (TH1D *) h_l_tagged->ProjectionY(Form("h_l_tagged_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            // Get MC fractions as initial values 
            TH1D *h_dijet_b_tagged_1d = (TH1D *) h_dijet_b_tagged->ProjectionY(Form("h_dijet_b_tagged_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_bb_tagged_1d = (TH1D *) h_dijet_bb_tagged->ProjectionY(Form("h_dijet_bb_tagged_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_c_tagged_1d = (TH1D *) h_dijet_c_tagged->ProjectionY(Form("h_dijet_c_tagged_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_cc_tagged_1d = (TH1D *) h_dijet_cc_tagged->ProjectionY(Form("h_dijet_cc_tagged_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);
            TH1D *h_dijet_l_tagged_1d = (TH1D *) h_dijet_l_tagged->ProjectionY(Form("h_dijet_l_tagged_1d_%d_%d", ibin_pt, ibin_x), ibin_x, ibin_x, ibin_pt, ibin_pt);

            for (auto h : {
                    h_data_tagged_1d, h_bbb_tagged_1d, h_ccc_tagged_1d, h_l_tagged_1d,
                    h_dijet_b_tagged_1d, h_dijet_bb_tagged_1d, h_dijet_c_tagged_1d, h_dijet_cc_tagged_1d, h_dijet_l_tagged_1d
                }) {
                if (observable=="rg"&&ibin_x==nbins_x) h->Rebin(2);    
                else if (observable=="zpt"&&ibin_x>3) h->Rebin(5);   
                else h->Rebin(7);   
            }
            
            // Double_t int0, int1, int2, int3, total;
            int0 = h_dijet_b_tagged_1d->Integral()+h_dijet_bb_tagged_1d->Integral();
            int1 = h_dijet_c_tagged_1d->Integral()+h_dijet_cc_tagged_1d->Integral();
            int2 = h_dijet_l_tagged_1d->Integral();
            total = int0+int1+int2;

            // Create the observable
            Double_t min_jp_tagged = h_data_tagged_1d->GetXaxis()->GetBinLowEdge(1);
            Double_t max_jp_tagged = h_data_tagged_1d->GetXaxis()->GetBinUpEdge(nbins_jp_tagged);
            RooRealVar jp_tagged(Form("jp_%d_%d", ibin_pt, ibin_x), "jp", min_jp_tagged, max_jp_tagged);
            jp_tagged.setBins(nbins_jp_tagged);

            // Create the RooDataHist object for the observed data + templates
            RooDataHist *dh_data_tagged_1d = new RooDataHist(Form("dh_data_tagged_1d_%d_%d", ibin_pt, ibin_x), "dh_data_tagged_1d", jp_tagged, RooFit::Import(*h_data_tagged_1d));
            RooDataHist *dh_bbb_tagged_1d = new RooDataHist(Form("dh_bbb_tagged_1d_%d_%d", ibin_pt, ibin_x), "dh_bbb_tagged_1d", jp_tagged, RooFit::Import(*h_bbb_tagged_1d));
            RooDataHist *dh_ccc_tagged_1d = new RooDataHist(Form("dh_ccc_tagged_1d_%d_%d", ibin_pt, ibin_x), "dh_ccc_tagged_1d", jp_tagged, RooFit::Import(*h_ccc_tagged_1d));
            RooDataHist *dh_l_tagged_1d = new RooDataHist(Form("dh_l_tagged_1d_%d_%d", ibin_pt, ibin_x), "dh_l_tagged_1d", jp_tagged, RooFit::Import(*h_l_tagged_1d));

            // std::cout << h_bbb_tagged_1d->GetNbinsX() << std::endl;

            // Create the RooHistPdf objects for the template PDFs
            RooHistPdf bbb_tagged_template(Form("bbb_tagged_template_%d_%d", ibin_pt, ibin_x), "bbb_tagged_template", jp_tagged, *dh_bbb_tagged_1d);
            RooHistPdf ccc_tagged_template(Form("ccc_tagged_template_%d_%d", ibin_pt, ibin_x), "ccc_tagged_template", jp_tagged, *dh_ccc_tagged_1d);
            RooHistPdf l_tagged_template(Form("l_tagged_template_%d_%d", ibin_pt, ibin_x), "l_tagged_template", jp_tagged, *dh_l_tagged_1d);
            RooArgList template_list_tagged(bbb_tagged_template, ccc_tagged_template, l_tagged_template, Form("template_list_tagged_%d_%d", ibin_pt, ibin_x));

            // Create the RooRealVar for the fit parameter (e.g., fraction of template A)
            double init_val = 0.9; // ok for calojet60
            // if (observable=="rg") init_val = 1.;
            // else if (observable=="zg") init_val = (int0+int1)/total;
            // else if (observable=="zpt") init_val = (int0+int1)/total;
            // else if (observable=="zpt") init_val = (int0+int1)/total;

            RooRealVar bbb_tagged_fraction_val(Form("bbb_tagged_fraction_val_%d_%d", ibin_pt, ibin_x),"bbb_tagged_fraction_val",int0/total,0.,1.);
            RooRealVar ccc_tagged_fraction_val(Form("ccc_tagged_fraction_val_%d_%d", ibin_pt, ibin_x),"ccc_tagged_fraction_val",int1/(total-int0),0.,1.); // c=(1-bbb)*c'
            RooArgList coeff_list_tagged(bbb_tagged_fraction_val, ccc_tagged_fraction_val, Form("coeff_list_tagged_%d_%d", ibin_pt, ibin_x));
            // l=1-bbb-c

            // Create the composite PDF using a linear combination of the template PDFs
            RooAddPdf model0_tagged(Form("model0_tagged_%d_%d", ibin_pt, ibin_x), "model0_tagged", template_list_tagged, coeff_list_tagged, true);
            // RooFitResult* result_tagged = model0_tagged.chi2FitTo(*dh_data_tagged_1d, RooFit::SumW2Error(true), RooFit::Save(), RooFit::CloneData(true), RooFit::PrintLevel(-1), RooFit::Strategy(1), RooFit::Minos(false)); // result_tagged is already given a unique name
            RooFitResult* result_tagged = model0_tagged.fitTo(*dh_data_tagged_1d, RooFit::SumW2Error(true), RooFit::Save(), RooFit::CloneData(true), RooFit::PrintLevel(-1), RooFit::Strategy(1), RooFit::Minos(false)); // result_tagged is already given a unique name
            auto status_tagged = result_tagged->status();
            result_tagged->Print();

            std::cout << "covariance matrix:" << std::endl;
            (result_tagged->covarianceMatrix().Print());


            // Get the fitted parameter values
            // Double_t a, da, b, db;
            a = bbb_tagged_fraction_val.getValV();
            da = bbb_tagged_fraction_val.getError();

            // std::cout << "a=" <<  a << std::endl;

            if (observable=="rg" && ibin_pt==1 && ibin_x==nbins_x) a = 1.;

            b = ccc_tagged_fraction_val.getValV();
            db = ccc_tagged_fraction_val.getError();

            if (status_tagged != 0) {
                std::cout << "\n\n\n\n!!!Fitting for ipt = " << ibin_pt 
                        << ", ix = " << ibin_x 
                        << " did not converge\n\n\n\n" << std::endl;
                non_converge_bins_tagged.push_back(std::pair<int, int>(ibin_pt, ibin_x));
                // continue;
            }

            // Double_t p0, p1, p2, errP0, errP1, errP2;
            p0 = a;
            p1 = (1-a)*b;
            p2 = (1-a)*(1-b);

            errP0 = da;
            errP1 = std::sqrt((b*b*da*da) + ((1-a)*(1-a)*db*db));
            errP2 = std::sqrt(((1-b)*(1-b)*da*da)+((1-a)*(1-a)*db*db)); // assuming no correlation

            h_bbb_f_tagged->SetBinContent(ibin_x, ibin_pt, p0);
            h_bbb_f_tagged->SetBinError(ibin_x, ibin_pt, errP0);

            h_ccc_f_tagged->SetBinContent(ibin_x, ibin_pt, p1);
            h_ccc_f_tagged->SetBinError(ibin_x, ibin_pt, errP1);

            h_l_f_tagged->SetBinContent(ibin_x, ibin_pt, p2);
            h_l_f_tagged->SetBinError(ibin_x, ibin_pt, errP2);

            // if (ibin_x<nbins_x) 
                unc_tagged.push_back(errP0/p0);            
        } // observable bin loop
    } // pt bin loop

    std::cout << "Max rel unc for inclusive=" << *max_element(unc.begin(), unc.end()) << std::endl;
    std::cout << "Max rel unc for tagged=" << *max_element(unc_tagged.begin(), unc_tagged.end()) << std::endl;

    std::cout << "Fit did not converged for inclusive bins:" << std::endl;
    for (auto p : non_converge_bins) {
        std::cout << "- (" << p.first << ", " << p.second << ")" << std::endl;
    }
    std::cout << "Fit did not converged for tagged bins:" << std::endl;
    for (auto p : non_converge_bins_tagged) {
        std::cout << "- (" << p.first << ", " << p.second << ")" << std::endl;
    }

    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_bbb_f, h_ccc_f, h_l_f}) {
        h->Write();
    }
    for (auto h : {h_bbb, h_ccc, h_l, h_data}) {
        h->Write();
    }

    for (auto h : {h_bbb_f_tagged, h_ccc_f_tagged, h_l_f_tagged}) {
        h->Write();
    }
    for (auto h : {h_bbb_tagged, h_ccc_tagged, h_l_tagged, h_data_tagged}) {
        h->RebinY(7);
        h->Write();
    }

    h_bbb_f_mc->Write();
    h_ccc_f_mc->Write();
    h_l_f_mc->Write();

    h_bbb_f_mc_tagged->Write();
    h_ccc_f_mc_tagged->Write();
    h_l_f_mc_tagged->Write();

    std::cout << h_dijet_bbb_tagged->GetNbinsY() << std::endl;

    for (int i = 1; i <= nbins_pt; i++) {
        for (int j = 1; j <= nbins_x; j++) {
            std::cout << "(" << i << ", " << j << ") = " << h_bbb_f->GetBinContent(j,i) << ", " << h_bbb_f_tagged->GetBinContent(j,i) << std::endl;
        }
    }

    // fout->Close();
}