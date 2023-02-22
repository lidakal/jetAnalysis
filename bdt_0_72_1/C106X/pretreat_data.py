import uproot
import pandas as pd
import numpy as np
import os
import yaml

from sklearn.model_selection import train_test_split
from sklearn.datasets import dump_svmlight_file

random_seed = 42

def load_data(filename, variables):
    ## Read data from ROOT files
    data_sig = pd.DataFrame()
    data_bkg = pd.DataFrame()
    # data_pu = pd.DataFrame()

    with uproot.open(filename) as fin:
        for var in variables:
            branchS = fin[f"TreeS/{var}"].array(library="pd")
            data_sig[var] = branchS

            branchB = fin[f"TreeB/{var}"].array(library="pd")
            data_bkg[var] = branchB

            # branchPU = fin[f"TreePU/{var}"].array(library="pd")
            # data_pu[var] = branchPU
 
    ## Drop tracks with NaN values 
    data_sig = data_sig.dropna().reset_index(drop=True)
    data_bkg = data_bkg.dropna().reset_index(drop=True)
    # data_pu = data_pu.dropna().reset_index(drop=True)

    ## Convert inputs to format readable by machine learning tools
    x_sig = data_sig.to_numpy()
    x_bkg = data_bkg.to_numpy()
    # x_pu = data_pu.to_numpy()
    x = np.vstack([x_sig, x_bkg])
    # x = np.vstack([x_sig, x_bkg, x_pu])

    ## Create labels
    num_sig = x_sig.shape[0]
    num_bkg = x_bkg.shape[0]
    # num_pu = x_pu.shape[0]
    print("sum(negative)/sum(positive) = ", num_bkg / num_sig)
    y = np.hstack([np.ones(num_sig), np.zeros(num_bkg)])
    # y = np.hstack([np.ones(num_sig), np.zeros(num_bkg), 2 * np.ones(num_pu)])
 
    ## Compute weights balancing both classes
    num_all = num_sig + num_bkg# + num_pu
    w = np.hstack([np.ones(num_sig) * num_all / num_sig, np.ones(num_bkg) * num_all / num_bkg])
    # w = np.hstack([np.ones(num_sig) * num_all / num_sig, np.ones(num_bkg) * num_all / num_bkg, np.ones(num_pu) * num_all / num_pu])
 
    return x, y, w

if __name__ == "__main__":
    ## Load data
    indir = "./ntuples/"
    label = "ttbar_highPU"
    suffix = "_30_pt_700"
    fin = indir + label + suffix + ".root"

    conf = {}
    with open("./conf.yaml", "r") as yaml_in:
        conf = yaml.safe_load(yaml_in)
    features = conf['features']

    X, y, w = load_data(fin, features)
    print(f"Loaded {X.shape[0]} entries")

    sample_size = np.min([1000000, X.shape[0]-1])
    discard_size = np.max([X.shape[0] - sample_size, 3])
    X, _, y, _, w, _ = train_test_split(X, y, w, test_size=discard_size, stratify=y, random_state=random_seed)
    
    X_train, X_test, y_train, y_test, w_train, w_test = train_test_split(X, y, w, test_size=0.5, stratify=y, random_state=random_seed)
    X_test, X_val, y_test, y_val, w_test, w_val = train_test_split(X_test, y_test, w_test, test_size=0.5, stratify=y_test, random_state=random_seed)

    ## Save as csv
    odir = './data_csv_' + label + suffix
    if not os.path.exists(odir):
        print('Creating directory', odir)
        os.makedirs(odir)

    np.savetxt(odir + '/X_train.csv', X_train, delimiter=',')
    np.savetxt(odir + '/y_train.csv', y_train, delimiter=',')
    np.savetxt(odir + '/w_train.csv', w_train, delimiter=',')

    np.savetxt(odir + '/X_test.csv', X_test, delimiter=',')
    np.savetxt(odir + '/y_test.csv', y_test, delimiter=',')
    np.savetxt(odir + '/w_test.csv', w_test, delimiter=',')

    np.savetxt(odir + '/X_val.csv', X_val, delimiter=',')
    np.savetxt(odir + '/y_val.csv', y_val, delimiter=',')
    np.savetxt(odir + '/w_val.csv', w_val, delimiter=',')

    ## Save as DMatrices
    odir_svmlight = './data_svmlight_' + label + suffix
    if not os.path.exists(odir_svmlight):
        print('Creating directory', odir_svmlight)
        os.makedirs(odir_svmlight)

    dump_svmlight_file(X_train, y_train, odir_svmlight + '/dtrain.dat', zero_based=True, multilabel=False)
    dump_svmlight_file(X_test, y_test, odir_svmlight + '/dtest.dat', zero_based=True, multilabel=False)
    dump_svmlight_file(X_val, y_val, odir_svmlight + '/dval.dat', zero_based=True, multilabel=False)

    ## Save as TTress
    df_train = pd.DataFrame(X_train, columns=features, dtype=float)
    df_train["class"] = y_train
    df_train["trkIsLepton"] = df_train["trkIsLepton"].astype(int)
    df_train["trkInSV"] = df_train["trkInSV"].astype(int)
    df_train["class"] = df_train["class"].astype(int)

    df_test = pd.DataFrame(X_test, columns=features)
    df_test["class"] = y_test
    df_test["trkIsLepton"] = df_test["trkIsLepton"].astype(int)
    df_test["trkInSV"] = df_test["trkInSV"].astype(int)
    df_test["class"] = df_test["class"].astype(int)

    df_val = pd.DataFrame(X_val, columns=features)
    df_val["class"] = y_val
    df_val["trkIsLepton"] = df_val["trkIsLepton"].astype(int)
    df_val["trkInSV"] = df_val["trkInSV"].astype(int)
    df_val["class"] = df_val["class"].astype(int)

    odir_root = './data_root_' + label + suffix
    if not os.path.exists(odir_root):
        print('Creating directory', odir_root)
        os.makedirs(odir_root)
    froot = uproot.recreate(odir_root + "/data.root")
    froot["train_sig"] = df_train[df_train["class"] == 1]
    froot["train_bkg"] = df_train[df_train["class"] == 0]
    froot["test_sig"] = df_test[df_test["class"] == 1]
    froot["test_bkg"] = df_test[df_test["class"] == 0]
    froot["val_sig"] = df_val[df_val["class"] == 1]
    froot["val_bkg"] = df_val[df_val["class"] == 0]
    froot["val"] = df_val
