#ifndef T_DATASET_PARSER_H
#define T_DATASET_PARSER_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TSystem.h"
#include "TROOT.h"

#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooArgList.h"

class TDatasetParser
{
protected:
        std::vector<std::string> source_tree_names = {};	//The names of the TTrees in each source file to be retrieved

	std::vector<std::string> source_var_names = {};		//The branches to retrieve in source files
	std::vector<std::string> source_var_types = {};		//The corresponding type (int, float, double)
	RooArgList source_args;					//RooRealVars to represent them while reading
	
	std::vector<std::string> source_cut_names = {};		//Optional names of cuts to place on source data
	std::vector<std::string> source_cut_exprs = {};		//Cuts in terms of branche names in source files
	RooArgList source_cuts;					//RooFormulaVars to evaluate the cuts

	std::vector<std::string> target_var_names = {};		//The branches to update in target files
	std::vector<std::string> target_var_exprs = {};		//The expressio
	RooArgList target_args;					//RooFormulaVars to represent them while reading
	
	std::vector<std::string> target_cut_names = {};		//Optional names of cuts to place on target data
	std::vector<std::string> target_cut_exprs = {};		//Cuts in terms of branche names of the target ntuple
	RooArgList target_cuts;					//RooFormulaVars to evaluate the cuts

	template <typename T>
	void* MakeVoidAddressTemplate(TTree* t, std::string s)
	{
		void* ptr = (void*)(new T[max_size]);

		if(!t)return ptr;
		if(s == "")return ptr;
		if(!(t->GetBranch(s.c_str())))return ptr;

		t->SetBranchStatus(s.c_str(), 1);
		t->SetBranchAddress(s.c_str(), (T*)ptr);

		return ptr;
	}
	void* MakeVoidAddress(std::string, TTree*, std::string);

	template <typename T>
	float ReadVoidAddressTemplate(void* ptr, int i)
	{
		if(!ptr)return 0.0;
		if(!(0 <= i and i < max_size))return 0.0;

		return ((T*)ptr)[i];
	}
	float ReadVoidAddress(std::string, void*, int);

	template <typename T>
	void FreeVoidAddressTemplate(void* ptr)
	{
		if(!ptr)return;

		delete[] ((T*)ptr);
	}
	void FreeVoidAddress(std::string, void*);

	int UpdateNtuple(TTree*, std::string, bool);
public:
	std::string target_file_name = "";			//The fullpath name of the target file to recreate or update
	std::string target_ntpl_name = "";			//The name of the ntpl in that target file to recreate or update

	std::string source_file_name = "";			//The fullpath name of source files used to read in data
	std::string source_list_name = "";			//Or, the fullpath name of the list file containing the fullpath names of other source files
	int starting_index = 0;					//the inclusive starting index for the list of source files
	int stopping_index = -1;				//the exclusive stopping index for the list of source files (reads to the end if less than starting_index)

	int max_warnings = -1;		//the most warnings to output while Updating or Recreating the target (will always output warnings if negative)

	std::string source_var_sizes = "";	//If the TTrees in the source file contain arrays of integral types, this is the branch that specifies the multiplicity 
	int max_size = 1;			//the highest possible multiplicity--if this isn't large enough, the program will segfault

	TDatasetParser();

	void AddSourceTree(std::string);
	void AddSourceVar(std::string, std::string);
	void AddSourceCut(std::string, std::string);
	void AddTargetVar(std::string, std::string);
	void AddTargetCut(std::string, std::string);

	void ClearSourceTrees();
	void ClearSourceVars();
	void ClearSourceCuts();
	void ClearTargetVars();
	void ClearTargetCuts();
	void Clear();

	int Init(bool);	//Initializes the RooArgLists, outputs errors if bool is true
	int CheckTarget(bool);	//Returns 0 if target exists and is not a zombie, returns 1 otherwise (outputs a description with return 1 if bool is true)
	int MakeTarget();	//Makes the target file with an empty ntuple that has appropriate branches
	int UpdateTarget();	//Updates the target only if it exists
	int RecreateTarget();	//Creates the target and updates it; equivalent to calling MakeTarget() and, if successful, UpdateTarget()


	void AddSourceVar(std::string s){AddSourceVar(s, "");}
	void AddSourceCut(std::string s){AddSourceVar("", s);}
	void AddTargetVar(std::string s){AddTargetVar(s, s);}
	void AddTargetCut(std::string s){AddTargetCut("", s);}
};

#endif
