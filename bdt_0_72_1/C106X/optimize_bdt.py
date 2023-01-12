import xgboost as xgb # Using version 0.72.1
import pandas as pd
from sklearn.model_selection import GridSearchCV
import yaml

## Constants
random_seed = 42
missing_value = -1000000.


if __name__ == "__main__":
    ## Load pre-treated data
    features = ["trkInSV", "svtxdls", "svtxm", "trkIp3dSig"]
    # indir = "./data_svmlight_50_pt_80"
    # dtrain = xgb.DMatrix(indir + "/dtrain.dat", feature_names=features)
    # dtest = xgb.DMatrix(indir + "/dtest.dat", feature_names=features)
    # dval = xgb.DMatrix(indir + "/dval.dat", feature_names=features)
    indir = "./data_csv_30_pt_700/"
    X_train = pd.read_csv(indir + "X_train.csv", names=features)
    y_train = pd.read_csv(indir + "y_train.csv", names=['class'], dtype=int)
    X_test = pd.read_csv(indir + "X_test.csv", names=features)
    y_test = pd.read_csv(indir + "y_test.csv", names=['class'], dtype=int)

    param_grid_1 = {
        "max_depth" : [2, 3, 4],
        "learning_rate" : [0.1, 0.05, 0.01], 
        "gamma" : [0., 0.25, 1.],
        "reg_lambda" : [0., 1., 10.], 
        "scale_pos_weight" : [0.5, 1., 2.]
    }

    # param_grid_2 = {
    #     "max_depth" : [3],
    #     "learning_rate" : [0.005, 0.01, 0.015], 
    #     "gamma" : [0.7, 1., 1.5],
    #     "reg_lambda" : [5., 10., 15.], 
    #     "scale_pos_weight" : [2.7]
    # }

    model = xgb.XGBClassifier(objective='binary:logistic', 
                          seed=random_seed,
                          subsample=0.01,
                          colsample_bytree=1.)
    grid_search = GridSearchCV(estimator=model,
                                    param_grid=param_grid_1,
                                    scoring='roc_auc',
                                    verbose=1,
                                    n_jobs=10,
                                    cv=3)
    grid_search.fit(X_train, y_train.values.ravel())
    optimal_params = grid_search.best_params_

    print(optimal_params)

    with open("./optimal_params1.yaml", 'w') as fout:
        yaml.dump(optimal_params, fout)

