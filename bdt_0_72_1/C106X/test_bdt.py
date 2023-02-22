## Imports
import numpy as np
import pandas as pd
import xgboost as xgb # Using version 0.72.1
from sklearn.metrics import roc_curve, auc
import matplotlib.pyplot as plt
import matplotlib as mpl
import yaml

mpl.rcParams.update({'font.size': 22})

## Constants
random_seed = 42
missing_value = -1000000.

def plot_roc_bdt(savefig=False):
    suffix = "_30_pt_700"
    ## Load pre-treated data
    conf = {}
    with open("./conf.yaml", "r") as fin:
        conf = yaml.safe_load(fin)
    features = conf['features']
    # indir = "./data_svmlight" + suffix + "/"
    # dval = xgb.DMatrix(indir + "dval.dat", feature_names=features)
    X_val = pd.read_csv("./data_csv_30_pt_700/X_val.csv", names=features)
    y_val = pd.read_csv("./data_csv_30_pt_700/y_val.csv", names=['class'])
    dval = xgb.DMatrix(X_val.values, y_val.values)
    
    ## Load the saved model 
    # fin = "./saved_models/trained_bst" + suffix + ".model"
    fin = "./saved_models/sig_vs_bkg.model"
    bst = xgb.Booster(model_file=fin)

    ## Plot ROC curve
    yval = dval.get_label()
    yprob = bst.predict(dval)
    threshold = 0.44
    df = pd.DataFrame({'yval': yval, 'yprob': yprob})
    df['ypred'] = (df['yprob'] > threshold).astype(int)
    df['yval'] = df['yval'].astype(int)

    sig = df[df['yval']==1].shape[0]
    sig_pass = df[((df['yval']==1)&(df['ypred']==1))].shape[0]
    print("efficiency at threshold=", threshold, ":", sig_pass/sig)

    bkg = df[df['yval']==0].shape[0]
    bkg_rej = df[((df['yval']==0)&(df['ypred']==0))].shape[0]
    print("bkg rej at threshold=", threshold, ":", bkg_rej/bkg)
    fpr, tpr, threshold = roc_curve(yval, yprob)
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
    print("Working point with threshold {:.2f} results in eff {:.2f} and bkg rej {:.2f}".format(threshold[iwp], \
                                                                            tpr[iwp], 1 - fpr[iwp]))

    ## [DEBUG]
    # Calculate mistag of bkg at WP = 0.26
    # total_pu = 0
    # mistag_pu = 0
    # for ytrue, yprob in zip(yval, ypred):
    #     ytrue = int(ytrue)
    #     if ytrue != 0: continue
    #     total_pu += 1
    #     if (yprob > 0.26): mistag_pu += 1 
    # print("mistag pu rate:", mistag_pu / total_pu)

    fig, ax = plt.subplots(figsize=(16, 9))
    ax.plot(tpr, 1-fpr, label="XGBoost predictions, AUC = {:.2f}".format(auc_xgboost))
    ax.set_xlabel("TPR (Signal efficiency)")
    ax.set_ylabel("1-FPR (Background rejection rate)")
    ax.axvline(tpr[iwp], color="black", linestyle="dashed", label="Threshold {:.2f}, eff {:.2f}, bkg rej {:.2f}".format(threshold[iwp], \
                                                                            tpr[iwp], 1 - fpr[iwp]))
    ax.legend()
    if savefig:
        fig.savefig("./plots/roc_bdt" + suffix + ".png")

    # tpr, fpr = (0, 0)
    return tpr, 1-fpr
    

if __name__ == "__main__":
    eff, bkg_rej = plot_roc_bdt(savefig=True)