"""
Load a trained BDT using the xgboost package
on track-from-B MC data

@author: lidakal
"""

## Imports
import numpy as np
import xgboost as xgb # Using version 0.72.1

## Constants
random_seed = 42
missing_value = -1000000.

if __name__ == "__main__":
    ## Load the saved model 
    features = ["ipInSV", "ipSvtxdls", "ipSvtxm", "ip3dSig"]
    fin = "./saved_models/trained_bst.model"
    bst = xgb.Booster(model_file=fin)

    ## [DEBUG]: Read features from saved model
    print("Model features: {}".format(bst.feature_names))
    # Note: Feature names are not saved in the model but they seem to keep their order

    ## Make predictions
    xpred = np.array([[1., 2., 3., 5.],
                      [0., missing_value, missing_value, 1.],
                      [0., missing_value, missing_value, 10.]])
    dpred = xgb.DMatrix(xpred, feature_names=features)
    ypred = bst.predict(dpred)
    for i in range(len(ypred)):
        print("Booster prediction for {} is {}".format(xpred[i], ypred[i]))
    # Note: Same prediction as when trained, hurray!
    