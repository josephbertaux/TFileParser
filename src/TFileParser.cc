#include "TFileParser.h"

//private member functions

void* TFileParser::MakeVoidAddress(std::string type, TTree* tree, std::string branch, int size)
{
	std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c){return std::tolower(c);});

	if(type == "i" or type == "int")return MakeVoidAddressTemplate<int>(tree, branch, size);
	if(type == "f" or type == "float")return MakeVoidAddressTemplate<float>(tree, branch, size);
	if(type == "d" or type == "double")return MakeVoidAddressTemplate<double>(tree, branch, size);

	return (void*)nullptr;
}

float TFileParser::ReadVoidAddress(std::string type, void* ptr, int i)
{
	std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c){return std::tolower(c);});

	if(type == "i" or type == "int")return ReadVoidAddressTemplate<int>(ptr, i);
	if(type == "f" or type == "float")return ReadVoidAddressTemplate<float>(ptr, i);
	if(type == "d" or type == "double")return ReadVoidAddressTemplate<double>(ptr, i);

	return 0.0;
}

void TFileParser::FreeVoidAddress(std::string type, void* ptr)
{
	if(type == "i" or type == "int")FreeVoidAddressTemplate<int>(ptr);
	if(type == "f" or type == "float")FreeVoidAddressTemplate<float>(ptr);
	if(type == "d" or type == "double")FreeVoidAddressTemplate<double>(ptr);
}

