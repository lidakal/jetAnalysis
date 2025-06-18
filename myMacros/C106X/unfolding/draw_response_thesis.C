// #include "../draw_utils.h"
#include "../binning.h"
#include "../cms_palette.h"

void drawHeader(bool herwig=false) {
    TLatex *prelim = new TLatex;
    prelim->SetNDC();
    prelim->SetTextSize(28);
    prelim->SetTextAlign(13);
    prelim->DrawLatex(0.17, 0.95, "#bf{CMS} #it{Private work}");

    TLatex *lumi = new TLatex;
    lumi->SetNDC();
    lumi->SetTextSize(28);
    lumi->SetTextAlign(33);
    if (herwig) lumi->DrawLatex(0.85, 0.95, "HERWIG7 (pp 5.02 TeV)");
    else lumi->DrawLatex(0.85, 0.95, "PYTHIA8 (pp 5.02 TeV)");
}

void draw_response_thesis(TString observable="rg")
{
    // Setup plot
    Float_t text_size = 28.;
    gStyle->SetTextSize(text_size);
    gStyle->SetLegendTextSize(text_size-4);
    gStyle->SetLabelSize(text_size-4, "XYZ");
    gStyle->SetTitleSize(text_size, "XYZ");
    gStyle->SetPalette(57);
    gStyle->SetPaintTextFormat(".2f"); 

    TString xlabel;
    if (observable=="rg") xlabel = "ln(R/R_{g})";
    else if (observable=="zg") xlabel = "z_{g}";
    else if (observable=="zpt") xlabel = "z_{b,ch}";

    TString sample = "herwig_dijet_official_PF40";
    TString label = "aggrTMVA_inclusive";
    TString fin_name = "./histos/" + sample + "_" + label + "_response_jer_nom_jec_nom.root";   
    bool is_herwig = sample.Contains("herwig");

    std::cout << "File in: " << fin_name << std::endl;
    std::cout << "is_herwig = " << is_herwig << std::endl;
    TFile *fin = new TFile(fin_name);
    TH2D *h_purity;
    TH2D *h_efficiency;

    h_purity = (TH2D *) fin->Get("h_full_purity_" + observable + "pt");
    h_efficiency = (TH2D *) fin->Get("h_full_efficiency_" + observable + "pt");
    
    RooUnfoldResponse *response = (RooUnfoldResponse *) fin->Get("response_full_" + observable + "pt");

    // Draw purity + efficiency 
    for (auto h : {h_purity, h_efficiency}) {
        h->GetXaxis()->SetTitle(xlabel);
        h->GetYaxis()->SetTitle("p_{T}^{jet}");
        // std::cout << "h_purity->GetZaxis()->GetTitleOffset() = " << h->GetZaxis()->GetTitleOffset() << std::endl;
        h->GetZaxis()->SetTitleOffset(1.2);
        h->GetZaxis()->SetRangeUser(0.,1.);
        h->SetMarkerSize(800);
    }

    TCanvas *c_purity = new TCanvas("c_purity", "purity", 800, 400);
    c_purity->SetBottomMargin(0.2);
    c_purity->SetTopMargin(0.13);

    h_purity->GetZaxis()->SetTitle("Reconstruction purity");
    h_purity->GetXaxis()->SetTitleOffset(1.15);
    h_purity->GetYaxis()->SetTitleOffset(1.);
    h_purity->GetZaxis()->SetTitleOffset(0.65);

    h_purity->Draw("colz text");
    h_purity->GetXaxis()->ChangeLabel(1, -1, 0.0, -1, -1, -1, " ");
    drawHeader(is_herwig);

    if (observable!="zpt") {
        TPaveText *untagged_text = new TPaveText(0.17, 0.02, 0.375, 0.19, "NDC");
        untagged_text->SetFillColor(0);
        untagged_text->SetBorderSize(0);
        untagged_text->SetTextAlign(22);
        untagged_text->SetTextSize(text_size-4);
        untagged_text->AddText("SD-untagged");
        untagged_text->AddText("or k_{T} < 1 GeV/c");
        untagged_text->Draw();
    }
    
    c_purity->Draw();
    c_purity->Print("../plots_thesis/"+sample+"_"+label+"_purity_"+observable+".pdf");

    TCanvas *c_efficiency = new TCanvas("c_efficiency", "efficiency", 800, 400);
    c_efficiency->SetBottomMargin(0.2);
    c_efficiency->SetTopMargin(0.13);

    h_efficiency->GetZaxis()->SetTitle("Reconstruction efficiency");
    h_efficiency->GetXaxis()->SetTitleOffset(1.15);
    h_efficiency->GetYaxis()->SetTitleOffset(1.);
    h_efficiency->GetZaxis()->SetTitleOffset(0.65);

    h_efficiency->Draw("colz text");
    h_efficiency->GetXaxis()->ChangeLabel(1, -1, 0.0, -1, -1, -1, " ");
    drawHeader(is_herwig);

    if (observable!="zpt") {
        TPaveText *untagged_text = new TPaveText(0.17, 0.02, 0.375, 0.19, "NDC");
        untagged_text->SetFillColor(0);
        untagged_text->SetBorderSize(0);
        untagged_text->SetTextAlign(22);
        untagged_text->SetTextSize(text_size-4);
        untagged_text->AddText("SD-untagged");
        untagged_text->AddText("or k_{T} < 1 GeV/c");
        untagged_text->Draw();
    }
    
    c_efficiency->Draw();
    c_efficiency->Print("../plots_thesis/"+sample+"_"+label+"_efficiency_"+observable+".pdf");

    // Draw the response matrix
    Int_t nbins_x = h_purity->GetNbinsX(); 
    Int_t nbins_pt = h_purity->GetNbinsY(); 

    TMatrixD response_matrix = response->Mresponse();
    TH2D *response_histogram = new TH2D(response_matrix);

    // logrg_binsVectorSize, zg_binsVectorSize, zpt_binsVectorSize

    const Int_t nx = (observable=="zpt") ? zpt_binsVectorSize-1 : ((observable=="rg") ? logrg_binsVectorSize-1 : zg_binsVectorSize-1);
    const Int_t ny = jtpt_binsVectorSize-1;  
    Double_t new_bins[(ny*nx)+1];
    for (int i=0; i<(ny*nx)+1; i++) {
        new_bins[i] = -999.;
    }

    Double_t *v = (observable=="zpt") ? zpt_binsVector : ((observable=="rg") ? logrg_binsVector : zg_binsVector);

    for (int j=0; j<ny; j++) {
        for (int i=0; i<=nx; i++) {
            // std::cout << new_bins[j*nx + i] << std::endl;
            if (new_bins[j*nx + i] > -900.) continue; // skip already filled bins
            double previous_edge = (i==0&&j==0) ? 0 : new_bins[j*nx + i - 1];
            double new_edge = (j>0) ? previous_edge + (v[i] - v[i-1]) : v[i];
            // std::cout 
            //     << "bin " << j*nx + i
            //     << "\n\t i: " << i
            //     << "\n\t j: " << j
            //     << "\n\t v[i]: " << v[i]
            //     << "\n\t previous edge: " << previous_edge
            //     << "\n\t new edge: " << new_edge
            //     << std::endl;

            new_bins[j*nx + i] = new_edge;
        }
    }

    TH2D *h_real_response = new TH2D("h_real_response", "Real response matrix", nx*ny, new_bins, nx*ny, new_bins);

    // std::cout << "h_real_response->GetNbinsX() = " << h_real_response->GetNbinsX() << std::endl;
    // std::cout << "h_real_response->GetNbinsY() = " << h_real_response->GetNbinsY() << std::endl;

    for (int i = 1; i <= response_histogram->GetNbinsX(); i++) {
        for (int j = 1; j <= response_histogram->GetNbinsY(); j++) {
            double value = response_histogram->GetBinContent(i, j);
            h_real_response->SetBinContent(i, j, value);
        }
    }

    if (true) {
        TCanvas *c_response = new TCanvas("c_response", "response", 800, 700);
        // c_response->SetLogz();
        h_real_response->GetXaxis()->SetTitle("Detector level " + xlabel + " #times p_{T}^{jet} bins");
        h_real_response->GetXaxis()->SetTitleOffset(1.7);
        h_real_response->GetYaxis()->SetTitle("Particle level " + xlabel + " #times p_{T}^{jet} bins");
        h_real_response->GetYaxis()->SetTitleOffset(1.7);
        h_real_response->GetZaxis()->SetTitle("Migration probability");
        h_real_response->GetZaxis()->SetTitleOffset(1.2);
        h_real_response->GetZaxis()->SetRangeUser(0.,1.);
        h_real_response->Draw("colz");

        // remove equidistant ticks
        h_real_response->GetXaxis()->SetLabelSize(0);
        h_real_response->GetXaxis()->SetTickSize(0);

        h_real_response->GetYaxis()->SetLabelSize(0);
        h_real_response->GetYaxis()->SetTickSize(0);

        double y_min = new_bins[0];
        double y_max = new_bins[nx*ny]; 

        // std::cout << "y_max = " << y_max << std::endl;

        // add custom ticks
        double tickLength = (observable=="zpt") ? 0.05 : ((observable=="rg") ? 0.3 : 0.05);
        for (int i = 0; i < nx*ny; ++i) {
            double coord = new_bins[i];
            TLine *xtick = new TLine(coord, y_min, coord, y_min + tickLength);
            xtick->SetLineColor(kBlack);
            xtick->SetLineWidth(1);
            xtick->Draw();

            TLine *ytick = new TLine(y_min, coord, y_min + tickLength, coord);
            ytick->SetLineColor(kBlack);
            ytick->SetLineWidth(1);
            ytick->Draw();
        }

        // TLatex *pt_label = new TLatex;
        // pt_label->SetTextSize(text_size-4);
        // pt_label->DrawLatex(-2.5, -2., "p_{T}^{jet}");

        double multiplier = (observable=="zpt") ? 1. : ((observable=="rg") ? 2 : 3);
        double offset = (observable=="zpt") ? -0.3 : 0.;
        TPaveText *ptlabel = new TPaveText((y_min+offset)*multiplier, (y_min+offset)*multiplier, y_min, y_min);
        ptlabel->SetBorderSize(0);
        ptlabel->SetTextSize(text_size-4);
        ptlabel->SetTextAlign(22);
        ptlabel->SetFillColor(0);
        ptlabel->SetFillStyle(0);
        auto text = ptlabel->AddText("p_{T}^{jet}");
        text->SetTextAngle(45);
        ptlabel->Draw();
        

        // add vertical lines to split pt bins
        for (int i = 0; i < nbins_pt; i++) {
            double coord = new_bins[i * nbins_x];
            TLine *vline = new TLine(coord, y_min, coord, y_max);
            vline->SetLineColor(kBlack);
            vline->SetLineWidth(1);
            vline->Draw();

            TLine *hline = new TLine(y_min, coord, y_max, coord);
            hline->SetLineColor(kBlack);
            hline->SetLineWidth(1);
            hline->Draw();

            TPaveText *xptlabel = new TPaveText(new_bins[i * nbins_x], (y_min+offset)*multiplier, new_bins[(i+1) * nbins_x], y_min);
            xptlabel->SetBorderSize(0);
            xptlabel->SetTextSize(text_size-4);
            xptlabel->SetTextAlign(22);
            xptlabel->SetFillColor(0);
            xptlabel->SetFillStyle(0);
            xptlabel->AddText(Form("[%.0f,%.0f]", h_purity->GetYaxis()->GetBinLowEdge(i+1), h_purity->GetYaxis()->GetBinUpEdge(i+1)));
            xptlabel->Draw();

            TPaveText *yptlabel = new TPaveText((y_min+offset)*multiplier, new_bins[i * nbins_x], y_min, new_bins[(i+1) * nbins_x]);
            yptlabel->SetBorderSize(0);
            yptlabel->SetTextSize(text_size-4);
            yptlabel->SetTextAlign(22);
            yptlabel->SetFillColor(0);
            yptlabel->SetFillStyle(0);
            auto text = yptlabel->AddText(Form("[%.0f,%.0f]", h_purity->GetYaxis()->GetBinLowEdge(i+1), h_purity->GetYaxis()->GetBinUpEdge(i+1)));
            text->SetTextAngle(90);
            yptlabel->Draw();
        }
        drawHeader(is_herwig);
        c_response->Print("../plots_thesis/"+sample+"_"+label+"_response_"+observable+".pdf");
    }

    // DRAW SPECIFIC PT BIN
    if (false) {
        int ibin_pt = 2;
        TCanvas *c_response_i = new TCanvas(Form("c_response_%d",ibin_pt), "response", 800, 600);
        response_histogram->GetXaxis()->SetRange(((ibin_pt-1)*nbins_x)+1,ibin_pt*nbins_x);
        response_histogram->GetYaxis()->SetRange(((ibin_pt-1)*nbins_x)+1,ibin_pt*nbins_x);
        response_histogram->GetXaxis()->SetTitle("Detector level " + xlabel + Form(", p_{T}^{jet} bin = %d", ibin_pt));
        response_histogram->GetYaxis()->SetTitle("Particle level " + xlabel + Form(", p_{T}^{jet} bin = %d", ibin_pt));
        for (int i=1;i<=nbins_x+1;i++) {
            TString label = Form("%.2f",h_purity->GetXaxis()->GetBinLowEdge(i));
            response_histogram->GetYaxis()->ChangeLabel(i, -1, -1, -1, -1, -1, label);
            response_histogram->GetXaxis()->ChangeLabel(i, -1, -1, -1, -1, -1, label);
        }
        response_histogram->GetZaxis()->SetRangeUser(0, 0.7);

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
    }

    // Print condition number
    TDecompSVD *svd= new TDecompSVD(response->Mresponse());  // response is a RooUnfold response object, svd is the singular value decomposition (SVD) matrix. the response->Mresponse() returns the normalized migration matrix
    auto singular_values = svd->GetSig(); //this is a vector with the singular values, i.e., the diagonal elements of S. They are ordered from largest to smallest.
    double cond_number = singular_values.Max() / singular_values.Min();
    for (int i=0; i < singular_values.GetNrows(); i++) {
        double val = singular_values[i];
        // std::cout << val << std::endl;
    }
    std::cout << "Largest value = " << singular_values.Max() 
              << "\nSmallest value = " << singular_values.Min()
              << "\nCondition number = " << cond_number
              << std::endl;

}