"""
Train a BDT using the xgboost package
on track-from-B MC data

@author: lidakal
"""

## Imports

import uproot
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
plt.rcParams.update({'font.size': 22})

import ROOT

from sklearn.model_selection import train_test_split
from xgboost import XGBClassifier

random_seed = 42
missing_value = -1000000.

if __name__ == "__main__":
    ## Load pre-treated data

    X_train = np.loadtxt('./data_csv/X_train.csv', delimiter=',')
    y_train = np.loadtxt('./data_csv/y_train.csv', delimiter=',')
    w_train = np.loadtxt('./data_csv/w_train.csv', delimiter=',')

    X_test = np.loadtxt('./data_csv/X_test.csv', delimiter=',')
    y_test = np.loadtxt('./data_csv/y_test.csv', delimiter=',')
    w_test = np.loadtxt('./data_csv/w_test.csv', delimiter=',') 

    X_val = np.loadtxt('./data_csv/X_val.csv', delimiter=',')
    y_val = np.loadtxt('./data_csv/y_val.csv', delimiter=',')
    w_val = np.loadtxt('./data_csv/w_val.csv', delimiter=',')   

    ## Train the model (BDT)

    bdt_params = {"objective": "binary:logistic", 
                  "missing": missing_value, 
                  "seed": random_seed, 
                  "max_depth": 3, 
                  "n_estimators": 500,
                  "learning_rate": 0.05,
                  "gamma": 0.25,
                  "reg_lambda": 0.,
                  "scale_pos_weight": 3,
                  "use_label_encoder": False}

    eval_metric = "aucpr"

    ## [DEBUG]: Simple fit
    # bdt = XGBClassifier(max_depth=3, n_estimators=500)
    # bdt.fit(X_train, y_train, sample_weight=w_train)

    bdt = XGBClassifier(**bdt_params)
    
    # Note: need to use ndarrays in the fit in order to be able to save it for TMVA
    print("Training the model with early stopping...")
    bdt.fit(X_train, 
            y_train,
            sample_weight=w_train,
            verbose=False,
            early_stopping_rounds=10,
            eval_metric=eval_metric,
            eval_set=[(X_train, y_train), (X_test, y_test)])

    ## Plot loss function

    results = bdt.evals_result()
    fig, ax = plt.subplots(figsize=(16, 9))
    ax.plot(results["validation_0"][eval_metric], label="Training {}".format(eval_metric))
    ax.plot(results["validation_1"][eval_metric], label="Validation {}".format(eval_metric))
    ax.axvline(bdt.best_ntree_limit, color="gray", label="Optimal tree number")
    ax.set_xlabel("Number of trees")
    ax.set_ylabel(eval_metric)
    ax.legend()
    fig.savefig("./plots/loss_function_bdt.png")



    bdt_params["n_estimators"] = bdt.best_ntree_limit
    print("Re-training the model with the parameters of the best iteration...")
    bdt.set_params(**bdt_params)
    bdt.fit(X_train, y_train, sample_weight=w_train, verbose=False,)

    ## Print the accuracy of the model

    y_pred = bdt.predict(X_val)
    acc_score = pd.DataFrame({"y_val": y_val, "y_pred": y_pred})

    n_pos = y_val.tolist().count(1)
    n_neg = y_val.tolist().count(0)

    sel_pos = acc_score["y_val"] == 1
    sel_neg = acc_score["y_val"] == 0
    true_pos = (acc_score.loc[sel_pos, "y_pred"] == 1).tolist()
    true_neg = (acc_score.loc[sel_neg, "y_pred"] == 0).tolist()

    n_true_pos = true_pos.count(True)
    n_false_neg = true_pos.count(False)

    n_true_neg = true_neg.count(True)
    n_false_pos = true_neg.count(False)

    print("True positive rate (efficiency): {:.2f}".format(n_true_pos / n_pos))
    print("True negative rate (background rejection): {:.2f}".format(n_true_neg / n_neg))
    print("Accuracy score: {:.2f}".format((n_true_pos + n_true_neg) / len(acc_score.index)))

    ## Save the trained model

    fout_name = "./saved_models/xgboost_bdt.txt"
    print("Saving the model as {}".format(fout_name))
    bdt.save_model(fout_name)

    ## Save for TMVA

    fout_name_root = "./saved_models/xgboost_bdt.root"
    print("Saving the model as {}".format(fout_name_root))
    ROOT.TMVA.Experimental.SaveXGBoost(bdt, "TrackFromBClassifier", fout_name_root, num_inputs = X_train.shape[1])
