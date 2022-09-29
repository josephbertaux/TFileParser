#include "TDatasetParser.h"

//private member functions

void* TDatasetParser::MakeVoidAddress(std::string type, TTree* tree, std::string branch)
{
	std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c){return std::tolower(c);});

	if(type == "i" or type == "int")return MakeVoidAddressTemplate<int>(tree, branch);
	if(type == "f" or type == "float")return MakeVoidAddressTemplate<float>(tree, branch);
	if(type == "d" or type == "double")return MakeVoidAddressTemplate<double>(tree, branch);

	return (void*)nullptr;
}

float TDatasetParser::ReadVoidAddress(std::string type, void* ptr, int i)
{
	std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c){return std::tolower(c);});

	if(type == "i" or type == "int")return ReadVoidAddressTemplate<int>(ptr, i);
	if(type == "f" or type == "float")return ReadVoidAddressTemplate<float>(ptr, i);
	if(type == "d" or type == "double")return ReadVoidAddressTemplate<double>(ptr, i);

	return 0.0;
}

void TDatasetParser::FreeVoidAddress(std::string type, void* ptr)
{
	if(type == "i" or type == "int")FreeVoidAddressTemplate<int>(ptr);
	if(type == "f" or type == "float")FreeVoidAddressTemplate<float>(ptr);
	if(type == "d" or type == "double")FreeVoidAddressTemplate<double>(ptr);
}

