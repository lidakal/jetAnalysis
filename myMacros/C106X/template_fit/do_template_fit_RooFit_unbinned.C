#include "Purity.h"

void do_template_fit_RooFit_unbinned()
{
    // Setup 
    TString label_out = "pseudo_data";
    TString fout_name = "histos/fitted_parameters_RooFit_" + label_out + "_unbinned.root";

    // Load data
    // TString fin_data_name = "./histos/data_aggrTMVA_v2_tree_template_fit.root";
    // TFile *fin_data = new TFile(fin_data_name);
    // TTree *tree_data = (TTree *) fin_data->Get("tree_out")->Clone("tree_data");

    // Load training MC
    TString fin_dijet_histograms_name = "./histos/dijet_aggrTMVA_withHLT_v2_histograms.root";
    TFile *fin_dijet_histograms = new TFile(fin_dijet_histograms_name);
    TH3D *h_sig_training_dijet = (TH3D *) fin_dijet_histograms->Get("h_sig_training")->Clone("h_sig_training_dijet");
    TH3D *h_bkg_bb_training_dijet = (TH3D *) fin_dijet_histograms->Get("h_bkg_bb_training")->Clone("h_bkg_bb_training_dijet");
    TH3D *h_bkg_rest_training_dijet = (TH3D *) fin_dijet_histograms->Get("h_bkg_rest_training")->Clone("h_bkg_rest_training_dijet");

    for (auto h : {h_sig_training_dijet,
                   h_bkg_bb_training_dijet,
                   h_bkg_rest_training_dijet}) {
                    h->Sumw2();
                   }

    Int_t nbins_rg = h_sig_training_dijet->GetNbinsX();
    Int_t nbins_mb = h_sig_training_dijet->GetNbinsY();
    Int_t nbins_pt = h_sig_training_dijet->GetNbinsZ();

    Double_t mb_min = h_sig_training_dijet->GetYaxis()->GetBinLowEdge(1);
    Double_t mb_max = h_sig_training_dijet->GetYaxis()->GetBinUpEdge(nbins_mb);

    std::cout << "detected mb min = " << mb_min << ", mb max = " << mb_max << std::endl;

    // Load testing MC 
    TString fin_dijet_name = "/data_CMS/cms/kalipoliti/compact_trees/dijet_aggrTMVA_withHLT_v2_tree_template_fit.root";
    TFile *fin_dijet = new TFile(fin_dijet_name);
    TTree *tree_dijet_all = (TTree *) fin_dijet->Get("tree_all")->Clone("tree_dijet_all");
    Long64_t dijet_all_entries = tree_dijet_all->GetEntries();
    

    // TTree *tree_dijet_sig = (TTree *) fin_dijet->Get("tree_sig")->Clone("tree_dijet_sig");
    // TTree *tree_dijet_bkg_bb = (TTree *) fin_dijet->Get("tree_bkg_bb")->Clone("tree_dijet_bkg_bb");
    // TTree *tree_dijet_bkg_rest = (TTree *) fin_dijet->Get("tree_bkg_rest")->Clone("tree_dijet_bkg_rest");

    // TString fin_bjet_name = "./histos/bjet_aggrTMVA_withHLT_tree_template_fit.root";
    // TFile *fin_bjet = new TFile(fin_bjet_name);

    // TTree *tree_bjet_sig = (TTree *) fin_bjet->Get("tree_sig")->Clone("tree_bjet_sig");
    // TTree *tree_bjet_bkg_bb = (TTree *) fin_bjet->Get("tree_bkg_bb")->Clone("tree_bjet_bkg_bb");
    // TTree *tree_bjet_bkg_rest = (TTree *) fin_bjet->Get("tree_bkg_rest")->Clone("tree_bjet_bkg_rest");

    // The trees created from copyTree need to be written in a new file
    std::cout << "Creating file " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (int ibin_pt = 1; ibin_pt <= nbins_pt; ibin_pt++) {
        double pt_min = h_sig_training_dijet->GetZaxis()->GetBinLowEdge(ibin_pt);
        double pt_max = h_sig_training_dijet->GetZaxis()->GetBinUpEdge(ibin_pt);

        for (int ibin_rg = 0; ibin_rg <= nbins_rg; ibin_rg++) {
            if (ibin_pt != 2) continue;
            if (ibin_rg != 4) continue;

            double logrg_min = h_sig_training_dijet->GetXaxis()->GetBinLowEdge(ibin_rg);
            double logrg_max = h_sig_training_dijet->GetXaxis()->GetBinUpEdge(ibin_rg);
            
            // Make training MC projections
            TH1D *h_sig_training_dijet_mb = (TH1D *) h_sig_training_dijet->ProjectionY(Form("h_sig_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_bb_training_dijet_mb = (TH1D *) h_bkg_bb_training_dijet->ProjectionY(Form("h_bkg_bb_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);
            TH1D *h_bkg_rest_training_dijet_mb = (TH1D *) h_bkg_rest_training_dijet->ProjectionY(Form("h_bkg_rest_training_dijet_mb_%d_%d", ibin_pt, ibin_rg), ibin_rg, ibin_rg, ibin_pt, ibin_pt);

            // Select testing MC in specific range
            TString cuts = Form("Entry$<%lld&&jtpt>=%.2f&&jtpt<%.2f&&logrg>=%.2f&&logrg<%.2f", dijet_all_entries/2, pt_min, pt_max, logrg_min, logrg_max);
            TTree *tree_dijet_test = (TTree *) tree_dijet_all->CopyTree(cuts);
            tree_dijet_test->SetName(Form("tree_dijet_test_%d_%d", ibin_pt, ibin_rg));

            // Merge testing MC
            TTree *tree_test = (TTree *) tree_dijet_test->Clone(Form("tree_test_%d_%d", ibin_pt, ibin_rg));

            // Create RooFit objects
            RooRealVar mb_var("mb", "mb", mb_min, mb_max);
            RooRealVar weight_var("weight", "weight", 0., 1.);
            RooDataSet data_ds("data", "data", tree_test, RooArgSet(mb_var, weight_var), "", "weight");
            // RooDataSet data_ds("data", "data", tree_test, RooArgSet(mb_var)); // debug no weight
            // std::cout << data_ds.isWeighted() << std::endl;
            RooDataHist *dh_sig_training_mb = new RooDataHist("dh_sig_training_mb", "dh_sig_training_mb", mb_var, RooFit::Import(*h_sig_training_dijet_mb));
            RooDataHist *dh_bkg_bb_training_mb = new RooDataHist("dh_bkg_bb_training_mb", "dh_bkg_bb_training_mb", mb_var, RooFit::Import(*h_bkg_bb_training_dijet_mb));
            RooDataHist *dh_bkg_rest_training_mb = new RooDataHist("dh_bkg_rest_training_mb", "dh_bkg_rest_training_mb", mb_var, RooFit::Import(*h_bkg_rest_training_dijet_mb));

            // ***** Case 0 - 'Rigid templates' *****

            // Construct histogram shapes for signal and background
            RooHistFunc p_h_sig("p_h_sig", "p_h_sig", mb_var, *dh_sig_training_mb);
            RooHistFunc p_h_bkg_bb("p_h_bkg_bb", "p_h_bkg_bb", mb_var, *dh_bkg_bb_training_mb);
            RooHistFunc p_h_bkg_rest("p_h_bkg_rest", "p_h_bkg_rest", mb_var, *dh_bkg_rest_training_mb);
            
            // Construct scale factors for adding the two distributions
            // RooRealVar Asig0("Asig0", "Asig0", 1., 0.01, 10000.);
            // RooRealVar Abkg0("Abkg0", "Abkg0", 1., 0.01, 10000.);
            // RooRealVar Abkg1("Abkg1", "Abkg1", 1., 0.01, 10000.);

            RooRealVar Asig0("Asig0", "Asig0", .33, 0., 1.);
            RooRealVar Abkg0("Abkg0", "Abkg0", .33, 0., 1.);
            
            // Construct the sum model
            RooRealSumPdf model0("model0", "model0",
                RooArgList(p_h_sig, p_h_bkg_bb, p_h_bkg_rest),
                // RooArgList(Asig0, Abkg0, Abkg1),
                RooArgList(Asig0, Abkg0),
                false);


            // ***** Case 1 - 'Barlow Beeston' *****
 
            // Construct parameterized histogram shapes for signal and background
            RooParamHistFunc p_ph_sig1("p_ph_sig1","p_ph_sig1",*dh_sig_training_mb);
            RooParamHistFunc p_ph_bkg1("p_ph_bkg1","p_ph_bkg1",*dh_bkg_bb_training_mb);
            RooParamHistFunc p_ph_bkg2("p_ph_bkg2","p_ph_bkg2",*dh_bkg_rest_training_mb);
            
            RooRealVar Asig1("Asig1","Asig1",.33, 0., 1.);
            RooRealVar Abkg1("Abkg1","Abkg1",.33, 0., 1.);
            
            // Construct the sum of these
            RooRealSumPdf model_tmp("sp_ph", "sp_ph",
                RooArgList(p_ph_sig1,p_ph_bkg1,p_ph_bkg2),
                RooArgList(Asig1,Abkg1),
                false);
            
            // Construct the subsidiary poisson measurements constraining the histogram parameters
            // These ensure that the bin contents of the histograms are only allowed to vary within
            // the statistical uncertainty of the Monte Carlo.
            RooHistConstraint hc_sig1("hc_sig1","hc_sig1",p_ph_sig1);
            RooHistConstraint hc_bkg1("hc_bkg1","hc_bkg1",p_ph_bkg1);
            RooHistConstraint hc_bkg2("hc_bkg2","hc_bkg2",p_ph_bkg2);
            
            // Construct the joint model with template PDFs and constraints
            RooProdPdf model1("model1","model1",RooArgSet(hc_sig1,hc_bkg1,hc_bkg2),RooFit::Conditional(model_tmp,mb_var));


            auto result0 = model0.fitTo(data_ds, RooFit::SumW2Error(kTRUE), RooFit::PrintLevel(0), RooFit::Save());
            // auto result1 = model1.fitTo(data_ds, RooFit::SumW2Error(kTRUE), RooFit::PrintLevel(0), RooFit::Save());

            // Plot
            // Construct plot frame
            RooPlot* frame = mb_var.frame(RooFit::Title("Unbinned ML fit, binned chi^2 fit to weighted data")) ;

            // Plot data using sum-of-weights-squared error rather than Poisson errors
            data_ds.plotOn(frame,RooFit::DataError(RooAbsData::SumW2)) ;

            model0.plotOn(frame, RooFit::LineColor(kBlue));
            model0.plotOn(frame, RooFit::Components(p_h_sig), RooFit::LineColor(kTeal));
            model0.plotOn(frame, RooFit::Components(p_h_bkg_bb), RooFit::LineColor(kRed));

            
            // model0.paramOn(frame);
            
            // Overlay result of 2nd order polynomial fit to weighted data
            // p2.plotOn(frame) ;

            new TCanvas("rf403_weightedevts","rf403_weightedevts",600,600) ;
            gPad->SetLeftMargin(0.15) ; frame->GetYaxis()->SetTitleOffset(1.8) ; frame->Draw() ;

            // draw also the real signal contribution 
            TH1D *hsig = (TH1D *) h_sig_training_dijet_mb->Clone("hsig");
            Double_t ndata = data_ds.sumEntries("mb>0.&&mb<7.");
            Double_t sigInt = hsig->Integral();
            Double_t bw = 0.7;
            std::cout << "ndata = " << ndata << std::endl;
            hsig->Scale(ndata * Asig0.getValV() / (sigInt * bw));
            hsig->SetFillStyle(3004);
            hsig->SetFillColor(kTeal);
            hsig->Draw("hist same");


            // std::cout << Asig0.getValV() << std::endl;
            double p0 = Asig0.getValV();
            double p1 = Abkg0.getValV();
            // double p2 = Abkg1.getValV();

            


        }
    }

    
    // for (auto h : {hdata, // raw data
    //                hsig, hbkg_bb, hbkg_rest, // training MC (when running on data, includes pseudo)
    //                hdata_sig_dijet, hdata_bkg_bb_dijet, hdata_bkg_rest_dijet // pseudo data
    //                }) {
    //     h->Write();
    // }
    // for (auto h : {h_sig_fraction, h_sig_fraction_error,
    //                h_bkg_bb_fraction, h_bkg_bb_fraction_error,
    //                h_bkg_rest_fraction, h_bkg_rest_fraction_error,
    //                h_sig_fraction_true,
    //                h_bkg_bb_fraction_true, 
    //                h_bkg_rest_fraction_true,
    //                h_sig_fraction_true_test
    //                }) {
    //     h->Write();
    // }
        
    // fout->Close();
}