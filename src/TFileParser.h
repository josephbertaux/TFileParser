#ifndef TFILE_PARSER_H
#define TFILE_PARSER_H

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"

#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooArgList.h"

class TFileParser
{
protected:
	std::string target_file_name = "";			//The fullpath name of the target file to recreate or update
	std::string target_ntpl_name = "";			//The name of the ntpl in that target file to recreate or update

	std::string source_file_name = "";			//The fullpath name of source files used to read in data
	std::string source_list_name = "";			//Or, the fullpath name of the list file containing the fullpath names of other source files

	int starting_index = 0;					//the inclusive starting index for the list of source files
	int stopping_index = -1;				//the exclusive stopping index for the list of source files (reads to the end if less than starting_index)

	int max_warnings = -1;					//the most warnings to output while Updating or Recreating the target (will always output warnings if negative)

	std::vector<std::string> source_tree_names = {};	//The names of the TTrees in each source file to be retrieved

	std::vector<std::string> source_size_vars = {};		//Branches of type int which determine the size of other branches

	std::vector<std::string> source_var_names = {};		//The branches to retrieve in source files
	std::vector<std::string> source_var_types = {};		//The corresponding type (int, float, double)
	std::vector<std::string> source_var_sizes = {};		//The number of instances of the current source var entry
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
	void* MakeVoidAddressTemplate(TTree* t, std::string s, int size)
	{
		if(size < 1)size = 1;
		void* ptr = (void*)(new T[size]);

		if(!t)return ptr;
		if(s == "")return ptr;
		if(!(t->GetBranch(s.c_str())))return ptr;

		t->SetBranchStatus(s.c_str(), 1);
		t->SetBranchAddress(s.c_str(), (T*)ptr);

		return ptr;
	}
	void* MakeVoidAddress(std::string, TTree*, std::string, int size);

	template <typename T>
	float ReadVoidAddressTemplate(void* ptr, int i)
	{
		if(!ptr)return 0.0;
		//if(!(0 <= i and i < size))return 0.0;

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
	void ClearSourceTrees();
	void ClearSourceVars();
	void ClearSourceCuts();
	void ClearTargetVars();
	void ClearTargetCuts();
	void Clear();

					//bool is if error output is to be written to std::out
	int Init(bool);			//Initializes the RooArgLists
	int CheckTarget(bool);		//Returns 0 if target exists and is not a zombie, returns 1 otherwise
	int UpdateTarget(bool);		//Updates the target only if it exists
	int RecreateTarget(bool);	//Recreates the target file with an empty target ntuple with the correct branches

	TFileParser();

 	int SetTargetFile(std::string);
	int SetTargetNtpl(std::string);
	int SetSourceName(std::string s){source_file_name = s;return 0;}
	int SetSourceList(std::string s){source_list_name = s;return 0;}

	int SetStartingIndex(int i){starting_index = i;return 0;}
	int SetStoppingIndex(int i){stopping_index = i;return 0;}
	int SetMaxWarnings(int i){max_warnings = i;return 0;}

	int GetStartingIndex(){return starting_index;}
	int GetStoppingIndex(){return stopping_index;}

	int AddSourceTree(std::string);
	int AddSizeVar(std::string s);
	int AddSourceVar(std::string, std::string);
	int AddSourceCut(std::string, std::string);
	int AddTargetVar(std::string, std::string);
	int AddTargetCut(std::string, std::string);

	int Init(){return Init(true);}
	int CheckTarget(){return CheckTarget(true);}
	int UpdateTarget(){return UpdateTarget(true);}
	int RecreateTarget(){return RecreateTarget(true);}

	//overloads
	int AddSourceVar(std::string s){return AddSourceVar(s, "");}
	int AddSourceCut(std::string s){return AddSourceCut("", s);}
	int AddTargetVar(std::string s){return AddTargetVar(s, s);}
	int AddTargetCut(std::string s){return AddTargetCut("", s);}
};

#endif
