## Imports
import numpy as np
import xgboost as xgb # Using version 0.72.1
from sklearn.metrics import roc_curve, auc
import matplotlib.pyplot as plt

## Constants
random_seed = 42
missing_value = -1000000.

def plot_roc_bdt(savefig=False):
    suffix = "_50_pt_80"
    ## Load pre-treated data
    features = ["ipInSV", "ipSvtxdls", "ipSvtxm", "ip3dSig"]
    indir = "./data_svmlight" + suffix + "/"
    dval = xgb.DMatrix(indir + "dval.dat", feature_names=features)
    
    ## Load the saved model 
    fin = "./saved_models/trained_bst" + suffix + ".model"
    bst = xgb.Booster(model_file=fin)

    ## Plot ROC curve
    yval = dval.get_label()
    ypred = bst.predict(dval)
    fpr, tpr, threshold = roc_curve(yval, ypred)
    auc_xgboost = auc(tpr, 1-fpr)

    ## Find working point
    eff_threshold = 0.96
    iwp = 0
    diff = 1
    for i in range(len(threshold)):
        eff = tpr[i]
        tmp = abs(eff - eff_threshold)
        if tmp < diff:
            diff = tmp
            iwp = i
    print("Working point with threshold {} results in eff {} and bkg rej {}".format(threshold[iwp], \
                                                                            tpr[iwp], 1 - fpr[iwp]))

    fig, ax = plt.subplots(figsize=(16, 9))
    ax.plot(tpr, 1-fpr, label="XGBoost predictions, AUC = {:.2f}".format(auc_xgboost))
    ax.set_xlabel("TPR (Efficiency)")
    ax.set_ylabel("1-FPR (Bkg rejection rate)")
    ax.axvline(tpr[iwp], color="black", linestyle="dashed", label="Threshold {}, eff {}, bkg rej {}".format(threshold[iwp], \
                                                                            tpr[iwp], 1 - fpr[iwp]))
    ax.legend()
    if savefig:
        fig.savefig("./plots/roc_bdt" + suffix + ".png")

    return tpr, 1-fpr
    

if __name__ == "__main__":
    eff, bkg_rej = plot_roc_bdt(savefig=True)