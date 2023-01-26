void normalize_histo(TH1D *h, Int_t ixmin=0, Int_t ixmax=-1)
{
    if (ixmax == -1) ixmax = h->GetNbinsX() + 1;
    h->GetXaxis()->SetRange(ixmin, ixmax);
    h->Scale(1 / h->Integral("width"));
    h->GetXaxis()->SetRange(1, h->GetNbinsX());
}

void format_histo(TH1D *h, int color, int width=2)
{
    h->SetLineColor(color);
    h->SetLineWidth(width);
}

void format_histo(TH1I *h, int color, int width=2)
{
    h->SetLineColor(color);
    h->SetLineWidth(width);
}