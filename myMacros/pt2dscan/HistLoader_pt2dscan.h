#include "TFile.h"
#include "TH3D.h"
#include "TH2D.h"
#include "TH1D.h"

// Class to load a 3D histo and create the projections
class HistLoader_pt2dscan
{
    public:
        TH3D *h3d; 

        // ------ Constructors -------
        HistLoader_pt2dscan() {};
        HistLoader_pt2dscan(std::string fname, std::string hname);

        TH2D * do_rgkt_projection(Float_t *ptrange, std::string hname);
        TH1D * do_rg_projection(Float_t *ptrange, std::string hname);
		TH1D * do_kt_projection(Float_t *ptrange, std::string hname);
};

HistLoader_pt2dscan::HistLoader_pt2dscan(std::string fname, std::string hname)
{
    TFile *fin = new TFile(fname.c_str());
    h3d = (TH3D *) fin->Get(hname.c_str());
}

TH2D * HistLoader_pt2dscan::do_rgkt_projection(Float_t *ptrange, std::string hname)
{
    TH3D *htemp = (TH3D *) h3d->Clone();
    htemp->GetZaxis()->SetRangeUser(ptrange[0], ptrange[1]);

    TH2D *h2d = (TH2D *) htemp->Project3D("yx");
	h2d->SetName(hname.c_str());
    h2d->Scale(1. / h2d->Integral("width"));

    std::string xtitle = "ln(1/R_{g})";
    std::string ytitle = "ln(k_{T})";

    h2d->GetXaxis()->SetTitle(xtitle.c_str());
	h2d->GetXaxis()->SetTitleOffset(2.5);
    h2d->GetYaxis()->SetTitle(ytitle.c_str());
   

    return h2d;
}

TH1D * HistLoader_pt2dscan::do_rg_projection(Float_t *ptrange, std::string hname)
{
    TH3D *htemp = (TH3D *) h3d->Clone();
    htemp->GetZaxis()->SetRangeUser(ptrange[0], ptrange[1]);
	htemp->GetYaxis()->SetRangeUser(0., 5.);


    TH1D *h1d = (TH1D *) htemp->Project3D("x");
	h1d->SetName(hname.c_str());
    h1d->Scale(1. / h1d->Integral("width"));

    std::string xtitle = "ln(1/R_{g})";
    std::string ytitle = "1 / N_{2-prog jets} dN/dln(1/R_{g})";

    h1d->GetXaxis()->SetTitle(xtitle.c_str());
    h1d->GetYaxis()->SetTitle(ytitle.c_str());

    return h1d;
}

TH1D * HistLoader_pt2dscan::do_kt_projection(Float_t *ptrange, std::string hname)
{
    TH3D *htemp = (TH3D *) h3d->Clone();
    htemp->GetZaxis()->SetRangeUser(ptrange[0], ptrange[1]);

    TH1D *h1d = (TH1D *) htemp->Project3D("y");
	h1d->SetName(hname.c_str());
    h1d->Scale(1. / h1d->Integral("width"));

    std::string xtitle = "ln(k_{T})";
    std::string ytitle = "1 / N_{2-prog jets} dN/dln(k_{T})";

    h1d->GetXaxis()->SetTitle(xtitle.c_str());
    h1d->GetYaxis()->SetTitle(ytitle.c_str());

    return h1d;
}
