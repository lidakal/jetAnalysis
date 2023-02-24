#include "TH2.h"
#include "TString.h"
#include "TCanvas.h"

#include "root_utils.h"

void normalize_probability(TH2F *h) {
    Int_t nbinsX = h->GetNbinsX();
    Int_t nbinsY = h->GetNbinsY();

    for (Int_t ibinY = 1; ibinY <= nbinsY; ibinY++) {
        Float_t integral = h->Integral(1, nbinsX, ibinY, ibinY);
        // std::cout << "binY"  << ibinY << " integral " << integral << std::endl;
        for (Int_t ibinX = 1; ibinX <= nbinsX; ibinX++) {
            Float_t binContent = h->GetBinContent(ibinX, ibinY);
            Float_t newBinContent = binContent / integral;
            // std::cout << "\tbinX " << ibinX << " binContent " << binContent << std::endl;
            // std::cout << "\tbinX " << ibinX << " newBinContent " << newBinContent << std::endl;
            h->SetBinContent(ibinX, ibinY, newBinContent);
        }
    }
}

void format_response(TH2F *h, 
                     TString var="ln(R/R_{g})") {
    TString xtitle = "Detector level " + var;
    TString ytitle = "Particle level " + var;

    h->GetXaxis()->SetTitle(xtitle);
    h->GetYaxis()->SetTitle(ytitle);
    h->GetYaxis()->SetTitleOffset(1.5);
}

void format_probability(TH2F *h, 
                        TString var="ln(R/R_{g})") {
    TString xtitle = "Detector level " + var;
    TString ytitle = "Particle level " + var;

    h->GetXaxis()->SetTitle(xtitle);
    h->GetYaxis()->SetTitle(ytitle);
    h->GetYaxis()->SetTitleOffset(1.5);

    normalize_probability(h);
}

void draw_canvas_response(TCanvas *c, TH2F *h, TString cname) {
    h->Draw("colz");
    c->Draw();
    c->SetLogz();
    SetRealAspectRatio(c);
    c->Print(cname);
}


 