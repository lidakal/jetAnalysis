from ROOT import TColor

# good for primary marker colors
def palette(i):
    pal = TColor()
    
    kMagenta = pal.GetColor(124,  0,124)
    kViolet  = pal.GetColor( 72,  0,190)
    kBlue    = pal.GetColor(  9,  0,200)
    kAzure   = pal.GetColor(  0, 48, 97)
    kCyan    = pal.GetColor(  0, 83, 98)
    kTeal    = pal.GetColor(  0, 92, 46)
    kGreen   = pal.GetColor( 15, 85, 15)
    kSpring  = pal.GetColor( 75, 97, 53)
    kYellow  = pal.GetColor(117,118,  0)
    kOrange  = pal.GetColor(101, 42,  0)
    kRed     = pal.GetColor(190,  0,  3)
    kPink    = pal.GetColor(180, 35,145)
    
    palette = [kRed, kGreen, kBlue, kMagenta, kCyan, kSpring, kYellow, kAzure, kViolet, kTeal, kPink]
    return palette[i]

# good for systematic band fill
def paletteLight(i):
    pal = TColor()
                                                                                                              
    kMagentaLight = pal.GetColor(215,165,215)
    kVioletLight  = pal.GetColor(200,160,255)
    kBlueLight    = pal.GetColor(178,185,254)
    kAzureLight   = pal.GetColor(153,195,225)
    kCyanLight    = pal.GetColor(140,209,224)
    kTealLight    = pal.GetColor( 92,217,141)
    kGreenLight   = pal.GetColor(135,222,135)
    kSpringLight  = pal.GetColor(151,207,116)
    kYellowLight  = pal.GetColor(225,225,100)
    kOrangeLight  = pal.GetColor(255,168,104)
    kRedLight     = pal.GetColor(253,169,179)
    kPinkLight    = pal.GetColor(255,192,224)

    #TODO: Make palette to return
