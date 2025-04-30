// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//
//   CMS palette
//
// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#include "TColor.h"
#include <vector>

// Define the colors to be ready for use 
TColor *cmsPal;

int cmsBlue = cmsPal->GetColor("#3f90da");
int cmsGray = cmsPal->GetColor("#717581");
int cmsLightBlue = cmsPal->GetColor("#92dadd");
int cmsYellow = cmsPal->GetColor("#ffa90e");
int cmsOrange = cmsPal->GetColor("#e76300");
int cmsRed = cmsPal->GetColor("#bd1f01");
int cmsViolet = cmsPal->GetColor("#832db6");
int cmsLightOlive = cmsPal->GetColor("#b9ac70");
int cmsLightGray = cmsPal->GetColor("#94a4a2");
int cmsLightBrown = cmsPal->GetColor("#a96b59");

const std::vector<int> cms_palette = {
    cmsBlue,
    cmsYellow,
    cmsRed,
    cmsLightGray, 
    cmsViolet,
    cmsLightBrown,
    cmsOrange,
    cmsLightOlive,
    cmsLightGray,
    cmsLightBlue
};