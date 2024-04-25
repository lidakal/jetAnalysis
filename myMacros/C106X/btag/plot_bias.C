void plot_bias(TString jer_opt="nom", TString jec_opt="nom")
{
    // inclusive = non-tagged, but still true b jets
    // in both cases, use only bjet enhanced samples (pythia or herwig)

    bool applySF = true;
    bool sfDown = false;
    bool sfUp = false;

    TString suffix = "_jer_" + jer_opt + "_jec_" + jec_opt;

    TString sample = "pythia_PF40";
    TString label = "aggrTMVA_XXT";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";

    std::cout << "fin: " << fin_name << std::endl;
    std::cout << "applySF: " << applySF << std::endl;
    std::cout << "sfUp: " << sfUp << std::endl;
    std::cout << "sfDown: " << sfDown << std::endl;

    if (label.Contains("XXT") && applySF) {
        suffix += "_withSF";
        if (sfUp) suffix += "Up";
        if (sfDown) suffix += "Down";
    }
    TString fout_name = "histos/" + sample + "_" + label + "_histograms"+suffix+".root";

    TFile *fin = new TFile(fin_name);

    Double_t jtpt;
    Double_t jtptCh;
    Double_t logrg;
    Double_t zg;
    Double_t mb;
    Double_t bpt;

    Double_t jtpt_gen;
    Double_t jtptCh_gen;
    Double_t logrg_gen;
    Double_t zg_gen;
    Double_t mb_gen;
    Double_t bpt_gen;

    Double_t weight;
    Double_t jer_sf_nom;
    Double_t jer_sf_up;
    Double_t jer_sf_down;
    Double_t jec_unc;

    TTree *tree_sig = (TTree *) fin->Get("tree_sig");
    tree_sig->SetBranchAddress("jtpt", &jtpt);
    tree_sig->SetBranchAddress("jtptCh", &jtptCh);
    tree_sig->SetBranchAddress("logrg", &logrg);
    tree_sig->SetBranchAddress("zg", &zg);
    tree_sig->SetBranchAddress("mb", &mb);
    tree_sig->SetBranchAddress("bpt", &bpt);

    tree_sig->SetBranchAddress("jtpt_gen", &jtpt_gen);
    tree_sig->SetBranchAddress("jtptCh_gen", &jtptCh_gen);
    tree_sig->SetBranchAddress("logrg_gen", &logrg_gen);
    tree_sig->SetBranchAddress("zg_gen", &zg_gen);
    tree_sig->SetBranchAddress("mb_gen", &mb_gen);
    tree_sig->SetBranchAddress("bpt_gen", &bpt_gen);

    tree_sig->SetBranchAddress("weight", &weight);
    tree_sig->SetBranchAddress("jer_sf_nom", &jer_sf_nom);
    tree_sig->SetBranchAddress("jer_sf_up", &jer_sf_up);
    tree_sig->SetBranchAddress("jer_sf_down", &jer_sf_down);
    tree_sig->SetBranchAddress("jec_unc", &jec_unc);

    // jet pt bins : no underflow or overflow
    const Int_t z1binsVectorSize = 4;
    Int_t z1bins = z1binsVectorSize - 1;
    Double_t z1binsVector[z1binsVectorSize] = {80., 
                                               100., 
                                               120.,
                                               140.};
    Double_t z1min = z1binsVector[0];
    Double_t z1max = z1binsVector[z1bins];
    int nbins_pt = z1bins;

    // ln(0.4/rg) bins : 1st bin untagged (unphysical)
    const Int_t x1binsVectorSize = 9;
    Int_t x1bins = x1binsVectorSize - 1;
    Double_t x1binsVector[x1binsVectorSize] = {-1.2, 
                                               0., 
                                               0.3,
                                               0.6,
                                               0.9,
                                               1.2,
                                               1.6,
                                               2.1,
                                               2.5};
    Double_t x1min = x1binsVector[0];
    Double_t x1max = x1binsVector[x1bins];
    int nbins_rg = x1bins;

    // zg bins : 1st bin untagged (unphysical)
    const Int_t x3binsVectorSize = 8;
    Int_t x3bins = x3binsVectorSize - 1;
    Double_t x3binsVector[x3binsVectorSize] = {-0.1, 
                                               0.1, 
                                               0.15,
                                               0.2,
                                               0.25,
                                               0.3,
                                               0.4,
                                               0.5};
    Double_t x3min = x3binsVector[0];
    Double_t x3max = x3binsVector[x3bins];
    int nbins_zg = x3bins;


    // mBch bins : 
    const Int_t y1binsVectorSize = 21;
    Int_t y1bins = y1binsVectorSize - 1;
    Double_t y1binsVector[y1binsVectorSize];
    y1binsVector[0] = 0.;
    double y1step = 7./y1bins;
    for (size_t i=1; i<y1binsVectorSize; i++) {
        y1binsVector[i] = y1binsVector[i-1] + y1step;
    }
    Double_t y1min = y1binsVector[0];
    Double_t y1max = y1binsVector[y1bins];

    // bpt/jtptCh bins :
    const Int_t x2binsVectorSize = 7;
    Int_t x2bins = x2binsVectorSize - 1;
    Double_t x2binsVector[x2binsVectorSize] = {0., 
                                               0.35,
                                               0.55,
                                               0.7,
                                               0.8,
                                               0.9,
                                               1.};
    Double_t x2min = x2binsVector[0];
    Double_t x2max = x2binsVector[x2bins];
    int nbins_zpt = x2bins;

    TH2D *h_rgpt_gen = new TH2D("h_rgpt_gen", "x=logrg_gen, y=jtpt_gen", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_zgpt_gen = new TH2D("h_zgpt_gen", "x=zg_gen, y=jtpt_gen", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_mbpt_gen = new TH2D("h_mbpt_gen", "x=mb_gen, y=jtpt_gen", y1bins, y1binsVector, z1bins, z1binsVector);
    TH2D *h_zptpt_gen = new TH2D("h_zptpt_gen", "x=zpt_gen, y=jtpt_gen", x2bins, x2binsVector, z1bins, z1binsVector);

    // find correct SF based on reco 
    TFile *fin_sf_rg = new TFile("histos/aggrTMVA_inclusive_rg_sfs.root");
    TH2D *h_eff_sf_rg = (TH2D *) fin_sf_rg->Get("h_eff_sf")->Clone("h_eff_sf_rg");

    TFile *fin_sf_zg = new TFile("histos/aggrTMVA_inclusive_zg_sfs.root");
    TH2D *h_eff_sf_zg = (TH2D *) fin_sf_zg->Get("h_eff_sf")->Clone("h_eff_sf_zg");

    TFile *fin_sf_zpt = new TFile("histos/aggrTMVA_inclusive_zpt_sfs.root");
    TH2D *h_eff_sf_zpt = (TH2D *) fin_sf_zpt->Get("h_eff_sf")->Clone("h_eff_sf_zpt");

    std::cout << "filling histograms" << std::endl;
    for (Long64_t ient = 0; ient < tree_sig->GetEntries(); ient++) {
        // if (ient > 1) continue;
        tree_sig->GetEntry(ient);
        if (weight > 0.01) continue;

        if (jer_opt=="nom") jtpt = jtpt * jer_sf_nom;
        else if (jer_opt=="up") jtpt = jtpt * jer_sf_up;
        else if (jer_opt=="down") jtpt = jtpt * jer_sf_down;

        double jec_fact = 0; // nominal
        if (jec_opt=="up") jec_fact = 1;
        if (jec_opt=="down") jec_fact = -1;
        double newCorrection = 1 + (jec_fact * jec_unc);
        jtpt = jtpt * newCorrection;

        // Check for logrg 
        if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
        if (logrg>=2.5) logrg = 2.499; // overflow
        if (logrg_gen>=2.5) logrg_gen = 2.499; // overflow

        // Check for jtptCh
        if (std::abs(jtptCh) < 1e-4) continue;

        // Check for mb
        if (mb_gen >= 7.) mb_gen = 6.999; // overflow

        // Check for zg
        if (std::abs(zg-0.5)<1e-4) zg = 0.499; // fix max value
        if (std::abs(zg_gen-0.5)<1e-4) zg_gen = 0.499; // fix max value
        
        double zpt = -1.;
        if (jtptCh > 0) zpt = bpt / jtptCh;
        if (std::abs(zpt-1)<1e-3) zpt = 0.999;

        double zpt_gen = -1.;
        if (jtptCh_gen > 0) zpt_gen = bpt_gen / jtptCh_gen;
        if (std::abs(zpt_gen-1)<1e-3) zpt_gen = 0.999;

        double weight_rg = weight;
        double weight_zg = weight;
        double weight_mb = weight;
        double weight_zpt = weight;

        if (label.Contains("XXT") && applySF) {
            int ibin_pt = h_eff_sf_rg->GetYaxis()->FindBin(jtpt); // should be the same for all observables
            if (ibin_pt==0) ibin_pt = 1;
            if (ibin_pt==nbins_pt+1) ibin_pt = nbins_pt;

            int ibin_rg = h_eff_sf_rg->GetXaxis()->FindBin(logrg);
            int ibin_zg = h_eff_sf_zg->GetXaxis()->FindBin(zg);
            int ibin_zpt = h_eff_sf_zpt->GetXaxis()->FindBin(zpt);
            
            if (ibin_zg==0||ibin_zg>nbins_zg) std::cout << "YELL" << std::endl;
            double sf_rg = h_eff_sf_rg->GetBinContent(ibin_rg, ibin_pt);
            double sfUnc_rg = std::abs(sf_rg - 1.); 
            if (sfUp) sf_rg += sfUnc_rg;
            if (sfDown) sf_rg -= sfUnc_rg;
            weight_rg *= sf_rg;

            double sf_zg = h_eff_sf_zg->GetBinContent(ibin_zg, ibin_pt);
            double sfUnc_zg = std::abs(sf_zg - 1.); 
            if (sfUp) sf_zg += sfUnc_zg;
            if (sfDown) sf_zg -= sfUnc_zg;
            weight_zg *= sf_zg;

            double sf_zpt = h_eff_sf_zpt->GetBinContent(ibin_zpt, ibin_pt);
            double sfUnc_zpt = std::abs(sf_zpt - 1.); 
            if (sfUp) sf_zpt += sfUnc_zpt;
            if (sfDown) sf_zpt -= sfUnc_zpt;
            weight_zpt *= sf_zpt;
        }

        h_rgpt_gen->Fill(logrg_gen, jtpt_gen, weight_rg);
        h_zgpt_gen->Fill(zg_gen, jtpt_gen, weight_zg);
        h_mbpt_gen->Fill(mb_gen, jtpt_gen, weight_mb);
        h_zptpt_gen->Fill(zpt_gen, jtpt_gen, weight_zpt);
    }

    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_rgpt_gen, h_zgpt_gen, h_mbpt_gen, h_zptpt_gen}) {
        h->Write("", TObject::kOverwrite);
    }

    fout->Close();
    delete fout;
}
