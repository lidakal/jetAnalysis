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

        TH2D * do_2d_projection(std::string axes = "yx", Float_t *zrange = {}, std::string hname = "", 
                                std::string xtitle = "ln(1/R_{g})", std::string ytitle = "ln(k_{T})");
                                
        TH1D * do_1d_projection(std::string axis = "x", Float_t *zrange = {}, Float_t *yrange = {}, std::string hname = "", 
                                std::string xtitle = "ln(1/R_{g})", std::string ytitle = "1 / N_{2-prog jets} dN/dln(1/R_{g})"); 
};

HistLoader_pt2dscan::HistLoader_pt2dscan(std::string fname, std::string hname)
{
    TFile *fin = new TFile(fname.c_str());
    h3d = (TH3D *) fin->Get(hname.c_str());
}

TH2D * HistLoader_pt2dscan::do_2d_projection(std::string axes = "yx", Float_t *zrange = {}, std::string hname = "", 
                                             std::string xtitle = "ln(1/R_{g})", std::string ytitle = "ln(k_{T})")
{
    TH3D *htemp = (TH3D *) h3d->Clone();

    if (zrange != nullptr) {
        htemp->GetZaxis()->SetRangeUser(zrange[0], zrange[1]);
    }

    TH2D *h2d = (TH2D *) htemp->Project3D(axes.c_str());
	h2d->SetName(hname.c_str());
    h2d->Scale(1. / h2d->Integral("width"));

    h2d->GetXaxis()->SetTitle(xtitle.c_str());
	h2d->GetXaxis()->SetTitleOffset(2.5);

    h2d->GetYaxis()->SetTitle(ytitle.c_str());
    h2d->GetYaxis()->SetTitleOffset(1.);
   
    return h2d;
}

TH1D * HistLoader_pt2dscan::do_1d_projection(std::string axis = "x", Float_t *zrange = nullptr, Float_t *yrange = nullptr, std::string hname = "", 
                                             std::string xtitle = "ln(1/R_{g})", std::string ytitle = "1 / N_{2-prog jets} dN/dln(1/R_{g})")
{
    TH3D *htemp = (TH3D *) h3d->Clone();

    if (zrange != nullptr) {
        htemp->GetZaxis()->SetRangeUser(zrange[0], zrange[1]);
    }

    if (yrange != nullptr) {
        if (axis == "x" ) {
	        htemp->GetYaxis()->SetRangeUser(yrange[0], yrange[1]);
        } else if (axis == "y") {
            htemp->GetXaxis()->SetRangeUser(yrange[0], yrange[1]);
        }
    }

    TH1D *h1d = (TH1D *) htemp->Project3D(axis.c_str());
	h1d->SetName(hname.c_str());

    // Rebin histo to make it easier to read
    h1d->Rebin(2);
    h1d->Scale(1. / h1d->Integral("width"));

    h1d->GetXaxis()->SetTitle(xtitle.c_str());
    h1d->GetYaxis()->SetTitle(ytitle.c_str());

    //if (yrange[0] >= 0) {
    //    h1d->GetXaxis()->SetRangeUser(0.91, 3.5);
    //}

    return h1d;
}
