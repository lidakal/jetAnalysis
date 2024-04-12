void calculate_prescale()
{
    TString sample = "HighEGJet";
    TString fin_name = "histos/" + sample + "_prescale.root";
    TFile *fin = new TFile(fin_name);

    TString low = "40";
    TString high = "80";

    TH2D *h_hlt = (TH2D *) fin->Get("hHLT"+low);
    h_hlt->SetMarkerSize(1000);
    h_hlt->GetXaxis()->SetBinLabel(1, "NOT pass "+low);
    h_hlt->GetXaxis()->SetBinLabel(2, "pass "+low);
    h_hlt->GetYaxis()->SetBinLabel(1, "NOT pass "+high);
    h_hlt->GetYaxis()->SetBinLabel(2, "pass "+high);

    double passLowAndHigh = h_hlt->Integral(2, 2, 2, 2);
    double passHigh = h_hlt->Integral(1, 2, 2, 2);
    std::cout << sample << std::endl;
    std::cout << "prescale factor = " << passHigh / passLowAndHigh << std::endl;
    std::cout << "passLowAndHigh = " << passLowAndHigh << std::endl;

    // TH1D *hPass60 = (TH1D *) fin->Get("hPass60");
    // TH1D *hPass80 = (TH1D *) fin->Get("hPass80");
    // TH1D *hPass60butNot80 = (TH1D *) fin->Get("hPass60butNot80");

    // THStack *h_jtpt = new THStack("h_jtpt", "");
    // h_jtpt->SetTitle("; leading jet pT; # Events");

    // TLegend *leg_jtpt = new TLegend(0.5, 0.7, 0.9, 0.9);
    // leg_jtpt->SetFillStyle(0);

    // hPass60->SetLineColor(kBlue);
    // h_jtpt->Add(hPass60, "hist");
    // leg_jtpt->AddEntry(hPass60, "CaloJet60", "l");

    // hPass80->SetLineColor(kBlack);
    // h_jtpt->Add(hPass80, "hist");
    // leg_jtpt->AddEntry(hPass80, "CaloJet80", "l");

    // hPass60butNot80->SetLineColor(kRed);
    // hPass60butNot80->SetLineStyle(kDashed);
    // h_jtpt->Add(hPass60butNot80, "hist");
    // leg_jtpt->AddEntry(hPass60butNot80, "CaloJet60 && (!CaloJet80)", "l");


    TCanvas *c_hlt = new TCanvas("c_hlt", "", 1000, 600);
    h_hlt->Draw("col text");
}