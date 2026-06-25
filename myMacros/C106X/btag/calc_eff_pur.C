// void calc_eff_pur()
// {
//     // Calculate efficiency and purity from compact trees
//     std::string fin_btag_name = "/data_CMS/cms/kalipoliti/compact_trees/dijet_aggrTMVA_XXT_tree.root";
//     std::string fin_incl_name = "/data_CMS/cms/kalipoliti/compact_trees/dijet_aggrTMVA_inclusive_tree.root";

//     // std::string fin_btag_name = "/data_CMS/cms/kalipoliti/compact_trees/dijet_aggrTMVA_tree.root";
//     // std::string fin_incl_name = "/data_CMS/cms/kalipoliti/compact_trees/dijet_aggrTMVA_inclusive_tree.root";

//     std::string cut = "jtpt>=80&&jtpt<300&&weight<0.05";

//     // Efficiency = btag_b / incl_b
//     ROOT::RDataFrame incl_sig("tree_sig", fin_incl_name);
//     ROOT::RDataFrame btag_sig("tree_sig", fin_btag_name);

//     double incl_b = *(incl_sig.Filter(cut).Sum("weight"));
//     double btag_b = *(btag_sig.Filter(cut).Sum("weight"));

//     // Purity = btag_b / btag_total
//     ROOT::RDataFrame btag_all("tree_all", fin_btag_name);
//     double btag_total = *(btag_all.Filter(cut).Sum("weight"));

//     std::cout << "In " << cut << " : "
//               << " efficiency = " << btag_b / incl_b 
//               << ", purity = " << btag_b / btag_total
//               << std::endl;
// }

void calc_eff_pur()
{
    // Calculate efficiency and purity from compact trees
    std::string fin_incl_name = "/data_CMS/cms/kalipoliti/compact_trees/dijet_aggrTMVA_inclusive_tree.root";

    std::string cut_incl = "jtpt>=80&&jtpt<300&&weight<0.05";
    std::string cut_tag = "jtpt>=80&&jtpt<300&&weight<0.05&&(discr_b+discr_bb+discr_lepb)>0.55";

    // Efficiency = btag_b / incl_b
    ROOT::RDataFrame sig("tree_sig", fin_incl_name);

    double incl_b = *(sig.Filter(cut_incl).Sum("weight"));
    double btag_b = *(sig.Filter(cut_tag).Sum("weight"));

    // Light mistag = btag_l / incl_l
    ROOT::RDataFrame light("tree_bkg_l", fin_incl_name);

    double incl_l = *(light.Filter(cut_incl).Sum("weight"));
    double btag_l = *(light.Filter(cut_tag).Sum("weight"));

    // Purity = btag_b / btag_total
    // ROOT::RDataFrame total("tree_all", fin_incl_name);
    // double btag_total = *(total.Filter(cut_tag).Sum("weight"));

    std::cout << "In " << cut_tag << " : "
              << " efficiency = " << btag_b / incl_b 
            //   << ", purity = " << btag_b / btag_total
              << ", l mistag = " << btag_l / incl_l
              << std::endl;
}