#include "TH2.h"
#include "TString.h"

void format_response(TH2F *h, 
                     TString var="ln(R/R_{g})")
{
    TString xtitle = "Detector level " + var;
    TString ytitle = "Particle level " + var;

    h->GetXaxis()->SetTitle(xtitle);
    h->GetYaxis()->SetTitle(ytitle);
    h->GetYaxis()->SetTitleOffset(1.5);
}

// From newer ROOT version
bool SetRealAspectRatio(TCanvas *c, const Int_t axis=1)
{
   c->Update();
 
   //Get how many pixels are occupied by the canvas
   Int_t npx = c->GetWw();
   Int_t npy = c->GetWh();
 
   //Get x-y coordinates at the edges of the canvas (extrapolating outside the axes, NOT at the edges of the histogram)
   Double_t x1 = c->GetX1();
   Double_t y1 = c->GetY1();
   Double_t x2 = c->GetX2();
   Double_t y2 = c->GetY2();
 
   //Get the length of extrapolated x and y axes
   Double_t xlength2 = x2 - x1;
   Double_t ylength2 = y2 - y1;
   Double_t ratio2   = xlength2/ylength2;
 
   //Now get the number of pixels including the canvas borders
   Int_t bnpx = c->GetWindowWidth();
   Int_t bnpy = c->GetWindowHeight();
 
   if (axis==1) {
      c->SetCanvasSize(TMath::Nint(npy*ratio2), npy);
      c->SetWindowSize((bnpx-npx)+TMath::Nint(npy*ratio2), bnpy);
   } else if (axis==2) {
      c->SetCanvasSize(npx, TMath::Nint(npx/ratio2));
      c->SetWindowSize(bnpx, (bnpy-npy)+TMath::Nint(npx/ratio2));
   } else {
      Error("SetRealAspectRatio", "axis value %d is neither 1 (resize along x-axis) nor 2 (resize along y-axis).",axis);
      return false;
   }
 
   //Check now that resizing has worked
 
   c->Update();
 
   //Get how many pixels are occupied by the canvas
   npx = c->GetWw();
   npy = c->GetWh();
 
   //Get x-y coordinates at the edges of the canvas (extrapolating outside the axes,
   //NOT at the edges of the histogram)
   x1 = c->GetX1();
   y1 = c->GetY1();
   x2 = c->GetX2();
   y2 = c->GetY2();
 
   //Get the length of extrapolated x and y axes
   xlength2 = x2 - x1;
   ylength2 = y2 - y1;
   ratio2 = xlength2/ylength2;
 
   //Check accuracy +/-1 pixel due to rounding
   if (abs(TMath::Nint(npy*ratio2) - npx)<2) {
      return true;
   } else {
      Error("SetRealAspectRatio", "Resizing failed.");
      return false;
   }
}
 