int TFileParser::UpdateNtuple(TTree* target_tree, std::string reader_file_name, bool w)
{
	int return_val = 0;
	std::stringstream output_str;

	bool b = false;
	uint u = 0;
	uint v = 0;
	int size = 1;
	int s = 0;
	int size_vars[source_size_vars.size()] = {0};
	int max_sizes[source_size_vars.size()] = {0};
	void* ptrs[source_var_names.size()] = {nullptr};
	float vals[target_var_names.size()] = {0.0};
	long long n;
	long long N;
	TFile* reader_file = (TFile*)nullptr;
	TTree* reader_tree = (TTree*)nullptr;
	TTree* friend_tree = (TTree*)nullptr;

	if(!target_tree)
	{
		return_val = 1;
		goto label;
	}

	if(reader_file_name == "")
	{
		output_str << std::endl;
		output_str << "Argument 'reader_file_name' not set" << std::endl;
		return_val = 1;
		goto label;
	}
	reader_file = TFile::Open(reader_file_name.c_str(), "READ");
	if(!reader_file)
	{
		output_str << std::endl;
		output_str << "Could not get file:" << std::endl;
		output_str << "\t" << reader_file_name << std::endl;
		return_val = 1;
		goto label;
	}
	if(reader_file->IsZombie())
	{
		output_str << std::endl;
		output_str << "File:" << std::endl;
		output_str << "\t" << reader_file_name << std::endl;
		output_str << "Is zombie" << std::endl;
		return_val = 1;
		goto label;
	}

	output_str << std::endl;
	output_str << "In file:" << std::endl;
	output_str << "\t" << reader_file_name << std::endl;

	if(w)
	{
		std::cout << std::endl;
		std::cout << "In file:" << std::endl;
		std::cout << "\t" << reader_file_name << std::endl;
	}

	for(u = 0; u < source_tree_names.size(); u++)
	{
		std::cout << "\t" << source_tree_names[u] << std::endl;
		friend_tree = (TTree*)reader_file->Get(source_tree_names[u].c_str());
		if(friend_tree)
		{
			if(!reader_tree)	reader_tree = friend_tree;
			else			reader_tree->AddFriend(friend_tree);
		}
		else
		{
			output_str << "\tCould not get tree:" << std::endl;
			output_str << "\t\t" << source_tree_names[u] << std::endl;
		}
	}

	if(!reader_tree)
	{
		return_val = 1;
		goto label;
	}

	reader_tree->SetBranchStatus("*", 0);
	reader_tree->ResetBranchAddresses();

	for(v = 0; v < source_size_vars.size(); v++)
	{
		if(!reader_tree->GetBranch(source_size_vars[v].c_str()))
		{
			output_str << "\tCould not get size var branch:" << std::endl;
			output_str << "\t\t" << source_size_vars[v] << std::endl;
			return_val = 1;
		}
		else
		{
			size_vars[v] = 0;
			reader_tree->SetBranchStatus(source_size_vars[v].c_str(), 1);
			reader_tree->SetBranchAddress(source_size_vars[v].c_str(), &(size_vars[v]));
		}
	}

	if(w)
	{
		std::cout << "\tFetched size branches" << std::endl;
	}

	N = reader_tree->GetEntriesFast();
	for(n = 0; n < N; n++)
	{
		reader_tree->GetEntry(n);
		for(v = 0; v < source_size_vars.size(); v++)
		{
			if(size_vars[v] > max_sizes[v])max_sizes[v] = size_vars[v];
		}
	}

	if(w)
	{
		std::cout << "\tFetched sizes" << std::endl;
		for(v = 0; v < source_size_vars.size(); v++)
		{
			std::cout << "\t\t" << source_size_vars[v] << ":\t" << max_sizes[v] << std::endl;
		}
	}

	for(u = 0; u < source_var_names.size(); u++)
	{
		if(!reader_tree->GetBranch(source_var_names[u].c_str()))
		{
			output_str << "\tCould not get source branch:" << std::endl;
			output_str << "\t\t" << source_var_names[u] << std::endl;
			return_val = 1;
		}
		else
		{
			try
			{
				ptrs[u] = MakeVoidAddress(source_var_types[u], reader_tree, source_var_names[u], std::stoi(source_var_sizes[u]));
				if(ptrs[u] == nullptr)
				{
					output_str << "\tFailed to allocate size:" << std::endl;
					output_str << "\t\t" << source_var_sizes[u] << std::endl;
					output_str << "\tFor source var:" << std::endl;
					output_str << "\t\t" << source_var_names[u] << std::endl;
					return_val = 1;
				}
			}
			catch(const std::invalid_argument&)
			{
				b = true;
				for(v = 0; v < source_size_vars.size(); v++)
				{
					if(source_size_vars[v] == source_var_sizes[u])
					{
						ptrs[u] = MakeVoidAddress(source_var_types[u], reader_tree, source_var_names[u], max_sizes[v]);
						b = false;
						break;
					}
				}
				if(b)
				{
					output_str << "\tSource branch:" << std::endl;
					output_str << "\t\t" << source_var_names[u] << std::endl;
					output_str << "\tHas size specified as non-numeric value:" << std::endl;
					output_str << "\t\t" << source_var_sizes[u] << std::endl;
					output_str << "\tHowever, no variable of the same name found among source_size_vars" << std::endl;
					return_val = 1;
				}
			}
		}
	}
	if(return_val)goto label;

	if(w)
	{
		std::cout << "\tFetched all branches" << std::endl;
	}

	target_tree->ResetBranchAddresses();
	for(u = 0; u < target_var_names.size(); u++)
	{
		if(!target_tree->GetBranch(target_var_names[u].c_str()))
		{
			//Warnings for this guard would've been caught by a preceding guard (CheckTarget)
			//No appending to the output stream
			return_val = 1;
		}
		else
		{
			target_tree->SetBranchStatus(target_var_names[u].c_str(), 1);
			target_tree->SetBranchAddress(target_var_names[u].c_str(), &(vals[u]));
		}
	}
	if(return_val)goto label;

	if(w)
	{
		std::cout << "\tAll branches found, beginning reading..." << std::endl;
	}

	N = reader_tree->GetEntriesFast();
	for(n = 0; n < N; n++)
	{
		reader_tree->GetEntry(n);
		for(u = 0; u < source_args.getSize(); u++)
		{
			((RooRealVar*)&(source_args[u]))->setVal(ReadVoidAddress(source_var_types[u], ptrs[u], 0));
		}

		for(v = 0; v < source_size_vars.size(); v++)
		{
			for(s = 0; s < size_vars[v]; s++)
			{
				for(u = 0; u < source_args.getSize(); u++)
				{
					if(source_var_sizes[u] != source_size_vars[v])continue;
					((RooRealVar*)&(source_args[u]))->setVal(ReadVoidAddress(source_var_types[u], ptrs[u], s));
				}

				b = false;
				for(u = 0; u < source_cuts.getSize(); u++)
				{
					if(((RooFormulaVar*)&(source_cuts[u]))->getValV() == 0.0)
					{
						b = true;
						break;
					}
				}
				if(b)continue;
				for(u = 0; u < target_cuts.getSize(); u++)
				{
					if(((RooFormulaVar*)&(target_cuts[u]))->getValV() == 0.0)
					{
						b = true;
						break;
					}
				}
				if(b)continue;

				for(u = 0; u < target_args.getSize(); u++)
				{
					vals[u] = ((RooFormulaVar*)&(target_args[u]))->getValV();
				}

				target_tree->Fill();
			}
		}
	}

	label:
	if(reader_tree)reader_tree->ResetBranchAddresses();
	if(target_tree)target_tree->ResetBranchAddresses();
	if(reader_file)reader_file->Close();
	for(u = 0; u < source_var_names.size(); u++)FreeVoidAddress(source_var_types[u], ptrs[u]);
	output_str << std::ends;
	if(w and return_val)std::cout << output_str.str() << std::endl;
	return return_val;
}

