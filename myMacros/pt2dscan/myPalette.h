// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//
//    Color myPalette from Matt
//
// -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#include "TColor.h"
#include <vector>

// Define the colors to be ready for use 
TColor *myPal;

int mykMagenta = myPal->GetColor(124,   0, 124);
int mykViolet  = myPal->GetColor( 72,   0, 190);
int mykBlue    = myPal->GetColor(  9,   0, 200);
int mykAzure   = myPal->GetColor(  0,  48,  97);
int mykCyan    = myPal->GetColor(  0,  83,  98);
int mykTeal    = myPal->GetColor(  0,  92,  46);
int mykGreen   = myPal->GetColor( 15,  85,  15);
int mykSpring  = myPal->GetColor( 75,  97,  53);
int mykYellow  = myPal->GetColor(117, 118,   0);
int mykOrange  = myPal->GetColor(101,  42,   0);
int mykRed     = myPal->GetColor(190,   0,   3);
int mykPink    = myPal->GetColor(180,  35, 145);

int mykMagentaLight = myPal->GetColor(215, 165, 215);
int mykVioletLight  = myPal->GetColor(200, 160, 255);
int mykBlueLight    = myPal->GetColor(178, 185, 254);
int mykAzureLight   = myPal->GetColor(153, 195, 225);
int mykCyanLight    = myPal->GetColor(140, 209, 224);
int mykTealLight    = myPal->GetColor( 92, 217, 141);
int mykGreenLight   = myPal->GetColor(135, 222, 135);
int mykSpringLight  = myPal->GetColor(151, 207, 116);
int mykYellowLight  = myPal->GetColor(225, 225, 100);
int mykOrangeLight  = myPal->GetColor(255, 168, 104);
int mykRedLight     = myPal->GetColor(253, 169, 179);
int mykPinkLight    = myPal->GetColor(255, 192, 224);

// Define myPalette functions 

// Good for primary marker colors
int myPalette(int i)
{   
    std::vector<int> myPalette = {mykRed,  mykGreen,  mykBlue,  mykMagenta,  mykCyan,  mykSpring,  mykYellow,  mykAzure,  mykViolet,  mykTeal,  mykPink};
    return myPalette[i];
}

// Good for systematic band fill
int myPaletteLight(int i)
{
    std::vector<int> myPalette = {mykRedLight,  mykGreenLight,  mykBlueLight,  mykMagentaLight,  mykCyanLight,  mykSpringLight,  mykYellowLight,  mykAzureLight,  mykVioletLight,  mykTealLight,  mykPinkLight};
    return myPalette[i];
}
