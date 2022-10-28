## Imports
import numpy as np
import xgboost as xgb # Using version 0.72.1
from sklearn.metrics import roc_curve, auc
import matplotlib.pyplot as plt

## Constants
random_seed = 42
missing_value = -1000000.

if __name__ == "__main__":
    ## Load pre-treated data
    features = ["ipInSV", "ipSvtxdls", "ipSvtxm", "ip3dSig"]
    dtrain = xgb.DMatrix("./data_svmlight/dtrain.dat", feature_names=features)
    dtest = xgb.DMatrix("./data_svmlight/dtest.dat", feature_names=features)
    dval = xgb.DMatrix("./data_svmlight/dval.dat", feature_names=features)
    
    ## Load the saved model 
    fin = "./saved_models/trained_bst.model"
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
    ax.axvline(tpr[iwp], 1 - fpr[iwp], label="Threshold {}, eff {}, bkg rej {}".format(threshold[iwp], \
                                                                            tpr[iwp], 1 - fpr[iwp]))
    ax.legend()
    fig.savefig("./plots/roc_bdt.png")
    

