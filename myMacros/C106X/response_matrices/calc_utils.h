#include <Math/Vector4D.h>
#include <Math/Vector4Dfwd.h>
#include <Math/VectorUtil.h>

Float_t calc_rg(Float_t eta1, Float_t phi1, Float_t pt1, Float_t eta2, Float_t phi2, Float_t pt2) {
    ROOT::Math::PtEtaPhiMVector v1;
    v1.SetPt(pt1);
    v1.SetEta(eta1);
    v1.SetPhi(phi1);

    ROOT::Math::PtEtaPhiMVector v2;
    v2.SetPt(pt2);
    v2.SetEta(eta2);
    v2.SetPhi(phi2);

    Float_t rg = ROOT::Math::VectorUtil::DeltaR(v1, v2);
    return rg;
}

