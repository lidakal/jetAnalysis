// Useful methods of processes that come up often

void normalise_histo_1D(TH1D *h)
{
    // Normalise the histogram
    h->GetXaxis()->SetRange(0, h->GetNbinsX() + 1);
    h->Scale(1/h->Integral("width"));
    // Hide the under/overflow again
    h->GetXaxis()->SetRange(1, h->GetNbinsX());
}

void normalise_histo_2D(TH2D *h)
{
    // Normalise the histogram
    h->GetXaxis()->SetRange(0, h->GetNbinsX() + 1);
    h->GetYaxis()->SetRange(0, h->GetNbinsY() + 1);
    h->Scale(1/h->Integral("width"));
    // Hide the under/overflow again
    h->GetXaxis()->SetRange(1, h->GetNbinsX());
    h->GetYaxis()->SetRange(1, h->GetNbinsY());
}
