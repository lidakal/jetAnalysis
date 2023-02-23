#include "TH2.h"
#include "TString.h"

void format_response(TH2F *h, 
                     TString var="ln(R/R_{g})")
{
    TString xtitle = "Detector level " + var;
    TString ytitle = "Particle level " + var;

    h->GetXaxis()->SetTitle(xtitle);
    h->GetYaxis()->SetTitle(ytitle);
    h->GetYaxis()->SetTitleOffset(1.);
}