//public member functions

void TFileParser::ClearSourceTrees()
{
	source_tree_names.clear();
}

void TFileParser::ClearSourceVars()
{
	source_var_names.clear();
	source_var_types.clear();
}

void TFileParser::ClearSourceCuts()
{
	source_cut_names.clear();
	source_cut_exprs.clear();
}

void TFileParser::ClearTargetVars()
{
	target_var_names.clear();
	target_var_exprs.clear();
}

void TFileParser::ClearTargetCuts()
{
	target_cut_names.clear();
	target_cut_exprs.clear();
}

void TFileParser::Clear()
{
	target_file_name = "";
	target_ntpl_name = "";

	ClearSourceTrees();

	source_file_name = "";
	source_list_name = "";
	starting_index = 0;
	stopping_index = -1;

	max_warnings = -1;

	source_var_sizes.clear();

	ClearSourceVars();
	ClearSourceCuts();
	ClearTargetVars();
	ClearTargetCuts();
}

TFileParser::TFileParser()
{
	Clear();
}

int TFileParser::Init(bool w)
{
	int return_val = 0;
	std::stringstream output_str;

	uint u = 0;

	source_args.clear();
	source_cuts.clear();
	target_args.clear();
	target_cuts.clear();

	if(source_var_names.size() == 0)
	{
		output_str << "No source vars have been added" << std::endl;
		return_val = 1;
		goto label;
	}
	if(target_var_names.size() == 0)
	{
		output_str << "No target vars have been added" << std::endl;
		return_val = 1;
		goto label;
	}

	for(u = 0; u < source_var_names.size(); u++)
	{
		source_args.addOwned(*(new RooRealVar(source_var_names[u].c_str(), source_var_names[u].c_str(), 0.0, -FLT_MAX, FLT_MAX)));
	}
	for(u = 0; u < source_cut_names.size(); u++)
	{
		source_cuts.addOwned(*(new RooFormulaVar(source_cut_names[u].c_str(), source_cut_exprs[u].c_str(), source_args)));
	}
	for(u = 0; u < target_var_names.size(); u++)
	{
		target_args.addOwned(*(new RooFormulaVar(target_var_names[u].c_str(), target_var_exprs[u].c_str(), source_args)));
	}
	for(u = 0; u < target_cut_names.size(); u++)
	{
		target_cuts.addOwned(*(new RooFormulaVar(target_cut_names[u].c_str(), target_cut_exprs[u].c_str(), target_args)));
	}

	label:
	output_str << std::ends;
	if(w and return_val)std::cout << output_str.str() << std::endl;
	return return_val;
}

