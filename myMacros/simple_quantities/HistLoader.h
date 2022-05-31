#include "TFile.h"
#include "TH3D.h"
#include "TH2D.h"
#include "TH1D.h"

// Class to load a 3D histo and create the projections
class HistLoader
{
    public:
        TH3D *h3d; 
        Int_t sjtID;

        // ------ Constructors -------
        HistLoader() {};
        HistLoader(std::string fname, std::string hname, Int_t id);

        TH1D * do_dpt_projection(Float_t *ptrange, std::string hname);
		TH1D * do_dr_projection(Float_t *ptrange, std::string hname);
};

HistLoader::HistLoader(std::string fname, std::string hname, Int_t id)
{
    TFile *fin = new TFile(fname.c_str());
    h3d = (TH3D *) fin->Get(hname.c_str());
    sjtID = id;
}

TH1D * HistLoader::do_dpt_projection(Float_t *ptrange, std::string hname)
{
    TH3D *htemp = (TH3D *) h3d->Clone();
    htemp->GetZaxis()->SetRangeUser(ptrange[0], ptrange[1]);

    TH1D *h1d = (TH1D *) htemp->Project3D("x");
	h1d->SetName(hname.c_str());
    h1d->Scale(1. / h1d->Integral("width"));

    std::string xtitle = Form("#Delta p_{T%d}^{reco-gen}", sjtID);
    std::string ytitle = Form("1 / N_{2-prog jets} dN/d#Delta p_{T%d}^{reco-gen}", sjtID);

    h1d->GetXaxis()->SetTitle(xtitle.c_str());
    h1d->GetYaxis()->SetTitle(ytitle.c_str());

    return h1d;
}

TH1D * HistLoader::do_dr_projection(Float_t *ptrange, std::string hname)
{
    TH3D *htemp = (TH3D *) h3d->Clone();
    htemp->GetZaxis()->SetRangeUser(ptrange[0], ptrange[1]);

    TH1D *h1d = (TH1D *) htemp->Project3D("y");
	h1d->SetName(hname.c_str());
    h1d->Scale(1. / h1d->Integral("width"));

    std::string xtitle = Form("#Delta R_{%d}^{reco-gen}", sjtID);
    std::string ytitle = Form("1 / N_{2-prog jets} dN/d#Delta R_{%d}^{reco-gen}", sjtID);

    h1d->GetXaxis()->SetTitle(xtitle.c_str());
    h1d->GetYaxis()->SetTitle(ytitle.c_str());

    return h1d;
}
