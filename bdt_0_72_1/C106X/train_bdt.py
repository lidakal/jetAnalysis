"""
Train a BDT using the xgboost package
on track-from-B MC data

@author: lidakal
"""

## Imports
import numpy as np
import matplotlib.pyplot as plt
import xgboost as xgb # Using version 0.72.1

## Constants
random_seed = 42
missing_value = -1000000.

if __name__ == "__main__":
    ## Load pre-treated data
    suffix = "_50_pt_80"
    indir = "./data_svmlight" + suffix
    features = ["trkInSV", "svtxdls", "svtxm", "trkIp3dSig"]
    dtrain = xgb.DMatrix(indir + "/dtrain.dat", feature_names=features)
    dtest = xgb.DMatrix(indir + "/dtest.dat", feature_names=features)
    dval = xgb.DMatrix(indir + "/dval.dat", feature_names=features)

    ## Train the model (BDT)
    params = {"objective": "binary:logistic", 
              "missing": missing_value, 
              "seed": random_seed, 
              "max_depth": 2, 
              "n_estimators": 500,
              "learning_rate": 0.1,
              "gamma": 0.,
              "reg_lambda": 1.,
              "scale_pos_weight": 0.5,
              "use_label_encoder": False
              }

    eval_metric = "error"
    evals_result = {}
    watchlist = [(dtrain, 'train'), (dtest, 'eval')]
    epochs = 500
    bst = xgb.train(params, dtrain, epochs, watchlist, early_stopping_rounds=20, evals_result=evals_result)
    # Note: the feature names are successfully passed to bst

    ## Get best iteration
    best_iteration = bst.best_iteration
    best_ntree_limit = bst.best_ntree_limit
    print("\nBest iteration: {}, best ntree limit: {}". format(best_iteration, best_ntree_limit))

    ## Plot evaluation metric
    fig, ax = plt.subplots(figsize=(16, 9))
    ax.plot(evals_result["train"][eval_metric], label="Training {}".format(eval_metric))
    ax.plot(evals_result["eval"][eval_metric], label="Validation {}".format(eval_metric))
    ax.axvline(best_iteration, color="gray", label="Best iteration")
    ax.set_xlabel("Iterations")
    ax.set_ylabel(eval_metric)
    ax.legend()
    fig.savefig("./plots/loss_function_bdt.png")

    ## Grab the best iteration 
    print("\nRe-training the model with the parameters of the best iteration...")
    bst = xgb.train(params, dtrain, best_iteration)

    ## Print the accuracy of the model
    preds = bst.predict(dval) # preds is a numpy array with the probability for the positive class
    labels = dval.get_label()

    print('Accuracy score at 0.5: %f' % (1-(sum(1 for i in range(len(preds)) if int(preds[i] > 0.5) != labels[i]) / float(len(preds)))))

    ## Make predictions
    xpred = np.array([[1., 2., 3., 5.],
                      [0., missing_value, missing_value, 1.],
                      [0., missing_value, missing_value, 10.]])
    dpred = xgb.DMatrix(xpred, feature_names=features)
    ypred = bst.predict(dpred)
    for i in range(len(ypred)):
        print("Booster prediction for {} is {}".format(xpred[i], ypred[i]))

    ## Save the trained model
    fout = "./saved_models/trained_bst" + suffix + ".model"
    print("Saving the model as {}".format(fout))
    bst.save_model(fout)
    # Note: the feature names are not saved with the model