int TFileParser::CheckTarget(bool w)
{
	int return_val = 0;
	std::stringstream output_str;

	uint u = 0;
	TFile* target_file = (TFile*)nullptr;
	TNtuple* target_ntpl = (TNtuple*)nullptr;

	if(target_file_name == "")
	{
		output_str << "Member 'target_file_name' not set" << std::endl;
		return_val = 1;
		goto label;
	}
	target_file = TFile::Open(target_file_name.c_str(), "READ");
	if(!target_file)
	{
		output_str << "Could not get file:" << std::endl;
		output_str << "\t" << target_file_name << std::endl;
		return_val = 1;
		goto label;
	}
	if(target_file->IsZombie())
	{
		output_str << "File:" << std::endl;
		output_str << "\t" << target_file_name << std::endl;
		output_str << "Is zombie" << std::endl;
		return_val = 1;
		goto label;
	}

	if(target_ntpl_name == "")
	{
		output_str << "Member 'target_ntpl_name' not set" << std::endl;
		return_val = 1;
		goto label;
	}
	output_str << "In file:" << std::endl;
	target_ntpl = (TNtuple*)target_file->Get(target_ntpl_name.c_str());
	if(!target_ntpl)
	{
		output_str << "\tCould not get ntuple:" << std::endl;
		output_str << "\t\t" << target_ntpl_name << std::endl;
		return_val = 1;
		goto label;
	}
	for(u = 0; u < target_var_names.size(); u++)
	{
		if(!target_ntpl->GetBranch(target_var_names[u].c_str()))
		{
			output_str << "\tCould not get target branch:" << std::endl;
			output_str << "\t\t" << source_var_names[u] << std::endl;
			return_val = 1;
		}
	}
	if(return_val)goto label;
	//superfluous but I'm doing this on principle
	//Any code between this and label should be skipped

	label:
	if(target_file)target_file->Close();
	output_str << std::ends;
	if(w and return_val)std::cout << output_str.str() << std::endl;
	return return_val;
}

int TFileParser::UpdateTarget(bool w)
{
	int return_val = 1;
	std::stringstream output_str;

	int warnings = 0;
	int index = -1;
	std::string current_file_name = "";
	TFile* target_file = (TFile*)nullptr;
	TTree* target_ntpl = (TTree*)nullptr;

	if(CheckTarget(w))
	{
		return_val = 1;
		goto label;
	}
	if(source_tree_names.size() == 0)
	{
		output_str << "No source trees have been added" << std::endl;
		return_val = 1;
		goto label;
	}
	if(Init(w))
	{
		return_val = 1;
		goto label;
	}

	target_file = TFile::Open(target_file_name.c_str(), "UPDATE");
	if(!target_file)
	{
		//Warnings for this guard would've been caught by a preceding guard (CheckTarget)
		//No appending to the output stream
		return_val = 1;
		goto label;
	}
	target_ntpl = (TTree*)target_file->Get(target_ntpl_name.c_str());
	if(!target_ntpl)
	{
		//Warnings for this guard would've been caught by a preceding guard (CheckTarget)
		//No appending to the output stream
		return_val = 1;
		goto label;
	}
	target_ntpl->SetDirectory(target_file);

	if(source_file_name == "" and source_list_name == "")
	{
		output_str << "Member 'source_file_name' not set" << std::endl;
		output_str << "Member 'source_list_name' not set" << std::endl;
		return_val = 1;
		goto label;
	}

	if(source_file_name != "")
	{
		//If there are no errors (UpdateNtuple returns 0), store that at least one successful write occured (return_val = 0)
		if(!UpdateNtuple(target_ntpl, source_file_name, w and (max_warnings != 0)))return_val = 0;
	}
	if(source_list_name != "")
	{
		std::ifstream source_list(source_list_name.c_str(), std::ios_base::in);
		if(!source_list.is_open())
		{
			output_str << "Could not get list:" << std::endl;
			output_str << "\t" << source_list_name << std::endl;
			return_val = 1;
			goto label;
		}

		while(true)
		{
			if(source_list.bad())break;

			index++;
			source_list >> current_file_name;

			if(index < starting_index)continue;
			if(starting_index < stopping_index and index >= stopping_index)break;
			if(source_list.eof())break;
			if(current_file_name == "")continue;

			//std::cout << current_file_name << std::endl;
			//std::cout << "\t(index: " << index << "\tstarting: " << starting_index << "\tstopping: " << stopping_index << ")" << std::endl;

			//If there are no errors (UpdateNtuple returns 0), store that at least one successful write occured (return_val = 0)
			if(!UpdateNtuple(target_ntpl, current_file_name, w and (max_warnings < 0 or warnings < max_warnings)))return_val = 0;
			else warnings ++;
		}

		source_list.close();
	}

	label:
	if(target_file)
	{
		target_file->cd();
		if(target_ntpl)target_ntpl->Write();
		target_file->Write();
		target_file->Close();
	}
	output_str << std::ends;
	if(w and return_val)std::cout << output_str.str() << std::endl;
	return return_val;
}

