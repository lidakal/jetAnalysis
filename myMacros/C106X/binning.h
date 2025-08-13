// jet pt bins : no underflow or overflow
const Int_t jtpt_binsVectorSize = 4;
Int_t jtpt_bins = jtpt_binsVectorSize - 1;
Double_t jtpt_binsVector[jtpt_binsVectorSize] = {
    80., 
    100., 
    120.,
    140.
};
Double_t jtpt_min = jtpt_binsVector[0];
Double_t jtpt_max = jtpt_binsVector[jtpt_bins];

// jet pt bins w/ underflow and overflow
const Int_t jtpt_wUO_binsVectorSize = 11;
Int_t jtpt_wUO_bins = jtpt_wUO_binsVectorSize - 1;
Double_t jtpt_wUO_binsVector[jtpt_wUO_binsVectorSize] = {
    5,
    50,
    60,
    70,
    80., 
    // 90,
    100., 
    // 110,
    120.,
    // 130,
    140.,
    150,
    200,
    5000,
};
Double_t jtpt_wUO_min = jtpt_wUO_binsVector[0];
Double_t jtpt_wUO_max = jtpt_wUO_binsVector[jtpt_wUO_bins];

// ln(0.4/rg) bins : 1st bin untagged (unphysical)
const Int_t logrg_binsVectorSize = 9;
Int_t logrg_bins = logrg_binsVectorSize - 1;
Double_t logrg_binsVector[logrg_binsVectorSize] = {
    -1.2, 
    0., 
    0.3,
    0.6,
    0.9,
    1.2,
    1.6,
    2.1,
    2.5
};
Double_t logrg_min = logrg_binsVector[0];
Double_t logrg_max = logrg_binsVector[logrg_bins];

// ln(0.4/rg) bins w/ underflow (rg>0.4 or logrg<0) : 1at bin untagged (unphysical), 2nd bin underflow
const Int_t logrg_wU_binsVectorSize = 10;
Int_t logrg_wU_bins = logrg_wU_binsVectorSize - 1;
Double_t logrg_wU_binsVector[logrg_wU_binsVectorSize] = {
    -2., // SD-untagged or kT<1 => use -1.1 as logrg_underflow value
    -1., // rg>0.4
    0., 
    // 0.15,
    0.3,
    // 0.45,
    0.6,
    // 0.75,
    0.9,
    // 1.05,
    1.2,
    // 1.4,
    1.6,
    // 1.8,
    2.1,
    2.5
};
Double_t logrg_wU_min = logrg_wU_binsVector[0];
Double_t logrg_wU_max = logrg_wU_binsVector[logrg_wU_bins];


// zg bins : 1st bin untagged (unphysical)
const Int_t zg_binsVectorSize = 6;
Int_t zg_bins = zg_binsVectorSize - 1;
Double_t zg_binsVector[zg_binsVectorSize] = {
    -0.1, 
    0.1, 
    // 0.15,
    0.2,
    // 0.25,
    0.3,
    0.4,
    0.5
};
Double_t zg_min = zg_binsVector[0];
Double_t zg_max = zg_binsVector[zg_bins];

// zpt bins (bpt/jtptCh)
const Int_t zpt_binsVectorSize = 7;
Int_t zpt_bins = zpt_binsVectorSize - 1;
Double_t zpt_binsVector[zpt_binsVectorSize] = {
    0., 
    0.35,
    0.55,
    0.7,
    0.8,
    0.9,
    1.
};
Double_t zpt_min = zpt_binsVector[0];
Double_t zpt_max = zpt_binsVector[zpt_bins];

// zpt bins (bpt/jtptCh) w/ underflow (no charged particles in the jet)
const Int_t zpt_wU_binsVectorSize = 8;
Int_t zpt_wU_bins = zpt_wU_binsVectorSize - 1;
Double_t zpt_wU_binsVector[zpt_wU_binsVectorSize] = {
    -0.1, // jtptCh==0
    0., 
    0.35,
    0.55,
    0.7,
    0.8,
    0.9,
    1.
};
Double_t zpt_wU_min = zpt_wU_binsVector[0];
Double_t zpt_wU_max = zpt_wU_binsVector[zpt_wU_bins];

bool skipMC(double jtpt, double refpt, double pthat) {
    if (!(refpt>0)) return true;    
    if (pthat<0.35*jtpt) return true;
    return false;
}