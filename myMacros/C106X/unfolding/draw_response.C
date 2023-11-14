void draw_response(TString observable="rg")
{
    Float_t font_size = 26.;
    gStyle->SetPalette(57);
    gStyle->SetPaintTextFormat(".2f"); 

    TString xlabel;
    if (observable=="rg") xlabel = "ln(#frac{0.4}{R_{g}})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z";

    TString sample = "bjet";
    TString label = "aggrTMVA_XXT";
    TString fin_name = "./histos/" + sample + "_" + label + "_response.root";   

    TFile *fin = new TFile(fin_name);
    TH2D *h_sig_training_purity = (TH2D *) fin->Get("h_sig_training_purity_" + observable + "pt");
    TH2D *h_sig_training_efficiency = (TH2D *) fin->Get("h_sig_training_efficiency_" + observable + "pt");
    RooUnfoldResponse *response = (RooUnfoldResponse *) fin->Get("response_" + observable + "pt");

    // Draw purity + efficiency 
    for (auto h : {h_sig_training_purity, h_sig_training_efficiency}) {
        h->GetXaxis()->SetTitleSize(font_size);    
        h->GetXaxis()->SetLabelSize(font_size);
        h->GetXaxis()->SetTitle(xlabel);

        h->GetYaxis()->SetTitleSize(font_size);    
        h->GetYaxis()->SetLabelSize(font_size);
        h->GetYaxis()->SetTitle("p_{T}^{jet}");

        h->SetMarkerSize(800);
    }

    TCanvas *c_purity = new TCanvas("c_purity", "purity", 800, 600);
    h_sig_training_purity->Draw("colz texte");
    c_purity->Draw();

    TCanvas *c_efficiency = new TCanvas("c_efficiency", "efficiency", 800, 600);
    h_sig_training_efficiency->Draw("colz texte");
    c_efficiency->Draw();

    // Draw the response matrix
    Int_t nbins_x = h_sig_training_purity->GetNbinsX(); 
    Int_t nbins_pt = h_sig_training_purity->GetNbinsY(); 

    TMatrixD response_matrix = response->Mresponse();
    TH2D *response_histogram = new TH2D(response_matrix);
    TCanvas *c_response = new TCanvas("c_response", "response", 800, 600);
    response_histogram->GetXaxis()->SetTitle("Detector level " + observable + " * p_{T}^{jet} bins");
    response_histogram->GetYaxis()->SetTitle("Particle level " + observable + " * p_{T}^{jet} bins");
    response_histogram->Draw("colz");
    for (int i = 1; i < nbins_pt; i++) {
        double coord = i * nbins_x;
        TLine *vline = new TLine(coord, 0, coord, nbins_x*nbins_pt);
        vline->SetLineColor(kBlack);
        vline->SetLineWidth(2);
        vline->Draw();

        TLine *hline = new TLine(0, coord, nbins_x*nbins_pt, coord);
        hline->SetLineColor(kBlack);
        hline->SetLineWidth(2);
        hline->Draw();
    }

    // Print condition number
    TDecompSVD *svd= new TDecompSVD(response->Mresponse());  // response is a RooUnfold response object, svd is the singular value decomposition (SVD) matrix. the response->Mresponse() returns the normalized migration matrix
    auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.
    double cond_number = singular_values.Max() / singular_values.Min();
    std::cout << "Largest value = " << singular_values.Max() 
              << "\nSmallest value = " << singular_values.Min()
              << "\nCondition number = " << cond_number
              << std::endl;

}