int TFileParser::RecreateTarget(bool w)
{
	int return_val = 0;
	std::stringstream output_str;

	uint u = 0;
	float f = 0.0;
	TFile* target_file = (TFile*)nullptr;
	TNtuple* target_ntpl = (TNtuple*)nullptr;

	if(target_file_name == "")
	{
		output_str << "Member 'target_file_name' not set" << std::endl;
		return_val = 1;
		goto label;
	}
	target_file = TFile::Open(target_file_name.c_str(), "RECREATE");
	if(!target_file)
	{
		output_str << "Failed to recreate file:" << std::endl;
		output_str << "\t" << target_file_name << std::endl;
		return_val = 1;
		goto label;
	}
	if(target_file->IsZombie())
	{
		output_str << "File:" << std::endl;
		output_str << "\t" << target_file_name << std::endl;
		output_str << "Is zombie" << std::endl;
		return_val = 1;
		goto label;
	}

	if(target_ntpl_name == "")
	{
		output_str << "Member 'target_ntpl_name' not set" << std::endl;
		return_val = 1;
		goto label;
	}
	if(target_var_names.size() == 0)
	{
		output_str << "No target vars have been added" << std::endl;
		return_val = 1;
		goto label;
	}
	target_ntpl = new TNtuple(target_ntpl_name.c_str(), target_ntpl_name.c_str(), "");
	if(!target_ntpl)
	{
		output_str << "Failed to recreate ntuple:" << std::endl;
		output_str << "\t" << target_ntpl_name << std::endl;
		return_val = 1;
		goto label;
	}
	target_ntpl->SetDirectory(target_file);
	for(u = 0; u < target_var_names.size(); u++)
	{
		target_ntpl->Branch(target_var_names[u].c_str(), &f);
	}

	label:
	if(target_file)
	{
		target_file->cd();
		if(target_ntpl)target_ntpl->Write();
		target_file->Write();
		target_file->Close();
	}
	output_str << std::ends;
	if(w and return_val)std::cout << output_str.str() << std::endl;
	return return_val;
}

//member accessor functions

//Implementations for Set...(...)
int TFileParser::SetTargetFile(std::string name)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetTargetFile(std::string name):" << std::endl;
	
	if(name == "")
	{
		output_str << "\tArg 'name' passed as \"\"" << std::endl; 
		return_val = 1;
	}
	
	target_file_name = name;

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFileParser::SetTargetNtpl(std::string name)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetTargetNtpl(std::string name):" << std::endl;
	
	if(name == "")
	{
		output_str << "\tArg 'name' passed as \"\"" << std::endl; 
		return_val = 1;
	}
	
	target_ntpl_name = name;

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

