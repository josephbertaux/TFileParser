#include "tmva_train.h"

void TMVA_Trainer::InitArgs()
{
	//=== raw expressions ===//
	if(raw_prompt_vars.size() == 0)
	{
		std::cout << "Member 'raw_prompt_vars' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(raw_nonprompt_vars.size() == 0)
	{
		std::cout << "Member 'raw_nonprompt_vars' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(raw_data_vars.size() == 0)
	{
		std::cout << "Member 'raw_data_vars' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}

	uint u;
	raw_prompt_args.clear();
	for(u = 0; u < raw_prompt_vars.size(); u++)
	{
		if(raw_prompt_vars[u] == "")continue;
		raw_prompt_args.addOwned(*(new RooRealVar(raw_prompt_vars[u].c_str(), raw_prompt_vars[u].c_str(), 0.0, -DBL_MAX, DBL_MAX)));
	}
	raw_nonprompt_args.clear();
	for(u = 0; u < raw_nonprompt_vars.size(); u++)
	{
		if(raw_nonprompt_vars[u] == "")continue;
		raw_nonprompt_args.addOwned(*(new RooRealVar(raw_nonprompt_vars[u].c_str(), raw_nonprompt_vars[u].c_str(), 0.0, -DBL_MAX, DBL_MAX)));
	}
	raw_data_args.clear();
	for(u = 0; u < raw_data_vars.size(); u++)
	{
		if(raw_data_vars[u] == "")continue;
		raw_data_args.addOwned(*(new RooRealVar(raw_data_vars[u].c_str(), raw_data_vars[u].c_str(), 0.0, -DBL_MAX, DBL_MAX)));
	}
	//=== raw expressions ===//

	//=== Training Expressions and Mass	===//
	if(mass_name == "")
	{
		std::cout << "Member 'mass_name' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(prompt_mass_formula == "")
	{
		std::cout << "Member 'prompt_mass_formula' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(nonprompt_mass_formula == "")
	{
		std::cout << "Member 'nonprompt_mass_formula' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(data_mass_formula == "")
	{
		std::cout << "Member 'data_mass_formula' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}

	training_prompt_args.clear();
	training_prompt_args.addOwned(*(new RooFormulaVar(mass_name.c_str(), prompt_mass_formula.c_str(), raw_prompt_args)));
	for(u = 0; u < training_prompt_formulas.size(); u++)
	{
		if(training_vars[u] == "")continue;
		if(training_prompt_formulas[u] == "")continue;
		training_prompt_args.addOwned(*(new RooFormulaVar(training_vars[u].c_str(), training_prompt_formulas[u].c_str(), raw_prompt_args)));
	}
	training_nonprompt_args.clear();
	training_nonprompt_args.addOwned(*(new RooFormulaVar(mass_name.c_str(), nonprompt_mass_formula.c_str(), raw_nonprompt_args)));
	for(u = 0; u < training_nonprompt_formulas.size(); u++)
	{
		if(training_vars[u] == "")continue;
		if(training_nonprompt_formulas[u] == "")continue;
		training_nonprompt_args.addOwned(*(new RooFormulaVar(training_vars[u].c_str(), training_nonprompt_formulas[u].c_str(), raw_nonprompt_args)));
	}
	training_data_args.clear();
	training_data_args.addOwned(*(new RooFormulaVar(mass_name.c_str(), data_mass_formula.c_str(), raw_data_args)));
	for(u = 0; u < training_data_formulas.size(); u++)
	{
		if(training_vars[u] == "")continue;
		if(training_data_formulas[u] == "")continue;
		training_data_args.addOwned(*(new RooFormulaVar(training_vars[u].c_str(), training_data_formulas[u].c_str(), raw_data_args)));
	}
	//=== Training Expressions and Mass	===//

	//===	Cuts	===//
	prompt_cuts.clear();
	for(u = 0; u < prompt_cut_formulas.size(); u++)
	{
		if(prompt_cut_formulas[u] == "")continue;
		prompt_cuts.addOwned(*(new RooFormulaVar(Form("Prompt_Cut_%d", u), prompt_cut_formulas[u].c_str(), raw_prompt_args)));
	} 
	nonprompt_cuts.clear();
	for(u = 0; u < nonprompt_cut_formulas.size(); u++)
	{
		if(nonprompt_cut_formulas[u] == "")continue;
		nonprompt_cuts.addOwned(*(new RooFormulaVar(Form("NonPrompt_Cut_%d", u), nonprompt_cut_formulas[u].c_str(), raw_nonprompt_args)));
	}
	//=== Cuts	===//
}

void TMVA_Trainer::InitFiles()
{
	if(training_file_name == "")
	{
		std::cout << "Member 'training_file_name' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}

	TFile* training_file = TFile::Open(training_file_name.c_str(), "RECREATE");

	TTree* prompt_tree = new TTree(prompt_tree_name.c_str(), prompt_tree_name.c_str());
	TTree* nonprompt_tree = new TTree(nonprompt_tree_name.c_str(), nonprompt_tree_name.c_str());
	TTree* background_tree = new TTree(background_tree_name.c_str(), background_tree_name.c_str());

	prompt_tree->SetDirectory(training_file);
	nonprompt_tree->SetDirectory(training_file);
	background_tree->SetDirectory(training_file);

	uint u;
	double d;
	prompt_tree->Branch(mass_name.c_str(), &d);
	nonprompt_tree->Branch(mass_name.c_str(), &d);
	background_tree->Branch(mass_name.c_str(), &d);
	for(u = 0; u < training_vars.size(); u++)
	{
		prompt_tree->Branch(training_vars[u].c_str(), &d);
		nonprompt_tree->Branch(training_vars[u].c_str(), &d);
		background_tree->Branch(training_vars[u].c_str(), &d);
	}

	prompt_tree->ResetBranchAddresses();
	nonprompt_tree->ResetBranchAddresses();
	background_tree->ResetBranchAddresses();

	training_file->Write();
	training_file->Close();
}

void TMVA_Trainer::ReadPrompt()
{
	if(training_file_name == "")
	{
		std::cout << "Member 'training_file_name' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	TFile* training_file = TFile::Open(training_file_name.c_str(), "UPDATE");
	if(!training_file)
	{
		std::cout << "Could not get file:" << std::endl;
		std::cout << "\t" << training_file_name << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(training_file.IsZombie())
	{
		std::cout << "File:" << std::endl;
		std::cout << "\t" << training_file_name << std::endl;
		std::cout << "Is zombie" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	TTree* prompt_tree = (TTree*)training_file->Get(prompt_tree_name.c_str());
	if(!prompt_tree)
	{
		std::cout << "Could not get tree:" << std::endl;
		std::cout << "\t" << prompt_tree_name << std::endl;
		std::cout << "From file:" << std::endl;
		std::cout << "\t" << training_file_name << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(prompt_file_name != "")
	{
		ProcessRawFile(prompt_file_name, prompt_tree_names, prompt_tree, raw_prompt_args, training_prompt_args, prompt_cuts, true);
	}

	if(prompt_file_name == "" and prompt_file_list_name != "")
	{
		std::string current_file_name = "";

		int MAX_WARNINGS = 30;
		int warnings = 0;
	
		std::ifstream file_list(prompt_file_list_name.c_str(), std::ios_base::in);
		while(file_list.is_open())
		{
			if(file_list.bad())break;
	
			file_list >> current_file_name;
	
			if(file_list.eof())break;
	
			warnings += ProcessRawFile(prompt_file_name, prompt_tree_names, prompt_tree, raw_prompt_args, training_prompt_args, prompt_cuts, (warnings < MAX_WARNINGS));
		}

		file_list.close();
	}

	training_file->Write();
	training_file->Close();
}

void TMVA_Trainer::ReadNonprompt()
{
	if(training_file_name == "")
	{
		std::cout << "Member 'training_file_name' not set" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	TFile* training_file = TFile::Open(training_file_name.c_str(), "UPDATE");
	if(!training_file)
	{
		std::cout << "Could not get file:" << std::endl;
		std::cout << "\t" << training_file_name << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(training_file.IsZombie())
	{
		std::cout << "File:" << std::endl;
		std::cout << "\t" << training_file_name << std::endl;
		std::cout << "Is zombie" << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	TTree* nonprompt_tree = (TTree*)training_file->Get(nonprompt_tree_name.c_str());
	if(!nonprompt_tree)
	{
		std::cout << "Could not get tree:" << std::endl;
		std::cout << "\t" << nonprompt_tree_name << std::endl;
		std::cout << "From file:" << std::endl;
		std::cout << "\t" << training_file_name << std::endl;
		std::cout << "Exiting" << std::endl;

		return;
	}
	if(nonprompt_file_name != "")
	{
		ProcessRawFile(nonprompt_file_name, nonprompt_tree_names, nonprompt_tree, raw_nonprompt_args, training_nonprompt_args, nonprompt_cuts, true);
	}

	if(nonprompt_file_name == "" and nonprompt_file_list_name != "")
	{
		std::string current_file_name = "";

		int MAX_WARNINGS = 30;
		int warnings = 0;
	
		std::ifstream file_list(nonprompt_file_list_name.c_str(), std::ios_base::in);
		while(file_list.is_open())
		{
			if(file_list.bad())break;
	
			file_list >> current_file_name;
	
			if(file_list.eof())break;
	
			warnings += ProcessRawFile(nonprompt_file_name, nonprompt_tree_names, nonprompt_tree, raw_nonprompt_args, training_nonprompt_args, nonprompt_cuts, (warnings < MAX_WARNINGS));
		}

		file_list.close();
	}

	training_file->Write();
	training_file->Close();
}

void TMVA_Trainer::ReadMC()
{
	ReadPrompt();
	ReadNonprompt();
}


















int TMVA_Trainer::ProcessRawFile(std::string raw_file_name, std::vector<std::string> raw_tree_names, TTree* tree, RooArgList& raw_args, RooArgList& args, RooArgList& cuts, bool v)
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

	if(raw_file.IsZombie())
	{
		std::cout << std::endl;
		std::cout << "File:" << std::endl;
		std::cout << "\t" << raw_file_name << std::endl;
		std::cout << "Is zombie" << std::cout << std::endl;
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

	double raw_vals[raw_args.getSize()];
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

		raw_tree->SetBranchStatus(raw_args[u].GetName(), 1);
		raw_tree->SetBranchAddress(raw_args[u].GetName(), &(raw_vals[u]));
	}

	double vals[args.getSize()];
	for(u = 0; u < raw_args.getSize(); u++)
	{
		if(!tree->GetBranch(args[u].GetName()))
		{
			if(v)
			{
				std::cout << std::endl;
	        	        std::cout << "Could not get branch:" << std::endl;
				std::cout << "\t" << args[u].GetName() << std::endl;
	        	        std::cout << "In passed TTree" << std::endl;
	        	        std::cout << "Exiting" << std::endl;
				std::cout << std::endl;
			}

			return 1;
		}

		tree->SetBranchStatus(args[u].GetName(), 1);
		tree->SetBranchAddress(args[u].GetName(), &(vals[u]));
	}

	Long64_t n;
	bool b;

	for(n = 0; n < raw_tree->GetEntriesFast(); n++)
	{
		raw_tree->GetEntry(n);

		for(u = 0; u < raw_args.getSize(); u++)
		{
			((RooRealVar*)&(raw_args[u]))->setVal(raw_vals[u]);
		}

		for(u = 0; u < args.getSize(); u++)
		{
			vals[u] = ((RooFormulaVar*)&(args[u]))->getValV();
		}

		b = true;
		for(u = 0; u < cuts.getSize(); u++)
		{
			if(((RooFormulaVar*)&(cuts[u]))->getValV() == 0.0)b = false;
		}

		if(b)tree->Fill();
	}

	return 0;
}

void TMVA_Trainer::AddTrainingVar(std::string var, std::string prompt_formula, std::string nonprompt_formula, std::string data_formula)
{
	training_vars.push_back(var);

	training_prompt_formulas.push_back(prompt_formula);
	training_nonprompt_formulas.push_back(nonprompt_formula);
	training_data_formulas.push_back(data_formula);
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
