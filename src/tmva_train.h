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
	std::string func_deliminator = ":=";

	std::string signal_tree_name = "signal_tree";
	std::string background_tree_name = "background_tree";

	std::string factory_name = "factory";
	std::string data_loader_name = "data_loader";

        void MakeTree(std::string, std::string, std::string, std::string);
public:
	//The names of the signal and background files the trees will be stored in
	std::string signal_file_name = "";
	std::string background_file_name = "";
	//File where the results of the tmva training will be stored
	std::string tmva_output_file_name = "";

	//The fullpath name of the .list file, listing files containing raw, unprocessed TTrees for signal/background
        std::string signal_file_list_name = "";
        std::string background_file_list_name = "";

	//The name of the TTree in each file of the signal filelist or background filelist to be retrieved
        std::string signal_training_tree_name = "";
        std::string background_training_tree_name = "";

	//A vector containing the expressions, in terms of branchnames, to train on
        std::vector<std::string> training_expressions;
	//A vector containing the names of branches needed from the raw data
        std::vector<std::string> training_branches;

	//Training options strings; public so it can be set by the user but given a default value commensurate with the TMVAClassification tutorial
	std::string factory_options = "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification";

	void MakeTrees();
	void Train();
	void MiscDebug();
};

#endif
