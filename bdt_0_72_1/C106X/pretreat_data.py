import uproot
import pandas as pd
import numpy as np
import os

from sklearn.model_selection import train_test_split
from sklearn.datasets import dump_svmlight_file

random_seed = 42

def load_data(filename, variables):
    ## Read data from ROOT files
    data_sig = pd.DataFrame()
    data_bkg = pd.DataFrame()

    with uproot.open(filename) as fin:
        for var in variables:
            branchS = fin[f"TreeS/{var}"].array(library="pd")
            data_sig[var] = branchS

            branchB = fin[f"TreeB/{var}"].array(library="pd")
            data_bkg[var] = branchB
 
    ## Drop tracks with NaN values 
    data_sig = data_sig.dropna().reset_index(drop=True)
    data_bkg = data_bkg.dropna().reset_index(drop=True)

    ## Convert inputs to format readable by machine learning tools
    x_sig = data_sig.to_numpy()
    x_bkg = data_bkg.to_numpy()
    x = np.vstack([x_sig, x_bkg])

    ## Create labels
    num_sig = x_sig.shape[0]
    num_bkg = x_bkg.shape[0]
    print("sum(negative)/sum(positive) = ", num_bkg / num_sig)
    y = np.hstack([np.ones(num_sig), np.zeros(num_bkg)])
 
    ## Compute weights balancing both classes
    num_all = num_sig + num_bkg
    w = np.hstack([np.ones(num_sig) * num_all / num_sig, np.ones(num_bkg) * num_all / num_bkg])
 
    return x, y, w

if __name__ == "__main__":
    ## Load data
    indir = "./ntuples/"
    label = "HiForestMiniAOD_HighPU_100000events_conmatch"
    suffix = "_50_pt_80"
    fin = indir + label + suffix + ".root"

    variables = ["trkInSV", "svtxdls", "svtxm", "trkIp3dSig"]

    X, y, w = load_data(fin, variables)
    print(f"Loaded {X.shape[0]} entries")

    sample_size = np.min([1000000, X.shape[0]-1])
    discard_size = np.max([X.shape[0] - sample_size, 2])
    X, _, y, _, w, _ = train_test_split(X, y, w, test_size=discard_size, stratify=y, random_state=random_seed)
    
    X_train, X_test, y_train, y_test, w_train, w_test = train_test_split(X, y, w, test_size=0.5, stratify=y, random_state=random_seed)
    X_test, X_val, y_test, y_val, w_test, w_val = train_test_split(X_test, y_test, w_test, test_size=0.2, stratify=y_test, random_state=random_seed)

    odir = './data_csv' + suffix
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

    odir_svmlight = './data_svmlight' + suffix
    if not os.path.exists(odir_svmlight):
        print('Creating directory', odir_svmlight)
        os.makedirs(odir_svmlight)

    dump_svmlight_file(X_train, y_train, odir_svmlight + '/dtrain.dat', zero_based=True, multilabel=False)
    dump_svmlight_file(X_test, y_test, odir_svmlight + '/dtest.dat', zero_based=True, multilabel=False)
    dump_svmlight_file(X_val, y_val, odir_svmlight + '/dval.dat', zero_based=True, multilabel=False)