int TDatasetParser::UpdateNtuple(TTree* target_tree, std::string reader_file_name, bool v)
{
	if(!target_tree)return 1;

	if(reader_file_name == "")
	{
		if(v)
		{
			std::cout << std::endl;
			std::cout << "Argument 'reader_file_name' not set" << std::endl;
		}

		return 1;
	}
	TFile* reader_file = TFile::Open(reader_file_name.c_str(), "READ");
	if(!reader_file)
	{
		if(v)
		{
			std::cout << std::endl;
			std::cout << "Could not get file:" << std::endl;
			std::cout << "\t" << reader_file_name << std::endl;
		}

		return 1;
	}
	if(reader_file->IsZombie())
	{
		if(v)
		{
			std::cout << std::endl;
			std::cout << "File:" << std::endl;
			std::cout << "\t" << reader_file_name << std::endl;
			std::cout << "Is zombie" << std::endl;
		}

		reader_file->Close();
		return 1;
	}

	bool b = false;
	uint u = 0;
	int size = 1;
	int s = 0;
	if(v)
	{
		std::cout << std::endl;
		std::cout << "In file:" << std::endl;
		std::cout << reader_file_name << std::endl;
	}

	TTree* reader_tree = (TTree*)reader_file->Get(source_tree_names[0].c_str());
	if(!reader_tree)
	{
		if(v)
		{
			std::cout << "\tCould not get tree:" << std::endl;
			std::cout << "\t" << source_tree_names[0] << std::endl;
		}
	}
	TTree* friend_tree = (TTree*)nullptr;
	for(u = 1; u < source_tree_names.size(); u++)
	{
		friend_tree = (TTree*)reader_file->Get(source_tree_names[u].c_str());
		if(friend_tree)
		{
			if(reader_tree)reader_tree->AddFriend(friend_tree);
			else reader_tree = friend_tree;

			continue;
		}
		if(v)
		{
			std::cout << "\tCould not get tree:" << std::endl;
			std::cout << "\t" << source_tree_names[u] << std::endl;
		}
	}

	reader_tree->ResetBranchAddresses();
	if(source_var_sizes != "")
	{
		if(!reader_tree->GetBranch(source_var_sizes.c_str()))
		{
			if(v)
			{
				std::cout << "\tCould not get source var sizes branch:" << std::endl;
				std::cout << "\t" << source_var_sizes << std::endl;
			}
			b = true;
		}
		else
		{
			reader_tree->SetBranchStatus(source_var_sizes.c_str(), 1);
			reader_tree->SetBranchAddress(source_var_sizes.c_str(), &size);
		}
	}
	void* ptrs[source_var_names.size()] = {nullptr};
	for(u = 0; u < source_var_names.size(); u++)
	{
		if(!reader_tree->GetBranch(source_var_names[u].c_str()))
		{
			if(v)
			{
				std::cout << "\tCould not get source branch:" << std::endl;
				std::cout << "\t" << source_var_names[u] << std::endl;
			}
			b = true;
			continue;
		}
		ptrs[u] = MakeVoidAddress(source_var_types[u], reader_tree, source_var_names[u]);
	}

	target_tree->ResetBranchAddresses();
	float vals[target_var_names.size()] = {0.0};
	for(u = 0; u < target_var_names.size(); u++)
	{
		if(!target_tree->GetBranch(target_var_names[u].c_str()))
		{
			if(v)
			{
				std::cout << "\tCould not get target branch:" << std::endl;
				std::cout << "\t" << source_var_names[u] << std::endl;
			}
			b = true;
			continue;
		}
		target_tree->SetBranchStatus(target_var_names[u].c_str(), 1);
		target_tree->SetBranchAddress(target_var_names[u].c_str(), &(vals[u]));
	}
	if(b)
	{
		reader_file->Close();
		return 1;
	}

	long long n;
	long long N;
	N = reader_tree->GetEntriesFast();
	for(n = 0; n < N; n++)
	{
		reader_tree->GetEntry(n);

		for(s = 0; s < size; s++)
		{
			for(u = 0; u < source_args.getSize(); u++)
			{
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

	reader_tree->ResetBranchAddresses();
	target_tree->ResetBranchAddresses();
	reader_file->Close();

	for(u = 0; u < source_var_names.size(); u++)
	{
		FreeVoidAddress(source_var_types[u], ptrs[u]);
	}
	return 0;
}

//public member functions

TDatasetParser::TDatasetParser()
{
	Clear();

	target_file_name = "";
	target_ntpl_name = "";

	source_file_name = "";
	source_list_name = "";
	starting_index = 0;
	stopping_index = -1;

	max_warnings = -1;
	max_size = 1;
}

void TDatasetParser::AddSourceTree(std::string name)
{
	if(name == "")return;

	source_tree_names.push_back(name);
}

void TDatasetParser::AddSourceVar(std::string name, std::string type)
{
	if(name == "")return;
	if(type == "")type = "F";

	for(uint u = 0; u < source_var_names.size(); u++)
	{
		if(source_var_names[u] == name)return;
	}

	source_var_names.push_back(name);
	source_var_types.push_back(type);
}

void TDatasetParser::AddSourceCut(std::string name, std::string expr)
{
	if(expr == "")return;
	if(name == "")name = "SourceCut" + std::to_string(source_cut_exprs.size());

	source_cut_names.push_back(name);
	source_cut_exprs.push_back(expr);
}

void TDatasetParser::AddTargetVar(std::string name, std::string expr)
{
	if(name == "")return;
	if(expr == "")expr = name;

	for(uint u = 0; u < target_var_names.size(); u++)
	{
		if(target_var_names[u] == name)return;
	}

	target_var_names.push_back(name);
	target_var_exprs.push_back(expr);
}

void TDatasetParser::AddTargetCut(std::string name, std::string expr)
{
	if(expr == "")return;
	if(name == "")name = "TargetCut" + std::to_string(target_cut_exprs.size());

	target_cut_names.push_back(name);
	target_cut_exprs.push_back(expr);
}

void TDatasetParser::ClearSourceTrees()
{
	source_tree_names.clear();
}

void TDatasetParser::ClearSourceVars()
{
	source_var_names.clear();
	source_var_types.clear();
}

void TDatasetParser::ClearSourceCuts()
{
	source_cut_names.clear();
	source_cut_exprs.clear();
}

void TDatasetParser::ClearTargetVars()
{
	target_var_names.clear();
	target_var_exprs.clear();
}

void TDatasetParser::ClearTargetCuts()
{
	target_cut_names.clear();
	target_cut_exprs.clear();
}

void TDatasetParser::Clear()
{
	ClearSourceTrees();
	ClearSourceVars();
	ClearSourceCuts();
	ClearTargetVars();
	ClearTargetCuts();
}

int TDatasetParser::Init(bool v)
{
	if(source_var_names.size() == 0)
	{
		if(v)
		{
			std::cout << "No source vars have been added" << std::endl;
		}

		source_args.clear();
		source_cuts.clear();
		target_args.clear();
		target_cuts.clear();
		return 1;
	}

	if(target_var_names.size() == 0)
	{
		if(v)
		{
			std::cout << "No target vars have been added" << std::endl;
		}

		source_args.clear();
		source_cuts.clear();
		target_args.clear();
		target_cuts.clear();
		return 1;
	}

	uint u;
	source_args.clear();
	for(u = 0; u < source_var_names.size(); u++)
	{
		source_args.addOwned(*(new RooRealVar(source_var_names[u].c_str(), source_var_names[u].c_str(), 0.0, -FLT_MAX, FLT_MAX)));
	}

	source_cuts.clear();
	for(u = 0; u < source_cut_names.size(); u++)
	{
		source_cuts.addOwned(*(new RooFormulaVar(source_cut_names[u].c_str(), source_cut_exprs[u].c_str(), source_args)));
	}

	target_args.clear();
	for(u = 0; u < target_var_names.size(); u++)
	{
		target_args.addOwned(*(new RooFormulaVar(target_var_names[u].c_str(), target_var_exprs[u].c_str(), source_args)));
	}

	target_cuts.clear();
	for(u = 0; u < target_cut_names.size(); u++)
	{
		target_cuts.addOwned(*(new RooFormulaVar(target_cut_names[u].c_str(), target_cut_exprs[u].c_str(), target_args)));
	}

	return 0;
}

int TDatasetParser::CheckTarget(bool v)
{
	if(target_file_name == "")
	{
		if(v)
		{
			std::cout << "Member 'target_file_name' not set" << std::endl;
		}

		return 1;
	}
	TFile* target_file = TFile::Open(target_file_name.c_str(), "READ");
	if(!target_file)
	{
		if(v)
		{
			std::cout << "Could not get file:" << std::endl;
			std::cout << "\t" << target_file_name << std::endl;
		}

		return 1;
	}
	if(target_file->IsZombie())
	{
		if(v)
		{
			std::cout << "File:" << std::endl;
			std::cout << "\t" << target_file_name << std::endl;
			std::cout << "Is zombie" << std::endl;
		}

		target_file->Close();
		return 1;
	}

	if(target_ntpl_name == "")
	{
		if(v)
		{
			std::cout << "Member 'target_ntpl_name' not set" << std::endl;
		}

		target_file->Close();
		return 1;
	}
	TNtuple* target_ntpl = (TNtuple*)target_file->Get(target_ntpl_name.c_str());
	if(!target_ntpl)
	{
		if(v)
		{
			std::cout << "Could not get ntuple:" << std::endl;
			std::cout << "\t" << target_ntpl_name << std::endl;
			std::cout << "From file:" << std::endl;
			std::cout << "\t" << target_file_name << std::endl;
		}

		target_file->Close();
		return 1;
	}

	target_file->Close();
	return 0;
}

int TDatasetParser::MakeTarget()
{
	if(CheckTarget(false))
	{
		if(target_file_name == "")
		{
			std::cout << "Member 'target_file_name' not set" << std::endl;

			return 1;
		}
		TFile* target_file = TFile::Open(target_file_name.c_str(), "RECREATE");
		if(target_file->IsZombie())
		{
			std::cout << "File:" << std::endl;
			std::cout << "\t" << target_file_name << std::endl;
			std::cout << "Is zombie" << std::endl;

			target_file->Close();
			return 1;
		}


		if(target_ntpl_name == "")
		{
			std::cout << "Member 'target_ntpl_name' not set" << std::endl;

			target_file->Close();
			return 1;
		}
		if(target_var_names.size() == 0)
		{
			std::cout << "No target vars have been added" << std::endl;

			target_file->Close();
			return 1;
		}
		TNtuple* target_ntpl = new TNtuple(target_ntpl_name.c_str(), target_ntpl_name.c_str(), "");
		target_ntpl->SetDirectory(target_file);
		float f = 0.0;
		for(uint u = 0; u < target_var_names.size(); u++)
		{
			target_ntpl->Branch(target_var_names[u].c_str(), &f);	//Overload 10 of 13 of TTree::Branch()
		}

		target_file->cd();
		target_ntpl->Write();
		target_file->Write();
		target_file->Close();
	}

	return 0;
}

int TDatasetParser::UpdateTarget()
{
	if(CheckTarget(true))return 1;

	if(source_tree_names.size() == 0)
	{
		std::cout << "No source trees have been added" << std::endl;

		return 1;
	}

	Init(true);

	TFile* target_file = TFile::Open(target_file_name.c_str(), "UPDATE");
	TTree* target_ntpl = (TTree*)target_file->Get(target_ntpl_name.c_str());
	target_ntpl->SetDirectory(target_file);

	int i;			//to store the success of UpdateNtuple
	bool b = false;		//if at least 1 successful call of UpdateNtuple occurred

	if(source_file_name == "" and source_list_name == "")
	{
		std::cout << "Member 'source_file_name' not set" << std::endl;
		std::cout << "Member 'source_list_name' not set" << std::endl;

		target_file->cd();
		target_ntpl->Write();
		target_file->Write();
		target_file->Close();
		return 1;
	}

	if(source_file_name != "")
	{
		i = UpdateNtuple(target_ntpl, source_file_name, (max_warnings != 0));
		if(i == 0)b = true;
	}

	if(source_list_name != "")
	{
		int warnings = 0;
		int index = -1;
		std::string current_file_name = "";

		std::ifstream source_list(source_list_name.c_str(), std::ios_base::in);
		if(!source_list.is_open())
		{
			std::cout << "Could not get list:" << std::endl;
			std::cout << "\t" << source_list_name << std::endl;

			target_file->cd();
			target_ntpl->Write();
			target_file->Write();
			target_file->Close();
			if(b)return 0;
			return 1;
		}

		while(true)
		{
			index++;

			if(index < starting_index)continue;
			if(starting_index < stopping_index and index >= stopping_index)break;

			if(source_list.bad())break;

			source_list >> current_file_name;

			if(source_list.eof())break;

			i = UpdateNtuple(target_ntpl, current_file_name, (warnings < max_warnings or max_warnings < 0));
			if(i == 0)b = true;
			warnings += i;
		}

		source_list.close();
	}

	target_file->cd();
	target_ntpl->Write();
	target_file->Write();
	target_file->Close();
	if(b)return 0;
	return 1;
}

int TDatasetParser::RecreateTarget()
{
	if(MakeTarget())return 1;
	return UpdateTarget();
}
