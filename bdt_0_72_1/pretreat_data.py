import ROOT
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.datasets import dump_svmlight_file

random_seed = 42

def load_data(filename, variables):
    # Read data from ROOT files
    data_sig = ROOT.RDataFrame("TreeS", filename).AsNumpy()
    data_bkg = ROOT.RDataFrame("TreeB", filename).AsNumpy()
 
    # Convert inputs to format readable by machine learning tools
    x_sig = np.vstack([data_sig[var] for var in variables]).T
    x_bkg = np.vstack([data_bkg[var] for var in variables]).T
    x = np.vstack([x_sig, x_bkg])
 
    # Create labels
    num_sig = x_sig.shape[0]
    num_bkg = x_bkg.shape[0]
    y = np.hstack([np.ones(num_sig), np.zeros(num_bkg)])
 
    # Compute weights balancing both classes
    num_all = num_sig + num_bkg
    w = np.hstack([np.ones(num_sig) * num_all / num_sig, np.ones(num_bkg) * num_all / num_bkg])
 
    return x, y, w

if __name__ == "__main__":
    fname = "./ntuples/tmva_data_complex_bJetMC.root"
    variables = ["ipInSV", "ipSvtxdls", "ipSvtxm", "ip3dSig"]

    X, y, w = load_data(fname, variables)
    
    sample_size = 100000
    X, _, y, _, w, _ = train_test_split(X, y, w, train_size=sample_size, stratify=y, random_state=random_seed)

    X_train, X_test, y_train, y_test, w_train, w_test = train_test_split(X, y, w, test_size=0.5, stratify=y, random_state=random_seed)
    X_test, X_val, y_test, y_val, w_test, w_val = train_test_split(X_test, y_test, w_test, test_size=0.2, stratify=y_test, random_state=random_seed)

    np.savetxt('./data_csv/X_train.csv', X_train, delimiter=',')
    np.savetxt('./data_csv/y_train.csv', y_train, delimiter=',')
    np.savetxt('./data_csv/w_train.csv', w_train, delimiter=',')

    np.savetxt('./data_csv/X_test.csv', X_test, delimiter=',')
    np.savetxt('./data_csv/y_test.csv', y_test, delimiter=',')
    np.savetxt('./data_csv/w_test.csv', w_test, delimiter=',')

    np.savetxt('./data_csv/X_val.csv', X_val, delimiter=',')
    np.savetxt('./data_csv/y_val.csv', y_val, delimiter=',')
    np.savetxt('./data_csv/w_val.csv', w_val, delimiter=',')

    dump_svmlight_file(X_train, y_train, './data_svmlight/dtrain.dat', zero_based=True, multilabel=False)
    dump_svmlight_file(X_test, y_test, './data_svmlight/dtest.dat', zero_based=True, multilabel=False)
    dump_svmlight_file(X_val, y_val, './data_svmlight/dval.dat', zero_based=True, multilabel=False)
