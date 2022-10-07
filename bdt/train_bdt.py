import uproot3
import pandas as pd
import numpy as np

from sklearn.model_selection import train_test_split

fin_name = "tmva_data_complex_bJetMC.root"
sig_tree_name = "TreeS"
bkg_tree_name = "TreeB"

X = np.array()
y = np.array()

with uproot3.open(fin_name) as fin:
    _sig_tree = fin[sig_tree_name]
    _sig_data = _sig_tree.pandas.df()

    _bkg_tree = fin[bkg_tree_name]
    _bkg_data = _bkg_tree.pandas.df()

    X = pd.concat([_sig_data, _bkg_data])

    print(X)



