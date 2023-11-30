void calculate_correction(TString observable="rg")
{  
    // Load histograms
    TFile *fin_inclusive = new TFile("histos/bjet_aggrTMVA_inclusive_histograms.root");
    TH2D *h_inclusive = (TH2D *) fin_inclusive->Get("h_"+observable+"pt")->Clone("h_inclusive");

    TFile *fin_btag = new TFile("histos/bjet_aggrTMVA_XXT_histograms.root");
    TH2D *h_btag = (TH2D *) fin_btag->Get("h_"+observable+"pt")->Clone("h_btag");

    // Create scale histogram : btag data * (inclusive/btag) = inclusive data
    TH2D *h_correction = (TH2D *) h_inclusive->Clone("h_correction");
    h_correction->Divide(h_correction, h_btag, 1., 1., "b");

    TString fout_name = "./histos/aggrTMVA_XXT_" + observable + "pt_correction.root";
    std::cout << "fout : " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    h_correction->Write();

    fout->Close();
    delete fout;
}