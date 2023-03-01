#include <Math/Vector4D.h>
#include <Math/Vector4Dfwd.h>
#include <Math/VectorUtil.h>

Float_t calc_dr(Float_t eta1, Float_t phi1, Float_t eta2, Float_t phi2) {
    ROOT::Math::PtEtaPhiMVector v1;
    v1.SetEta(eta1);
    v1.SetPhi(phi1);

    ROOT::Math::PtEtaPhiMVector v2;
    v2.SetEta(eta2);
    v2.SetPhi(phi2);

    Float_t dr = ROOT::Math::VectorUtil::DeltaR(v1, v2);
    return dr;
}

Float_t calc_rg(Float_t y1, Float_t phi1, Float_t y2, Float_t phi2) {
    ROOT::Math::PtEtaPhiMVector v1;
    v1.SetPhi(phi1);

    ROOT::Math::PtEtaPhiMVector v2;
    v2.SetPhi(phi2);

    Float_t dphi = ROOT::Math::VectorUtil::DeltaPhi(v1, v2);
    Float_t dy = y1 - y2;

    // std::cout << "dphi = " << dphi << std::endl;
    Float_t rg = std::sqrt((dy*dy) + (dphi*dphi));
    return rg;
}

