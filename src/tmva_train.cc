#include "tmva_train.h"

void TMVA_Trainer::InitArgs()
{
	//===	RooRealVars for raw data branches	===//
	if(raw_vars_prompt.size() == 0)
	{
		std::cout << "Member 'raw_vars_prompt' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(raw_vars_nonprompt.size() == 0)
	{
		std::cout << "Member 'raw_vars_nonprompt' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(raw_vars_data.size() == 0)
	{
		std::cout << "Member 'raw_vars_data' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}

	uint u;
	raw_args_prompt.clear();
	for(u = 0; u < raw_vars_prompt.size(); u++)
	{
		if(raw_vars_prompt[u] == "")continue;
		raw_args_prompt.addOwned(*(new RooRealVar(raw_vars_prompt[u].c_str(), raw_vars_prompt[u].c_str(), 0.0, -DBL_MAX, DBL_MAX)));
	}
	raw_args_nonprompt.clear();
	for(u = 0; u < raw_vars_nonprompt.size(); u++)
	{
		if(raw_vars_nonprompt[u] == "")continue;
		raw_args_nonprompt.addOwned(*(new RooRealVar(raw_vars_nonprompt[u].c_str(), raw_vars_nonprompt[u].c_str(), 0.0, -DBL_MAX, DBL_MAX)));
	}
	raw_args_data.clear();
	for(u = 0; u < raw_vars_data.size(); u++)
	{
		if(raw_vars_data[u] == "")continue;
		raw_args_data.addOwned(*(new RooRealVar(raw_vars_data[u].c_str(), raw_vars_data[u].c_str(), 0.0, -DBL_MAX, DBL_MAX)));
	}
	//===	RooRealVars for raw data branches	===//

	//===	RooFormulaVars for cuts on raw data	===//
	raw_cuts_prompt.clear();
	for(u = 0; u < raw_cut_formulas_prompt.size(); u++)
	{
		if(raw_cut_formulas_prompt[u] == "")continue;
		raw_cuts_prompt.addOwned(*(new RooFormulaVar(Form("Cut_Prompt_%d", u), raw_cut_formulas_prompt[u].c_str(), raw_args_prompt)));
	} 
	raw_cuts_nonprompt.clear();
	for(u = 0; u < raw_cut_formulas_nonprompt.size(); u++)
	{
		if(raw_cut_formulas_nonprompt[u] == "")continue;
		raw_cuts_nonprompt.addOwned(*(new RooFormulaVar(Form("Cut_Nonprompt_%d", u), raw_cut_formulas_nonprompt[u].c_str(), raw_args_nonprompt)));
	} 
	raw_cuts_data.clear();
	for(u = 0; u < raw_cut_formulas_data.size(); u++)
	{
		if(raw_cut_formulas_data[u] == "")continue;
		raw_cuts_data.addOwned(*(new RooFormulaVar(Form("Cut_Data_%d", u), raw_cut_formulas_data[u].c_str(), raw_args_data)));
	} 
	//===	RooFormulaVars for cuts on raw data	===//

	//===	RooFormulaVars for the mass		===//
	if(mass_name == "")
	{
		std::cout << "Member 'mass_name' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(mass_formula_prompt == "")
	{
		std::cout << "Member 'mass_formula_prompt' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(mass_formula_nonprompt == "")
	{
		std::cout << "Member 'mass_formula_nonprompt' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(mass_formula_data == "")
	{
		std::cout << "Member 'mass_formula_data' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}

	mass_prompt = std::make_shared<RooFormulaVar>(RooFormulaVar(mass_name.c_str(), mass_formula_prompt.c_str(), raw_args_prompt));
	mass_nonprompt = std::make_shared<RooFormulaVar>(RooFormulaVar(mass_name.c_str(), mass_formula_nonprompt.c_str(), raw_args_nonprompt));
	mass_data = std::make_shared<RooFormulaVar>(RooFormulaVar(mass_name.c_str(), mass_formula_data.c_str(), raw_args_data));
	//===	RooFormulaVars for the mass		===//

	//===	Training Var Expressions		===//
	training_args_prompt.clear();
	for(u = 0; u < training_var_formulas_prompt.size(); u++)
	{
		if(training_vars[u] == "")continue;
		if(training_var_formulas_prompt[u] == "")continue;
		training_args_prompt.addOwned(*(new RooFormulaVar(training_vars[u].c_str(), training_var_formulas_prompt[u].c_str(), raw_args_prompt)));
	}
	training_args_nonprompt.clear();
	for(u = 0; u < training_var_formulas_nonprompt.size(); u++)
	{
		if(training_vars[u] == "")continue;
		if(training_var_formulas_nonprompt[u] == "")continue;
		training_args_nonprompt.addOwned(*(new RooFormulaVar(training_vars[u].c_str(), training_var_formulas_nonprompt[u].c_str(), raw_args_nonprompt)));
	}
	training_args_data.clear();
	for(u = 0; u < training_var_formulas_data.size(); u++)
	{
		if(training_vars[u] == "")continue;
		if(training_var_formulas_data[u] == "")continue;
		training_args_data.addOwned(*(new RooFormulaVar(training_vars[u].c_str(), training_var_formulas_data[u].c_str(), raw_args_data)));
	}
	//===	Training Var Expressions	===//

	//===	Training CutVar Expressions	===//
	training_cut_args_prompt.clear();
	for(u = 0; u < training_cut_var_formulas_prompt.size(); u++)
	{
		if(training_cut_vars[u] == "")continue;
		if(training_cut_var_formulas_prompt[u] == "")continue;
		training_cut_args_prompt.addOwned(*(new RooFormulaVar(training_cut_vars[u].c_str(), training_cut_var_formulas_prompt[u].c_str(), raw_args_prompt)));
	}
	training_cut_args_nonprompt.clear();
	for(u = 0; u < training_cut_var_formulas_nonprompt.size(); u++)
	{
		if(training_vars[u] == "")continue;
		if(training_cut_var_formulas_nonprompt[u] == "")continue;
		training_cut_args_nonprompt.addOwned(*(new RooFormulaVar(training_cut_vars[u].c_str(), training_cut_var_formulas_nonprompt[u].c_str(), raw_args_nonprompt)));
	}
	training_args_data.clear();
	for(u = 0; u < training_cut_var_formulas_data.size(); u++)
	{
		if(training_vars[u] == "")continue;
		if(training_cut_var_formulas_data[u] == "")continue;
		training_cut_args_data.addOwned(*(new RooFormulaVar(training_cut_vars[u].c_str(), training_cut_var_formulas_data[u].c_str(), raw_args_data)));
	}
	//===	Training CutVar Expressions	===//
}

void TMVA_Trainer::InitFiles()
{
	if(file_name_training == "")
	{
		std::cout << "Member 'file_name_training' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}

	TFile* file_training = TFile::Open(file_name_training.c_str(), "RECREATE");

	TTree* tree_prompt = new TTree(tree_name_prompt.c_str(), tree_name_prompt.c_str());
	TTree* tree_nonprompt = new TTree(tree_name_nonprompt.c_str(), tree_name_nonprompt.c_str());
	TTree* tree_background = new TTree(tree_name_background.c_str(), tree_name_background.c_str());

	tree_prompt->SetDirectory(file_training);
	tree_nonprompt->SetDirectory(file_training);
	tree_background->SetDirectory(file_training);

	uint u;
	float f;
	tree_prompt->Branch(mass_name.c_str(), &f);
	tree_nonprompt->Branch(mass_name.c_str(), &f);
	tree_background->Branch(mass_name.c_str(), &f);
	for(u = 0; u < training_vars.size(); u++)
	{
		tree_prompt->Branch(training_vars[u].c_str(), &f);
		tree_nonprompt->Branch(training_vars[u].c_str(), &f);
		tree_background->Branch(training_vars[u].c_str(), &f);
	}
	for(u = 0; u < training_cut_vars.size(); u++)
	{
		tree_prompt->Branch(training_cut_vars[u].c_str(), &f);
		tree_nonprompt->Branch(training_cut_vars[u].c_str(), &f);
		tree_background->Branch(training_cut_vars[u].c_str(), &f);
	}

	tree_prompt->ResetBranchAddresses();
	tree_nonprompt->ResetBranchAddresses();
	tree_background->ResetBranchAddresses();

	file_training->Write();
	file_training->Close();
}

void TMVA_Trainer::ReadPrompt()
{
	if(file_name_training == "")
	{
		std::cout << "Member 'file_name_training' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	TFile* file_training = TFile::Open(file_name_training.c_str(), "UPDATE");
	if(!file_training)
	{
		std::cout << "Could not get file:" << std::endl;
		std::cout << "\t" << file_name_training << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(file_training->IsZombie())
	{
		std::cout << "File:" << std::endl;
		std::cout << "\t" << file_name_training << std::endl;
		std::cout << "Is zombie" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}

	TTree* tree_prompt= (TTree*)file_training->Get(tree_name_prompt.c_str());
	if(!tree_prompt)
	{
		std::cout << "Could not get tree:" << std::endl;
		std::cout << "\t" << tree_name_prompt << std::endl;
		std::cout << "From file:" << std::endl;
		std::cout << "\t" << file_name_training << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}

	tree_prompt->SetDirectory(file_training);

	if(file_name_prompt != "")
	{
		ProcessRawFile
		(
			file_name_prompt, tree_names_prompt, tree_prompt,
			raw_size_var_prompt,
			raw_args_prompt, raw_types_prompt,
			raw_cuts_prompt,
			mass_prompt, training_args_prompt, training_cut_args_prompt,
			true
		);
	}

	if(file_name_prompt == "" and file_list_name_prompt != "")
	{
		std::string current_file_name = "";

		int MAX_WARNINGS = 30;
		int warnings = 0;
	
		std::ifstream file_list(file_list_name_prompt.c_str(), std::ios_base::in);
		while(file_list.is_open())
		{
			if(file_list.bad())break;
	
			file_list >> current_file_name;
	
			if(file_list.eof())break;
	
			warnings += ProcessRawFile
			(
				file_name_prompt, tree_names_prompt, tree_prompt,
				raw_size_var_prompt,
				raw_args_prompt, raw_types_prompt,
				raw_cuts_prompt,
				mass_prompt, training_args_prompt, training_cut_args_prompt,
				warnings < MAX_WARNINGS
			);
		}
		file_list.close();
	}

	file_training->cd();
	tree_prompt->Write();
	file_training->Write();
	file_training->Close();
}




















int TMVA_Trainer::ProcessRawFile
(
	std::string raw_file_name, std::vector<std::string> raw_tree_names, TTree* tree,
	std::string raw_size_var,
	RooArgList& raw_args, std::vector<std::string> raw_types,
	RooArgList& raw_cuts,
	std::shared_ptr<RooFormulaVar>& mass_arg, RooArgList& training_args, RooArgList& training_cut_args,
	bool v
)
{
	TFile* raw_file = TFile::Open(raw_file_name.c_str(), "READ");
	if(!raw_file)
	{
		if(v)
		{
			std::cout << std::endl;
			std::cout << "Could not get file:" << std::endl;
			std::cout << "\t" << raw_file_name << std::endl;
			std::cout << "Exiting" << std::endl;
		}

		return 1;
	}

	if(raw_file->IsZombie())
	{
		std::cout << std::endl;
		std::cout << "File:" << std::endl;
		std::cout << "\t" << raw_file_name << std::endl;
		std::cout << "Is zombie" << std::endl;
		std::cout << "Exiting" << std::endl;

		return 1;
	}

	uint u;
	TTree* raw_tree = (TTree*)raw_file->Get(raw_tree_names[0].c_str());
	if(!raw_tree)
	{
		if(v)
		{
			std::cout << std::endl;
        	        std::cout << "Could not get tree:" << std::endl;
			std::cout << "\t" << raw_tree_names[0] << std::endl;
			std::cout << "From file:" << std::endl;
        	        std::cout << "\t" << raw_file_name << std::endl;
        	        std::cout << "Exiting" << std::endl;
			std::cout << std::endl;
		}

		return 1;
	}
	TTree* frnd = 0x0;
	for(u = 1; u < raw_tree_names.size(); u++)
	{
		frnd = (TTree*)raw_file->Get(raw_tree_names[u].c_str());
		if(!frnd)
		{
			if(v)
			{
				std::cout << std::endl;
	        	        std::cout << "Could not get tree:" << std::endl;
				std::cout << "\t" << raw_tree_names[u] << std::endl;
				std::cout << "From file:" << std::endl;
	        	        std::cout << "\t" << raw_file_name << std::endl;
	        	        std::cout << "Exiting" << std::endl;
				std::cout << std::endl;
			}
	
			return 1;
		}

		raw_tree->AddFriend(frnd);
	}

	raw_tree->ResetBranchAddresses();
	
	int raw_size = 1;
	if(raw_size_var != "")
	{
		if(!raw_tree->GetBranch(raw_size_var.c_str()))
		{
			if(v)
			{
				std::cout << std::endl;
	        	        std::cout << "Could not get branch:" << std::endl;
				std::cout << "\t" << raw_size_var << std::endl;
	        	        std::cout << "From trees in file:" << std::endl;
	        	        std::cout << "\t" << raw_file_name << std::endl;
	        	        std::cout << "Exiting" << std::endl;
				std::cout << std::endl;
			}
		}
		raw_tree->SetBranchStatus(raw_size_var.c_str(), 1);
		raw_tree->SetBranchAddress(raw_size_var.c_str(), &raw_size);
	}

	std::vector<void*> ptrs = {};
	for(u = 0; u < raw_args.getSize(); u++)
	{
		if(!raw_tree->GetBranch(raw_args[u].GetName()))
		{
			if(v)
			{
				std::cout << std::endl;
	        	        std::cout << "Could not get branch:" << std::endl;
				std::cout << "\t" << raw_args[u].GetName() << std::endl;
	        	        std::cout << "From trees in file:" << std::endl;
	        	        std::cout << "\t" << raw_file_name << std::endl;
	        	        std::cout << "Exiting" << std::endl;
				std::cout << std::endl;
			}

			return 1;
		}
		ptrs.push_back(MakeVoidAddress(raw_types[u], raw_tree, raw_args[u].GetName()));
	}

	tree->ResetBranchAddresses();

	float mass;
	if(!tree->GetBranch(mass_arg->GetName()))
	{
		if(v)
		{
			std::cout << std::endl;
        	        std::cout << "Could not get branch:" << std::endl;
			std::cout << "\t" << mass_arg->GetName() << std::endl;
        	        std::cout << "In passed TTree" << std::endl;
        	        std::cout << "Exiting" << std::endl;
			std::cout << std::endl;
		}

		return 1;
	}
	tree->SetBranchStatus(mass_arg->GetName(), 1);
	tree->SetBranchAddress(mass_arg->GetName(), &mass);

	float training_vars[training_args.getSize()];
	for(u = 0; u < training_args.getSize(); u++)
	{
		if(!tree->GetBranch(training_args[u].GetName()))
		{
			if(v)
			{
				std::cout << std::endl;
	        	        std::cout << "Could not get branch:" << std::endl;
				std::cout << "\t" << training_args[u].GetName() << std::endl;
	        	        std::cout << "In passed TTree" << std::endl;
	        	        std::cout << "Exiting" << std::endl;
				std::cout << std::endl;
			}

			return 1;
		}
		tree->SetBranchStatus(training_args[u].GetName(), 1);
		tree->SetBranchAddress(training_args[u].GetName(), &(training_vars[u]));
	}

	float training_cut_vars[training_cut_args.getSize()];
	for(u = 0; u < training_cut_args.getSize(); u++)
	{
		if(!tree->GetBranch(training_cut_args[u].GetName()))
		{
			if(v)
			{
				std::cout << std::endl;
	        	        std::cout << "Could not get branch:" << std::endl;
				std::cout << "\t" << training_cut_args[u].GetName() << std::endl;
	        	        std::cout << "In passed TTree" << std::endl;
	        	        std::cout << "Exiting" << std::endl;
				std::cout << std::endl;
			}

			return 1;
		}
		tree->SetBranchStatus(training_cut_args[u].GetName(), 1);
		tree->SetBranchAddress(training_cut_args[u].GetName(), &(training_cut_vars[u]));
	}

	int i;
	Long64_t n;
	bool b;

	for(n = 0; n < raw_tree->GetEntriesFast(); n++)
	{
		raw_tree->GetEntry(n);
		for(i = 0; i < raw_size; i++)
		{
			for(u = 0; u < raw_args.getSize(); u++)
			{
				((RooRealVar*)&(raw_args[u]))->setVal(ReadVoidAddress(raw_types[u], ptrs[u], i));
			}

			b = true;
			for(u = 0; u < raw_cuts.getSize(); u++)
			{
				if(((RooFormulaVar*)&(raw_cuts[u]))->getValV() == 0.0)b = false;
			}
			if(!b)continue;

			mass = mass_arg->getValV();
			for(u = 0; u < training_args.getSize(); u++)
			{
				training_vars[u] = ((RooFormulaVar*)&(training_args[u]))->getValV();
			}
			for(u = 0; u < training_cut_args.getSize(); u++)
			{
				training_cut_vars[u] = ((RooFormulaVar*)&(training_cut_args[u]))->getValV();
			}

			tree->Fill();
		}
	}

	raw_tree->ResetBranchAddresses();
	tree->ResetBranchAddresses();

	raw_file->Close();

	return 0;
}

void TMVA_Trainer::AddRawVar(std::string formula_prompt, std::string type_prompt, std::string formula_nonprompt, std::string type_nonprompt, std::string formula_data, std::string type_data)
{
	raw_vars_prompt.push_back(formula_prompt);
	raw_vars_nonprompt.push_back(formula_nonprompt);
	raw_vars_data.push_back(formula_data);

	raw_types_prompt.push_back(type_prompt);
	raw_types_nonprompt.push_back(type_nonprompt);
	raw_types_data.push_back(type_data);
}

void TMVA_Trainer::AddTrainingVar(std::string var, std::string formula_prompt, std::string formula_nonprompt, std::string formula_data)
{
	training_vars.push_back(var);

	training_var_formulas_prompt.push_back(formula_prompt);
	training_var_formulas_nonprompt.push_back(formula_nonprompt);
	training_var_formulas_data.push_back(formula_data);
}

void TMVA_Trainer::AddTrainingCutVar(std::string var, std::string formula_prompt, std::string formula_nonprompt, std::string formula_data)
{
	training_cut_vars.push_back(var);

	training_cut_var_formulas_prompt.push_back(formula_prompt);
	training_cut_var_formulas_nonprompt.push_back(formula_nonprompt);
	training_cut_var_formulas_data.push_back(formula_data);
}

template <typename T>
void* TMVA_Trainer::MakeVoidAddressTemplate(TTree* tree, std::string branch)
{
	void* ptr = (void*)(new T[MAX_SIZE]);

	if(!tree)return ptr;
	if(!tree->GetBranch(branch.c_str()))return ptr;

	tree->SetBranchStatus(branch.c_str(), 1);
	tree->SetBranchAddress(branch.c_str(), (T*)ptr);

	return ptr;
}

template <typename T>
T TMVA_Trainer::ReadVoidAddressTemplate(void* ptr, int i)
{
	if(!ptr)return 0.0;
	if(!(0 <= i and i < MAX_SIZE))return 0.0;

	return ((T*)ptr)[i];
}

void* TMVA_Trainer::MakeVoidAddress(std::string type, TTree* tree, std::string branch)
{
	std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c){return std::tolower(c);});

	if(type == "i" or type == "int")return MakeVoidAddressTemplate<int>(tree, branch);
	if(type == "f" or type == "float")return MakeVoidAddressTemplate<float>(tree, branch);
	if(type == "d" or type == "double")return MakeVoidAddressTemplate<double>(tree, branch);

	return (void*)nullptr;
}

float TMVA_Trainer::ReadVoidAddress(std::string type, void* ptr, int i)
{
	std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c){return std::tolower(c);});

	if(type == "i" or type == "int")return ReadVoidAddressTemplate<int>(ptr, i);
	if(type == "f" or type == "float")return ReadVoidAddressTemplate<float>(ptr, i);
	if(type == "d" or type == "double")return ReadVoidAddressTemplate<double>(ptr, i);

	return 0.0;
}

/*
void TMVA_Trainer::Train()
{
	if(signal_file_name == "")
	{
		std::cout << "Member 'signal_file_name' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}
	if(background_file_name == "")
	{
		std::cout << "Member 'background_file_name' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}
	if(tmva_output_file_name == "")
	{
		std::cout << "Member 'tmva_output_file_name' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}

	if(training_expressions.size() == 0)
	{
		std::cout << "Member 'training_expressions' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}
	if(training_branches.size() == 0)
	{
		std::cout << "Member 'training_branches' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}

	TFile* signal_file = TFile::Open(signal_file_name.c_str(), "R");
	if(signal_file == 0x0)
	{
		std::cout << "File:" << std::endl;
		std::cout << "\t" << signal_file_name << std::endl;
		std::cout << "Not found" << std::endl;
		std::cout << "Try running 'MakeTrees()' first" << std::endl;

		return;
	}
	TTree* signal_tree = (TTree*)signal_file->Get(signal_tree_name.c_str());
	if(signal_tree == 0x0)
	{
		std::cout << "Tree:" << std::endl;
		std::cout << "\t" << signal_tree_name << std::endl;
		std::cout << "Not found in file:" << std::endl;
		std::cout << signal_file_name << std::endl;
		std::cout << "Try running 'MakeTrees()' first" << std::endl;

		return;
	}
	
	TFile* background_file = TFile::Open(background_file_name.c_str(), "R");
	if(background_file == 0x0)
	{
		std::cout << "File:" << std::endl;
		std::cout << "\t" << background_file_name << std::endl;
		std::cout << "Not found" << std::endl;
		std::cout << "Try running 'MakeTrees()' first" << std::endl;

		return;
	}
	TTree* background_tree = (TTree*)background_file->Get(background_tree_name.c_str());
	if(background_tree == 0x0)
	{
		std::cout << "Tree:" << std::endl;
		std::cout << "\t" << background_tree_name << std::endl;
		std::cout << "Not found in file:" << std::endl;
		std::cout << background_file_name << std::endl;
		std::cout << "Try running 'MakeTrees()' first" << std::endl;

		return;
	}

	TFile* tmva_output_file = TFile::Open(tmva_output_file_name.c_str(), "RECREATE");
	TMVA::Factory* factory = new TMVA::Factory(factory_name.c_str(), tmva_output_file, factory_options.c_str());
	TMVA::DataLoader* data_loader = new TMVA::DataLoader(data_loader_name.c_str());

	uint u;

	size_t pos = std::string::npos;
	std::string temp = "";

	data_loader->AddSignalTree(signal_tree, 1.0);
	data_loader->AddBackgroundTree(background_tree, 1.0);
	for(u = 0; u < training_expressions.size(); u++)
	{
		pos = training_expressions[u].find(func_deliminator);

		if(pos == std::string::npos)
		{
			data_loader->AddVariable(training_expressions[u].c_str(), training_expressions[u].c_str(), "", 'F');
		}
		else
		{
			temp = training_expressions[u].substr(0, pos);
			data_loader->AddVariable(temp.c_str(), temp.c_str(), "", 'F');
		}
	}
	data_loader->PrepareTrainingAndTestTree("", "", ""); //make third option member string

	factory->BookMethod(data_loader, TMVA::Types::kBDT, "BDT", ""); //make third option member string
	
	factory->TrainAllMethods();
	factory->TestAllMethods();
	factory->EvaluateAllMethods();

	tmva_output_file->Close();

	delete factory;
	delete data_loader;
}

void TMVA_Trainer::MiscDebug()
{
	//nothing atm
}
*/
//...
//===		~public member functions		===//