//Implementations for Add...(...)
int TFileParser::AddSourceTree(std::string name)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddSourceTree(std::string name):" << std::endl;

	if(name == "")
	{
		output_str << "\tArg 'name' passed as \"\"" << std::endl;
		return_val = 1;
		goto label;
	}

	source_tree_names.push_back(name);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFileParser::AddSizeVar(std::string name)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddSizeVar(std::string name):" << std::endl;

	if(name == "")
	{
		output_str << "\tArg 'name' passed as \"\"" << std::endl;
		return_val = 1;
		goto label;
	}

	for(uint u = 0; u < source_size_vars.size(); u++)
	{
		if(source_size_vars[u] == name)
		{
			output_str << "\tArg '" << name << "' has already been added" << std::endl;
			return_val = 1;
			goto label;
		}
	}

	source_size_vars.push_back(name);


	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFileParser::AddSourceVar(std::string name, std::string type)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddSourceVar(std::string name, std::string type):" << std::endl;

	if(name == "")
	{
		output_str << "\tArg 'name' passed as \"\"" << std::endl;
		return_val = 1;
		goto label;
	}
	if(type == "")type = "f";

	for(uint u = 0; u < source_var_names.size(); u++)
	{
		if(source_var_names[u] == name)
		{
			output_str << "\tArg '" << name << "' has already been added" << std::endl;
			return_val = 1;
			goto label;
		}
	}

	source_var_names.push_back(name);
	source_var_types.push_back(type.substr(0, type.find("[")));

	if(type.find("[") == std::string::npos or type.find("]") == std::string::npos)
	{
		type = "1";
	}
	else
	{
		type = type.substr(type.find("[") + 1);
		type = type.substr(0, type.find("]"));
	}
	source_var_sizes.push_back(type);

	std::cout << "Added source var:" << std::endl;
	std::cout << "\tName: " << source_var_names.back() << std::endl;
	std::cout << "\tType: " << source_var_types.back() << std::endl;
	std::cout << "\tSize: " << source_var_sizes.back() << std::endl;

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFileParser::AddSourceCut(std::string name, std::string expr)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddSourceCut(std::string name, std::string expr):" << std::endl;

	if(name == "")name = "SourceCut" + std::to_string(source_cut_exprs.size());
	if(expr == "")
	{
		output_str << "\tArg 'expr' passed as \"\"" << std::endl;
		return_val = 1;
		goto label;
	}

	for(uint u = 0; u < source_cut_names.size(); u++)
	{
		if(source_cut_names[u] == name)
		{
			output_str << "\tArg '" << name << "' has already been added" << std::endl;
			return_val = 1;
			goto label;
		}
	}

	source_cut_names.push_back(name);
	source_cut_exprs.push_back(expr);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFileParser::AddTargetVar(std::string name, std::string expr)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddTargetVar(std::string name, std::string type):" << std::endl;

	if(name == "")
	{
		output_str << "\tArg 'name' passed as \"\"" << std::endl;
		return_val = 1;
		goto label;
	}
	if(expr == "")expr = name;

	for(uint u = 0; u < target_var_names.size(); u++)
	{
		if(target_var_names[u] == name)
		{
			output_str << "\tArg '" << name << "' has already been added" << std::endl;
			return_val = 1;
			goto label;
		}
	}

	target_var_names.push_back(name);
	target_var_exprs.push_back(expr);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFileParser::AddTargetCut(std::string name, std::string expr)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddTargetCut(std::string name, std::string expr):" << std::endl;

	if(name == "")name = "TargetCut" + std::to_string(target_cut_exprs.size());
	if(expr == "")
	{
		output_str << "\tArg 'expr' passed as \"\"" << std::endl;
		return_val = 1;
		goto label;
	}

	for(uint u = 0; u < target_cut_names.size(); u++)
	{
		if(target_cut_names[u] == name)
		{
			output_str << "\tArg '" << name << "' has already been added" << std::endl;
			return_val = 1;
			goto label;
		}
	}

	target_cut_names.push_back(name);
	target_cut_exprs.push_back(expr);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}
