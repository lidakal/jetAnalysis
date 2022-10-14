import ROOT
import numpy as np
from xgboost import XGBClassifier
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 22})

from sklearn.metrics import roc_curve, auc

if __name__ == "__main__":

    ## Reminder that variables are in this order: 
    # variables = ["ipInSV", "ipSvtxdls", "ipSvtxm", "ip3dSig"]

    X_val = np.loadtxt('./data_csv/X_val.csv', delimiter=',')
    y_val = np.loadtxt('./data_csv/y_val.csv', delimiter=',')
    w_val = np.loadtxt('./data_csv/w_val.csv', delimiter=',')

    ## [DEBUG]: TMVA struggles with the missing values
    # X_val = np.array([[0., -1000000., -1000000., 3.]])
    # X_val = np.array([[1, 10., 5., 4.]])
    
    fname = "./saved_models/xgboost_bdt.txt"
    print("Loading classifier from", fname)
    bdt = XGBClassifier()
    bdt.load_model(fname)

    print("Using data:\n", X_val[-5:])
    
    y_pred = bdt.predict_proba(X_val)
    print("xgboost native prediction(s):\n", y_pred[-5:])
    
    fname_tmva = "./saved_models/xgboost_bdt.root"
    bdt_name_tmva = "TrackFromBClassifier"
    print("Loading classifier", bdt_name_tmva, "from", fname_tmva)
    bdt_tmva = ROOT.TMVA.Experimental.RBDT[""](bdt_name_tmva, fname_tmva)
 
    ## Make prediction
    y_pred_tmva = np.zeros(shape=(X_val.shape[0], 2))
    for i in range(X_val.shape[0]):
        xi = X_val[i].tolist()
        yi = bdt_tmva.Compute(xi)[0]
        yi = np.array([1 - yi, yi])
        y_pred_tmva[i] = yi
        
    print("xgboost TMVA prediction(s):\n", y_pred_tmva[-5:])

    ## Plot ROC curve

    fpr, tpr, threshold = roc_curve(y_val, y_pred.T[1], sample_weight=w_val)
    auc_xgboost = auc(tpr, 1-fpr)

    fpr_tmva, tpr_tmva, threshold_tmva = roc_curve(y_val, y_pred_tmva.T[1], sample_weight=w_val)
    auc_tmva = auc(tpr_tmva, 1-fpr_tmva)

    fig, ax = plt.subplots(figsize=(16, 9))
    ax.plot(tpr, 1-fpr, label="XGBoost native predictions, AUC = {:.2f}".format(auc_xgboost))
    ax.plot(tpr_tmva, 1-fpr_tmva, label="TMVA interfaced predictions, AUC = {:.2f}".format(auc_tmva))
    ax.set_xlabel("TPR (Efficiency)")
    ax.set_ylabel("1-FPR (Bkg rejection rate)")
    ax.legend()
    fig.savefig("./plots/roc_bdt.png")

