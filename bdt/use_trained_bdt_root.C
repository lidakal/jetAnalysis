/*
Load into ROOT a classifier trained using the XGBoost Python API
on track-from-B MC data
using TMVA

Author: Lida Kalipoliti
*/

#include <vector>

using namespace std;

void use_trained_bdt_root()
{
    // Initialize TMVA -- Necessary in Jupyter

    TMVA::Tools::Instance();

    // Load the trained model

    std::string finName = "./saved_models/xgboost_bdt.root";
    TMVA::Experimental::RBDT<> bdt("TrackFromBClassifier", finName);

    // Get the probability for the positive class

    float missing_value = -1000000.;

    vector<vector<float>> X_pred = {{1, 2., 3., 5.}, 
                                  {0, missing_value, missing_value, 6.},
                                  {0, missing_value, missing_value, 1.}};

    for (vector<float> x : X_pred) {
        vector<float> y = bdt.Compute(x);
        cout << y[0] << endl;
    }  
    
}


