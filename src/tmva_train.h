#ifndef TMVA_TRAINER_H
#define TMVA_TRAINER_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <memory>

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
	std::string tree_name_prompt = "prompt_tree";
	std::string tree_name_nonprompt = "nonprompt_tree";
	std::string tree_name_background = "background_tree";

	std::string factory_name = "factory";
	std::string data_loader_name = "data_loader";

	int MAX_WARNINGS = 30;

	//The branch names needed from these TTrees to construct the mass, training vars, and training cuts	
	std::vector<std::string> raw_vars_prompt = {};
	std::vector<std::string> raw_vars_nonprompt = {};
	std::vector<std::string> raw_vars_data = {};

	//Strings representing the types of these branches
	std::vector<std::string> raw_types_prompt = {};
	std::vector<std::string> raw_types_nonprompt = {};
	std::vector<std::string> raw_types_data = {};

	//RooArgLists that go with the corresponding raw_..._vars public member vectors
	//There will be a RooRealVar for each element of the raw_..._vars vectors
	RooArgList raw_args_prompt;
	RooArgList raw_args_nonprompt;
	RooArgList raw_args_data;

	//RooArgLists that go with the corresponding raw_..._cuts public member vectors
	//There will be a RooFormulaVar for each element of the raw_..._cut_formulas vectors
	RooArgList raw_cuts_prompt;
	RooArgList raw_cuts_nonprompt;
	RooArgList raw_cuts_data;

	//The name of the mass branch to be used in the processed trees
	std::string mass_name = "mass";

	std::shared_ptr<RooFormulaVar> mass_prompt;
	std::shared_ptr<RooFormulaVar> mass_nonprompt;
	std::shared_ptr<RooFormulaVar> mass_data;

	//Members to create branches in the processed trees used directly for training
	//Formulas are in terms of the corresponding raw_..._vars members
	std::vector<std::string> training_vars = {};

	std::vector<std::string> training_var_formulas_prompt = {};
	std::vector<std::string> training_var_formulas_nonprompt = {};
	std::vector<std::string> training_var_formulas_data = {};

	RooArgList training_args_prompt;
	RooArgList training_args_nonprompt;
	RooArgList training_args_data;

	//Members to create branches in the processed trees used only for applying cuts when training
	//Formulas are in terms of the corresponding raw_..._vars members
	//should be have no elements also in training_vars
	std::vector<std::string> training_cut_vars = {};

	std::vector<std::string> training_cut_var_formulas_prompt = {};
	std::vector<std::string> training_cut_var_formulas_nonprompt = {};
	std::vector<std::string> training_cut_var_formulas_data = {};

	RooArgList training_cut_args_prompt;
	RooArgList training_cut_args_nonprompt;
	RooArgList training_cut_args_data;

	int ProcessRawFile
	(
		std::string, std::vector<std::string>, TTree*,
		std::string,
		RooArgList&, std::vector<std::string>,
		RooArgList&,
		std::shared_ptr<RooFormulaVar>&, RooArgList&, RooArgList&,
		bool
	);

	template <typename T>
	void* MakeVoidAddressTemplate(TTree*, std::string);
	template <typename T>
	T ReadVoidAddressTemplate(void*, int);

	void* MakeVoidAddress(std::string, TTree*, std::string);
	float ReadVoidAddress(std::string, void*, int);
public:
	//The names of the prompt, nonprompt, and background files the trees will be stored in
	std::string file_name_training = "";

	//The fullpath name of raw files used to read in data
	//Or, the fullpath name of the .list file if the raw file names are unspecified
	std::string file_name_prompt = "";
	std::string file_name_nonprompt = "";
	std::string file_name_data = "";

	std::string file_list_name_prompt = "";
	std::string file_list_name_nonprompt = "";
	std::string file_list_name_data = "";

	//The names of the TTrees in each file of the raw data files to be retrieved
        std::vector<std::string> tree_names_prompt = {};
        std::vector<std::string> tree_names_nonprompt = {};
        std::vector<std::string> tree_names_data = {};

	//The branch in the raw data files that specifies entry multiplicity
	//e.g., if some branches are arrays, this specifies their size
	int MAX_SIZE = 1;

	std::string raw_size_var_prompt = "";
	std::string raw_size_var_nonprompt = "";
	std::string raw_size_var_data = "";

	//cut expressions on raw data in terms of the elements of the corresponding raw_..._vars vectors
	std::vector<std::string> raw_cut_formulas_prompt = {};
	std::vector<std::string> raw_cut_formulas_nonprompt = {};
	std::vector<std::string> raw_cut_formulas_data = {};

	//Mass expressions
	std::string mass_formula_prompt = "";
	std::string mass_formula_nonprompt = "";
	std::string mass_formula_data = "";

	//Training options
	std::string factory_options = "!V:!Silent:Color:DrawProgressBar:AnalysisType=Classification";

	void AddRawVar(std::string, std::string, std::string, std::string, std::string, std::string);
	void AddRawVar(std::string s, std::string t, std::string u, std::string v){AddRawVar(s, t, s, t, u, v);}
	void AddRawVar(std::string s, std::string t){AddRawVar(s, t, s, t, s, t);}

	void AddTrainingVar(std::string, std::string, std::string, std::string);
	void AddTrainingVar(std::string s, std::string t, std::string u){AddTrainingVar(s, t, t, u);}
	void AddTrainingVar(std::string s, std::string t){AddTrainingVar(s, t, t, t);}

	void AddTrainingCutVar(std::string, std::string, std::string, std::string);
	void AddTrainingCutVar(std::string s, std::string t, std::string u){AddTrainingCutVar(s, t, t, u);}
	void AddTrainingCutVar(std::string s, std::string t){AddTrainingCutVar(s, t, t, t);}

	void InitFiles();
	void InitArgs();

	void ReadPrompt();
	void ReadNonprompt();
	void ReadMC();
};

#endif
