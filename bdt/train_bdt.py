"""
Train a BDT using the xgboost package
on track-from-B MC data

@author: lidakal
"""

## Imports

import uproot3
import pandas as pd
import numpy as np

from sklearn.model_selection import train_test_split
from xgboost import XGBClassifier

## Load Data

fin_name = "tmva_data_complex_bJetMC.root"
sig_tree_name = "TreeS"
bkg_tree_name = "TreeB"

data = pd.DataFrame()
missing_value = -1000000.

with uproot3.open(fin_name) as fin:
    _sig_tree = fin[sig_tree_name]
    _sig_data = _sig_tree.pandas.df()
    ## [DEBUG]: getting a smaller dataset 
    _sig_data = _sig_data.sample(n = 10000)
    _sig_y = np.ones(len(_sig_data.index), dtype = int)

    _bkg_tree = fin[bkg_tree_name]
    _bkg_data = _bkg_tree.pandas.df()
    ## [DEBUG]: getting a smaller dataset 
    _bkg_data = _bkg_data.sample(n = 10000)
    _bkg_y = np.zeros(len(_bkg_data.index), dtype = int)

    data = pd.concat([_sig_data, _bkg_data])
    data["class"] = np.concatenate([_sig_y, _bkg_y])

## Split data into training, testing, validation
 
data_train, data_test = train_test_split(data, test_size = 0.5, stratify = data["class"])
data_test, data_val = train_test_split(data_test, test_size = 0.2, stratify = data_test["class"])

## Grab the relevant columns

discr_variables = ["ipInSV", "ipSvtxdls", "ipSvtxm", "ip3dSig"]
class_label = "class"

X_train = data_train[discr_variables]
X_test = data_test[discr_variables]
X_val = data_test[discr_variables]

y_train = data_train[class_label]
y_test = data_test[class_label]
y_val = data_val[class_label]

## Train the BDT

bdt = XGBClassifier(objective = "binary:logistic", 
                    missing = missing_value, 
                    seed = 42, 
                    max_depth = 3, 
                    n_estimators = 500,
                    learning_rate = 0.05,
                    gamma = 0.25,
                    reg_lambda = 0.,
                    scale_pos_weight = 3)
    
bdt.fit(X_train, 
        y_train,
        verbose = False,
        early_stopping_rounds = 10,
        eval_metric = "aucpr",
        eval_set = [(X_test, y_test)])

## Print the accuracy of the BDT

y_pred = bdt.predict(y_val)
y_mispred = y_pred - y_val

print(y_mispred)



