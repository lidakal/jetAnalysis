void plot_inputs(TString model="qcd_bjet_TMVA")
{
    // need to plot again because the 
    TString fin_name = "saved_models/"+model+".root";
    std::cout << "fin: " << fin_name << std::endl;
    TFile *fin = new TFile(fin_name);
    TTree *test = (TTree *) fin->Get("qcd_bjet_dataloader/TestTree");
    TTree *train = (TTree *) fin->Get("qcd_bjet_dataloader/TrainTree");
    
    // Use only train tree for now 
    Int_t classID; 
    Float_t trkIp3dSig;
    Float_t trkIp2dSig;
    Float_t trkDistToAxis;
    Float_t svtxdls;
    Float_t svtxdls2d;
    Float_t svtxm;
    Float_t svtxmcorr;
    Float_t svtxnormchi2;
    Float_t svtxNtrk;
    Float_t svtxTrkPtOverSv;
    Float_t jtpt;

    train->SetBranchAddress("classID", &classID);
    train->SetBranchAddress("trkIp3dSig", &trkIp3dSig);
    train->SetBranchAddress("trkIp2dSig", &trkIp2dSig);
    train->SetBranchAddress("trkDistToAxis", &trkDistToAxis);
    train->SetBranchAddress("svtxdls", &svtxdls);
    train->SetBranchAddress("svtxdls2d", &svtxdls2d);
    train->SetBranchAddress("svtxm", &svtxm);
    train->SetBranchAddress("svtxmcorr", &svtxmcorr);
    train->SetBranchAddress("svtxnormchi2", &svtxnormchi2);
    train->SetBranchAddress("svtxNtrk", &svtxNtrk);
    train->SetBranchAddress("svtxTrkPtOverSv", &svtxTrkPtOverSv);
    train->SetBranchAddress("jtpt", &jtpt);

    TH1F *h_trkIp3dSig_sig = new TH1F("h_trkIp3dSig_sig", "", 40, -60., 100.);
    TH1F *h_trkIp2dSig_sig = new TH1F("h_trkIp2dSig_sig", "", 40, -60., 100.);
    TH1F *h_trkDistToAxis_sig = new TH1F("h_trkDistToAxis_sig", "", 40, -0.12, 0.);
    TH1F *h_svtxdls_sig = new TH1F("h_svtxdls_sig", "", 10, 0., 100.);
    TH1F *h_svtxdls2d_sig = new TH1F("h_svtxdls2d_sig", "", 10, 0., 100.);
    TH1F *h_svtxm_sig = new TH1F("h_svtxm_sig", "", 10, 0., 5.);
    TH1F *h_svtxmcorr_sig = new TH1F("h_svtxmcorr_sig", "", 10, 0., 5.);
    TH1F *h_svtxnormchi2_sig = new TH1F("h_svtxnormchi2_sig", "", 10, 0., 5.);
    TH1F *h_svtxNtrk_sig = new TH1F("h_svtxNtrk_sig", "", 12, 0., 12.);
    TH1F *h_svtxTrkPtOverSv_sig = new TH1F("h_svtxTrkPtOverSv_sig", "", 10, 0., 1.);
    TH1F *h_jtpt_sig = new TH1F("h_jtpt_sig", "", 27, 30., 300.);

    TH1F *h_trkIp3dSig_bkg = new TH1F("h_trkIp3dSig_bkg", "", 40, -60., 100.);
    TH1F *h_trkIp2dSig_bkg = new TH1F("h_trkIp2dSig_bkg", "", 40, -60., 100.);
    TH1F *h_trkDistToAxis_bkg = new TH1F("h_trkDistToAxis_bkg", "", 40, -0.12, 0.);
    TH1F *h_svtxdls_bkg = new TH1F("h_svtxdls_bkg", "", 10, 0., 100.);
    TH1F *h_svtxdls2d_bkg = new TH1F("h_svtxdls2d_bkg", "", 10, 0., 100.);
    TH1F *h_svtxm_bkg = new TH1F("h_svtxm_bkg", "", 10, 0., 5.);
    TH1F *h_svtxmcorr_bkg = new TH1F("h_svtxmcorr_bkg", "", 10, 0., 5.);
    TH1F *h_svtxnormchi2_bkg = new TH1F("h_svtxnormchi2_bkg", "", 10, 0., 5.);
    TH1F *h_svtxNtrk_bkg = new TH1F("h_svtxNtrk_bkg", "", 12, 0., 12.);
    TH1F *h_svtxTrkPtOverSv_bkg = new TH1F("h_svtxTrkPtOverSv_bkg", "", 10, 0., 1.);
    TH1F *h_jtpt_bkg = new TH1F("h_jtpt_bkg", "", 27, 30., 300.);

    for (Long64_t ient=0; ient<train->GetEntries(); ient++) {
        // if (ient > 10) break;
        train->GetEntry(ient);
        // std::cout << trkIp3dSig << std::endl;

        if (classID==1) {
            h_trkIp3dSig_bkg->Fill(trkIp3dSig);
            h_trkIp2dSig_bkg->Fill(trkIp2dSig);
            h_trkDistToAxis_bkg->Fill(trkDistToAxis);
            h_svtxdls_bkg->Fill(svtxdls);
            h_svtxdls2d_bkg->Fill(svtxdls2d);
            h_svtxm_bkg->Fill(svtxm);
            h_svtxmcorr_bkg->Fill(svtxmcorr);
            h_svtxnormchi2_bkg->Fill(svtxnormchi2);
            h_svtxNtrk_bkg->Fill(svtxNtrk);
            h_svtxTrkPtOverSv_bkg->Fill(svtxTrkPtOverSv);
            h_jtpt_bkg->Fill(jtpt);
        } else if (classID==0) {
            h_trkIp3dSig_sig->Fill(trkIp3dSig);
            h_trkIp2dSig_sig->Fill(trkIp2dSig);
            h_trkDistToAxis_sig->Fill(trkDistToAxis);
            h_svtxdls_sig->Fill(svtxdls);
            h_svtxdls2d_sig->Fill(svtxdls2d);
            h_svtxm_sig->Fill(svtxm);
            h_svtxmcorr_sig->Fill(svtxmcorr);
            h_svtxnormchi2_sig->Fill(svtxnormchi2);
            h_svtxNtrk_sig->Fill(svtxNtrk);
            h_svtxTrkPtOverSv_sig->Fill(svtxTrkPtOverSv);
            h_jtpt_sig->Fill(jtpt);
        }
    }

    TString fout_name = "histos/"+model+"_inputs.root";
    std::cout << "fout: " << fout_name << std::endl;
    TFile *fout = new TFile(fout_name, "recreate");

    for (auto h : {
        h_trkIp3dSig_sig,
        h_trkIp2dSig_sig,
        h_trkDistToAxis_sig,
        h_svtxdls_sig,
        h_svtxdls2d_sig,
        h_svtxm_sig,
        h_svtxmcorr_sig,
        h_svtxnormchi2_sig,
        h_svtxNtrk_sig,
        h_svtxTrkPtOverSv_sig,
        h_jtpt_sig,
        h_trkIp3dSig_bkg,
        h_trkIp2dSig_bkg,
        h_trkDistToAxis_bkg,
        h_svtxdls_bkg,
        h_svtxdls2d_bkg,
        h_svtxm_bkg,
        h_svtxmcorr_bkg,
        h_svtxnormchi2_bkg,
        h_svtxNtrk_bkg,
        h_svtxTrkPtOverSv_bkg,
        h_jtpt_bkg
    }) {
        h->Write();
    }

    fout->Close();
    delete fout;
}