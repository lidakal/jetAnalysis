// Compare Lida's vs Jelena's ntuples of the same pp sample

void compare_substructure()
{
    // Load histograms

    TFile *fin_Lida = new TFile("histos/substructure_Lida.root");
    TH3D *h_rg_kt_jtpt_Lida = (TH3D *) fin_Lida->Get("h_rg_kt_jtpt")->Clone("h_rg_kt_jtpt_Lida");
    TH3D *h_zg_kt_jtpt_Lida = (TH3D *) fin_Lida->Get("h_zg_kt_jtpt")->Clone("h_zg_kt_jtpt_Lida");

    TFile *fin_Jelena = new TFile("histos/substructure_Jelena.root");
    TH3D *h_rg_kt_jtpt_Jelena = (TH3D *) fin_Jelena->Get("h_rg_kt_jtpt")->Clone("h_rg_kt_jtpt_Jelena");
    TH3D *h_zg_kt_jtpt_Jelena = (TH3D *) fin_Jelena->Get("h_zg_kt_jtpt")->Clone("h_zg_kt_jtpt_Jelena");

    // Make projections and normalize

    TH2D *h_rg_kt_Lida = (TH2D *) h_rg_kt_jtpt_Lida->Project3D("yx");
    h_rg_kt_Lida->Scale(1.0 / h_rg_kt_Lida->Integral());

    TH2D *h_rg_kt_Jelena = (TH2D *) h_rg_kt_jtpt_Jelena->Project3D("yx");
    h_rg_kt_Jelena->Scale(1.0 / h_rg_kt_Jelena->Integral());

    // Take ratios 

    TH2D *h_rg_kt_ratio = (TH2D *) h_rg_kt_Lida->Clone("h_rg_kt_ratio");
    h_rg_kt_ratio->Divide(h_rg_kt_Jelena);

    // Draw
    
    TCanvas *c_Lida = new TCanvas("c_Lida", "c_Lida", 800, 600);
    h_rg_kt_Lida->Draw("colz");

    TCanvas *c_Jelena = new TCanvas("c_Jelena", "c_Jelena", 800, 600);
    h_rg_kt_Jelena->Draw("colz");

    TCanvas *c_ratio = new TCanvas("c_ratio", "c_ratio", 800, 600);
    h_rg_kt_ratio->Draw("colz");

}