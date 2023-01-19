void normalize_histo(TH1D *h)
{
    h->GetXaxis()->SetRange(0, h->GetNbinsX() + 1);
    h->Scale(1 / h->Integral("width"));
    h->GetXaxis()->SetRange(1, h->GetNbinsX());
}

void format_histo(TH1D *h, int color, int width=2)
{
    h->SetLineColor(color);
    h->SetLineWidth(width);
}