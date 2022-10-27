"""
Load a trained BDT using the xgboost package
on track-from-B MC data

@author: lidakal
"""

## Imports
import ROOT
import numpy as np
import xgboost as xgb # Using version 0.72.1

if __name__ == "__main__":
    ## Load the saved model 

    # Directly into a classifier
    # model_name = "./saved_models/xgboost_bdt.txt"
    # bdt = XGBClassifier()
    # bdt.load_model(model_name)

    # Into a booster
    model_name_cmssw = "./saved_models/xgboost_bdt_cmsssw.model"
    bst = xgb.Booster(model_file=model_name_cmssw)

    # With pyROOT/TMVA
    model_name_tmva = "./saved_models/xgboost_bdt.root"
    bdt_tmva = ROOT.TMVA.Experimental.RBDT[""]("TrackFromBClassifier", model_name_tmva)

    ## Make a prediction

    # reminder that the BDT was trained on: 
    # discr_variables = ["ipInSV", "ipSvtxdls", "ipSvtxm", "ip3dSig"]

    missing_value = -1000000.

    X_pred = np.array([[1, 2., 3., 10.], 
                       [0, missing_value, missing_value, 6.],
                       [0, missing_value, missing_value, 1.]]) # needs to be a 2D array

    d_pred = xgb.DMatrix(X_pred)

    # y_pred = bdt.predict(X_pred)
    y_pred_cmssw = bst.predict(d_pred)

    # print("\nXGBClassifier: Predicted class(es) for: \n{} \nare: \n{}".format(X_pred, y_pred))
    print("\nXGBooster: Predicted class(es) for: \n{} \nare: \n{}".format(X_pred, y_pred_cmssw))

    ## We can also get the probabilities for each class

    # y_proba = bdt.predict_proba(X_pred)

    # print("\nPredicted probabilities for: \n{} \nare: \n{}".format(X_pred, y_proba))

    # ## Do the same with pyROOT and see if the predicted probability is the same 

    
    # y_proba_tmva = np.zeros(shape=(X_pred.shape[0], 2))
    # for i in range(X_pred.shape[0]):
    #     xi = X_pred[i].tolist()
    #     yi = bdt_tmva.Compute(xi)[0]
    #     yi = np.array([1 - yi, yi])
    #     y_proba_tmva[i] = yi

    # print("\nPredicted probabilities by TMVA for: \n{} \nare: \n{}".format(X_pred, y_proba_tmva))