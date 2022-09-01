#ifndef TMVA_TRAINER_H
#define TMVA_TRAINER_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooArgList.h"

class TMVA_Trainer
{
protected:
        TTree* signal_tree = 0x0;
        TTree* background_tree = 0x0;

public:
        std::string signal_list_file_name = ""; //The fullpath name of the .list file, listing files containing raw data for signal
        std::string background_list_file_name = ""; //The fullpath name of the .list file, listing files containing raw data for background
        std::string training_tree_name = ""; //The fullpath name of the TTree in each file of the filelist

        std::vector<std::string> training_branches; //A vector containing the names of branches needed from the raw data
        std::vector<std::string> training_expressions; //A vector containing the expressions, in terms of branchnames, to train on

        void MakeTrees();
};

#endif
