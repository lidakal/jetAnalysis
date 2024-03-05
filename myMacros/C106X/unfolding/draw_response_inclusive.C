#include "../draw_utils.h"

void draw_response_inclusive()
{
    // setTDRStyle();

    Float_t font_size = 20.;
    gStyle->SetPalette(57);
    gStyle->SetPaintTextFormat(".2f"); 

    TString sample = "herwig_dijet";
    TString label = "aggrTMVA_inclusive";
    TString split_option = "full";
    TString suffix = "_jer_nom_jec_nom";
    TString fin_name = "./histos/" + sample + "_" + label + "_response_" + split_option + suffix + ".root";   

    TFile *fin = new TFile(fin_name);
    TH2D *h_training_purity_rgpt = (TH2D *) fin->Get("h_training_purity_rgpt");
    TH2D *h_training_efficiency_rgpt = (TH2D *) fin->Get("h_training_efficiency_rgpt");
    RooUnfoldResponse *response_rgpt = (RooUnfoldResponse *) fin->Get("response_rgpt");

    // Draw purity + efficiency 
    for (auto h : {h_training_purity_rgpt, h_training_efficiency_rgpt}) {
        h->GetXaxis()->SetTitleSize(font_size);    
        h->GetXaxis()->SetLabelSize(font_size);
        h->GetXaxis()->SetTitle("ln(0.4/R_{g})");

        h->GetYaxis()->SetTitleSize(font_size);    
        h->GetYaxis()->SetLabelSize(font_size);
        h->GetYaxis()->SetTitle("p_{T}^{jet}");

        h->SetMarkerSize(800);
    }

    TCanvas *c_purity = new TCanvas("c_purity", "purity", 200, 10, 800, 600);
    h_training_purity_rgpt->GetZaxis()->SetTitle("Reconstruction purity");
    h_training_purity_rgpt->Draw("colz texte");
    drawHeaderHerwig();
    c_purity->Draw();
    c_purity->Print("plots_an/"+sample+"_"+label+"_purity.png");

    TCanvas *c_efficiency = new TCanvas("c_efficiency", "efficiency", 800, 600);
    h_training_efficiency_rgpt->GetZaxis()->SetTitle("Reconstruction efficiency");
    h_training_efficiency_rgpt->Draw("colz texte");
    drawHeaderHerwig();
    c_efficiency->Draw();
    c_efficiency->Print("plots_an/"+sample+"_"+label+"_efficiency.png");
    

    // Draw the response matrix
    Int_t nbins_rg = h_training_purity_rgpt->GetNbinsX(); 
    Int_t nbins_pt = h_training_purity_rgpt->GetNbinsY(); 

    TMatrixD response_matrix = response_rgpt->Mresponse();
    TH2D *response_histogram = new TH2D(response_matrix);
    TCanvas *c_response = new TCanvas("c_response", "response", 800, 600);
    response_histogram->GetXaxis()->SetTitle("Detector level ln(0.4/R_{g}) * p_{T}^{jet} bins");
    response_histogram->GetYaxis()->SetTitle("Particle level ln(0.4/R_{g}) * p_{T}^{jet} bins");
    response_histogram->GetZaxis()->SetTitle("Migration probability");
    response_histogram->Draw("colz");
    for (int i = 1; i < nbins_pt; i++) {
        double coord = i * nbins_rg;
        TLine *vline = new TLine(coord, 0, coord, nbins_rg*nbins_pt);
        vline->SetLineColor(kBlack);
        vline->SetLineWidth(2);
        vline->Draw();

        TLine *hline = new TLine(0, coord, nbins_rg*nbins_pt, coord);
        hline->SetLineColor(kBlack);
        hline->SetLineWidth(2);
        hline->Draw();
    }
    drawHeaderHerwig();
    c_response->Print("plots_an/"+sample+"_"+label+"_response.png");

    // Print condition number
    TDecompSVD *svd= new TDecompSVD(response_rgpt->Mresponse());  // response is a RooUnfold response object, svd is the singular value decomposition (SVD) matrix. the response->Mresponse() returns the normalized migration matrix
    auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.
    double cond_number = singular_values.Max() / singular_values.Min();
    std::cout << "Largest value = " << singular_values.Max() 
              << "\nSmallest value = " << singular_values.Min()
              << "\nCondition number = " << cond_number
              << std::endl;

}