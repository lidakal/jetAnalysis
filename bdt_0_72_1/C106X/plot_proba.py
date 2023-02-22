"""
Load a trained BDT using the xgboost package
on track-from-B MC data and plot the predicted
probabilities

@author: lidakal
"""

## Imports
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import xgboost as xgb

import matplotlib
matplotlib.rcParams.update({'font.size': 22})

if __name__ == "__main__":
    ## Load pre-treated data
    features = ["ipInSV", "ipSvtxdls", "ipSvtxm", "ip3dSig"]
    xcsv = pd.read_csv("./data_csv_50_pt_80/X_val.csv", names=features, dtype=float)
    ycsv = pd.read_csv("./data_csv_50_pt_80/y_val.csv", names=["label"], dtype=int)
    data = pd.concat([xcsv, ycsv], axis=1)

    data_tp = data[data["label"] == 1]
    data_tn = data[data["label"] == 0]

    dval_tp = xgb.DMatrix(data_tp[features].values, feature_names=features)
    dval_tn = xgb.DMatrix(data_tn[features].values, feature_names=features)

    ## Load the saved model 
    fin = "./saved_models/trained_bst_50_pt_80.model"
    bst = xgb.Booster(model_file=fin)

    ## Make predictions
    ypred_tp = bst.predict(dval_tp)
    ypred_tn = bst.predict(dval_tn)

    fig, ax = plt.subplots(figsize=(16, 9))
    binning = np.linspace(0., 1., 25)
    ax.hist(ypred_tp, binning, density=True, histtype="step", label="True positive")
    ax.hist(ypred_tn, binning, density=True, histtype="step", label="True negative")
    ax.set_xlabel("(signal) prob")
    ax.set_ylabel("1/N dN/dprob")
    ax.legend(loc="upper left")

    fig.savefig("./plots/proba_distribution.png")

