// Useful methods of processes that come up often

void normalise_histo(TH1D *h)
{
    // Normalise the histogram
    h->GetXaxis()->SetRange(0, h->GetNbinsX() + 1);
    h->Scale(1/h->Integral("width"));
    // Hide the under/overflow again
    h->GetXaxis()->SetRange(1, h->GetNbinsX());
}

void normalise_histo(TH2D *h)
{
    // Normalise the histogram
    h->GetXaxis()->SetRange(0, h->GetNbinsX() + 1);
    h->GetYaxis()->SetRange(0, h->GetNbinsY() + 1);
    h->Scale(1/h->Integral("width"));
    // Hide the under/overflow again
    h->GetXaxis()->SetRange(1, h->GetNbinsX());
    h->GetYaxis()->SetRange(1, h->GetNbinsY());
}

void set_axes_labels(TH2D *h, string xtitle, string ytitle)
{
    h->SetYTitle(ytitle.c_str());
    h->SetXTitle(xtitle.c_str());
    h->GetXaxis()->SetTitleOffset(2.5);
    h->GetYaxis()->SetTitleOffset(2.5);
}

void set_axes_labels(TH1D *h, string xtitle, string ytitle)
{
    h->SetYTitle(ytitle.c_str());
    h->SetXTitle(xtitle.c_str());
    h->GetXaxis()->SetTitleOffset(2.5);
    h->GetYaxis()->SetTitleOffset(2.5);
}

void set_zrange(TH2D *h, Float_t zmin, Float_t zmax)
{
    h->GetZaxis()->SetRangeUser(zmin, zmax);
}
