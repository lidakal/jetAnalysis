void calc_eff_pur()
{
    // Calculate efficiency and purity from compact trees
    std::string fin_btag_name = "/data_CMS/cms/kalipoliti/compact_trees/dijet_aggrTMVA_withHLT_v2_tree_template_fit.root";
    std::string fin_incl_name = "/data_CMS/cms/kalipoliti/compact_trees/dijet_aggrTMVA_withHLT_v2_inclusive_tree_template_fit.root";

    std::string cut = "jtpt>80&&jtpt<140";

    // Efficiency = btag_b / incl_b
    ROOT::RDataFrame incl_sig("tree_sig", fin_incl_name);
    ROOT::RDataFrame btag_sig("tree_sig", fin_btag_name);

    double incl_b = *(incl_sig.Filter(cut).Sum("weight"));
    double btag_b = *(btag_sig.Filter(cut).Sum("weight"));

    // Purity = btag_b / btag_total
    ROOT::RDataFrame btag_all("tree_all", fin_btag_name);
    double btag_total = *(btag_all.Filter(cut).Sum("weight"));

    std::cout << "In " << cut << " : "
              << " efficiency = " << btag_b / incl_b 
              << ", purity = " << btag_b / btag_total
              << std::endl;
}