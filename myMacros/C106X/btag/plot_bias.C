void plot_bias()
{
    // inclusive = non-tagged, but still true b jets
    // in both cases, use only bjet enhanced samples (pythia or herwig)

    TString sample = "bjet";
    TString label = "aggrTMVA_XXT";
    TString fin_name = "/data_CMS/cms/kalipoliti/compact_trees/" + sample + "_" + label + "_tree.root";
    TString fout_name = "histos/" + sample + "_" + label + "_histograms.root";

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

    // jet pt bins : no underflow or overflow
    const Int_t z1binsVectorSize = 4;
    Int_t z1bins = z1binsVectorSize - 1;
    Double_t z1binsVector[z1binsVectorSize] = {80., 
                                               100., 
                                               120.,
                                               140.};
    Double_t z1min = z1binsVector[0];
    Double_t z1max = z1binsVector[z1bins];

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
    const Int_t x2binsVectorSize = 8;
    Int_t x2bins = x2binsVectorSize - 1;
    Double_t x2binsVector[x2binsVectorSize] = {0., 
                                               0.4,
                                               0.5,
                                               0.6,
                                               0.7,
                                               0.8,
                                               0.9,
                                               1.};
    Double_t x2min = x2binsVector[0];
    Double_t x2max = x2binsVector[x2bins];

    TH2D *h_rgpt = new TH2D("h_rgpt", "x=logrg_gen, y=jtpt_gen", x1bins, x1binsVector, z1bins, z1binsVector);
    TH2D *h_zgpt = new TH2D("h_zgpt", "x=zg_gen, y=jtpt_gen", x3bins, x3binsVector, z1bins, z1binsVector);
    TH2D *h_mbpt = new TH2D("h_mbpt", "x=mb_gen, y=jtpt_gen", y1bins, y1binsVector, z1bins, z1binsVector);
    TH2D *h_zptpt = new TH2D("h_zptpt", "x=zpt_gen, y=jtpt_gen", x2bins, x2binsVector, z1bins, z1binsVector);

    std::cout << "filling histograms" << std::endl;
    for (Long64_t ient = 0; ient < tree_sig->GetEntries(); ient++) {
        tree_sig->GetEntry(ient);
        if (weight > 0.01) continue;

        // Check for logrg 
        if (std::abs(logrg - (-0.2)) > 1e-3 && logrg < 0.) continue;
        if (logrg_gen>=2.5) logrg_gen = 2.499; // overflow

        // Check for jtptCh
        if (std::abs(jtptCh) < 1e-4) continue;

        // Check for mb
        if (mb_gen >= 7.) mb_gen = 6.999; // overflow

        // Check for zg
        if (std::abs(zg_gen-0.5)<1e-4) zg_gen = 0.499; // fix max value

        double zpt_gen = -1.;
        if (jtptCh_gen > 0) zpt_gen = bpt_gen / jtptCh_gen;
        if (std::abs(zpt_gen-1)<1e-3) zpt_gen = 0.999;

        h_rgpt->Fill(logrg_gen, jtpt_gen, weight);
        h_zgpt->Fill(zg_gen, jtpt_gen, weight);
        h_mbpt->Fill(mb_gen, jtpt_gen, weight);
        h_zptpt->Fill(zpt_gen, jtpt_gen, weight);
    }

    std::cout << "creating " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {h_rgpt, h_zgpt, h_mbpt, h_zptpt}) {
        h->Write("", TObject::kOverwrite);
    }

    fout->Close();
    delete fout;
}