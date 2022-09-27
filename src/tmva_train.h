#ifndef TMVA_TRAINER_H
#define TMVA_TRAINER_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include "TTreeFormulaManager.h"
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
	//The names of the TTrees in the processed files
	std::string prompt_tree_name = "prompt_tree";
	std::string nonprompt_tree_name = "nonprompt_tree";
	std::string background_tree_name = "background_tree";

	std::string factory_name = "factory";
	std::string data_loader_name = "data_loader";

	//RooArgLists that go with the raw_..._vars initialized in Init();
	RooArgList raw_prompt_args;
	RooArgList raw_nonprompt_args;
	RooArgList raw_data_args;

	//The name of the mass branch to be used in the processed files
	std::string mass_name = "mass";

	//commensurate vectors that show what the names of the training vars should be and the formulas in the data files
	//mc_formulas are expressed in terms of mc_vars
	//data_formulas are expressed in terms of data_vars
	std::vector<std::string> training_vars = {};

	std::vector<std::string> training_prompt_formulas = {};
	std::vector<std::string> training_nonprompt_formulas = {};
	std::vector<std::string> training_data_formulas = {};

	RooArgList training_prompt_args;
	RooArgList training_nonprompt_args;
	RooArgList training_data_args;

	//RooArgLists that go with the cuts
	RooArgList prompt_cuts;
	RooArgList nonprompt_cuts;

public:
	//The names of the prompt, nonprompt, and background files the trees will be stored in
	std::string training_file_name = "";

	//The fullpath name of raw files used to read in data
	//Or, the fullpath name of the .list file if the raw file names are unspecified
	std::string prompt_file_name = "";
	std::string nonprompt_file_name = "";
	std::string data_file_name = "";

	std::string prompt_file_list_name = "";
	std::string nonprompt_file_list_name = "";
	std::string data_file_list_name = "";

	//The names of the TTrees in each file of the raw data files to be retrieved
        std::vector<std::string> prompt_tree_names = {};
        std::vector<std::string> nonprompt_tree_names = {};
        std::vector<std::string> data_tree_names = {};

	//The branch names needed from these TTrees to construct the mass, training, and cut RooFormulaVars
	std::vector<std::string> raw_prompt_vars = {};
	std::vector<std::string> raw_nonprompt_vars = {};
	std::vector<std::string> raw_data_vars = {};

	//Mass expressions
	std::string prompt_mass_formula = "";
	std::string nonprompt_mass_formula = "";
	std::string data_mass_formula = "";

	//Cut expressions
	std::vector<std::string> prompt_cut_formulas = {};
	std::vector<std::string> nonprompt_cut_formulas = {};

	//Training options
	std::string factory_options = "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification";

	void AddTrainingVar(std::string, std::string, std::string, std::string);
	void AddTrainingVar(std::string s, std::string t, std::string u){AddTrainingVar(s, t, t, u);}
	void AddTrainingVar(std::string s, std::string t){AddTrainingVar(s, t, t, t);}


	void InitFiles();
	void InitArgs();

	void ReadPrompt();
	void ReadNonprompt();
	void ReadMC();

	int ProcessRawFile(std::string, std::vector<std::string>, TTree*, RooArgList&, RooArgList&, RooArgList&, bool);
};

#endif
