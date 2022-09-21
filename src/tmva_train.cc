#include "tmva_train.h"

//===		protected member functions		===//
void TMVA_Trainer::MakeTree(std::string file_name, std::string tree_name, std::string file_list_name, std::string training_tree_name)
{
	//file_name is the name of the file to write the processed tree to
	//tree_name is the name of the processed tree
	//file_list_name is the name of the .list file to read in raw, unprocessed files from
	//training_tree_name is the name of the tree each file in the file list is expected to contain

	uint u;

	std::vector<double> training_vals = {};
	RooArgList training_args;
	for(u = 0; u < training_branches.size(); u++)
	{
		training_vals.push_back(0.0);
		training_args.add(*(new RooRealVar(training_branches[u].c_str(), training_branches[u].c_str(), 0.0, -DBL_MAX, DBL_MAX)));
	}

	size_t pos = std::string::npos;
	std::string temp1 = "";
	std::string temp2 = "";

	std::vector<double> expression_vals = {};
	std::vector<RooFormulaVar> expression_args = {};
	for(u = 0; u < training_expressions.size(); u++)
	{
		pos = training_expressions[u].find(func_deliminator);

		if(pos == std::string::npos)
		{
			temp1 = training_expressions[u];
			temp2 = training_expressions[u];
		}
		else
		{
			temp1 = training_expressions[u].substr(0, pos);
			temp2 = training_expressions[u].substr(pos + func_deliminator.length());
		}

		expression_vals.push_back(0.0);
		expression_args.push_back(RooFormulaVar(temp1.c_str(), temp2.c_str(), training_args));
	}

	TFile* file = TFile::Open(file_name.c_str(), "RECREATE");
	TTree* tree = new TTree(tree_name.c_str(), tree_name.c_str());
	for(u = 0; u < training_expressions.size(); u++)
	{
		pos = training_expressions[u].find(func_deliminator);

		if(pos == std::string::npos)
		{
			temp1 = training_expressions[u];
		}
		else
		{
			temp1 = training_expressions[u].substr(0, pos);
		}

		tree->Branch(temp1.c_str(), &expression_vals[u]);
		tree->SetBranchStatus(temp1.c_str(), 1);
		tree->SetBranchAddress(temp1.c_str(), &(expression_vals[u]));
	}
	tree->SetDirectory(file);

	std::string current_file_name = "";
	TFile* current_file = 0x0;
	TTree* current_tree = 0x0;
	Long64_t n;

	int MAX_WARNINGS = 100;
	int warnings = 0;
	bool missing_branch_flag = false;

	std::ifstream file_list(file_list_name.c_str(), std::ios_base::in);
	while(file_list.is_open())
	{
		if(file_list.bad())break;

		file_list >> current_file_name;

		if(file_list.eof())break;

		current_file = TFile::Open(current_file_name.c_str(), "R");
		if(!current_file and warnings < MAX_WARNINGS)
		{
			std::cout << std::endl;
			std::cout << "Could not open file:" << std::endl;
			std::cout << "\t" << file_name << std::endl;
			std::cout << "continuing" << std::endl;
			std::cout << std::endl;

			warnings++;

			continue;
		}

		current_tree = (TTree*)current_file->Get(training_tree_name.c_str());
		if(!current_tree and warnings < MAX_WARNINGS)
		{
			std::cout << std::endl;
                        std::cout << "Could not get tree:" << std::endl;
			std::cout << "\t" << training_tree_name << std::endl;
			std::cout << "From file:" << std::endl;
                        std::cout << "\t" << file_name << std::endl;
                        std::cout << "continuing" << std::endl;
			std::cout << std::endl;

			warnings++;

			continue;
		}

		missing_branch_flag = false;
		current_tree->SetBranchStatus("*", 0);
		for(u = 0; u < training_vals.size(); u++)
		{
			if(!current_tree->GetBranch(training_branches[u].c_str()))
			{
				std::cout << std::endl;
				std::cout << "\tCould not get branch:" << std::endl;
				std::cout << "\t" << training_branches[u] << std::endl;

				missing_branch_flag = true;
			}
			current_tree->SetBranchStatus(training_branches[u].c_str(), 1);
			current_tree->SetBranchAddress(training_branches[u].c_str(), &(training_vals[u]));
		}
		if(missing_branch_flag and warnings < MAX_WARNINGS)
		{
			std::cout << "Tree:" << std::endl;
			std::cout << "\t" << training_tree_name << std::endl;
			std::cout << "In file:" << std::endl;
			std::cout << "\t" << file_name << std::endl;
			std::cout << "Is missing training branches" << std::endl;
			std::cout << "Continuing..." << std::endl;
			std::cout << std::endl;

			warnings++;

			continue;
		}

		if(!(warnings < MAX_WARNINGS))
		{
			std::cout << std::endl;
			std::cout << "Maximum number of warnings (" << MAX_WARNINGS << ") printed" << std::endl;
			std::cout << "Suppressing further warnings" << std::endl;
		}

		for(n = 0; n < current_tree->GetEntriesFast(); n++)
		{
			current_tree->GetEntry(n);

			for(u = 0; u < training_vals.size(); u++)
			{
				((RooRealVar*)&training_args[u])->setVal(training_vals[u]);
			}

			for(u = 0; u < expression_args.size(); u++)
			{
				expression_vals[u] = expression_args[u].getValV();
			}

			tree->Fill();
		}
	}

	file_list.close();

	file->Write();
	file->Close();

	for(u = 0; u < training_vals.size(); u++)
	{
		delete &training_args[u];
	}
}
//===		~protected member functions		===//

//===		public member functions			===//
void TMVA_Trainer::MakeTrees()
{
	if(signal_file_name == "")
	{
		std::cout << "Member 'signal_file_name' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}
	if(signal_file_list_name == "")
	{
		std::cout << "Member 'signal_file_list_name' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}
	if(signal_training_tree_name == "")
	{
		std::cout << "Member 'signal_training_tree_name' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}

	if(background_file_name == "")
	{
		std::cout << "Member 'background_file_name' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}
	if(background_file_list_name == "")
	{
		std::cout << "Member 'background_file_list_name' isn't set" << std::endl;
		std::cout << "Returning" << std::endl;

		return;
	}
	if(background_training_tree_name == "")
	{
		std::cout << "Member 'background_training_tree_name' isn't set" << std::endl;
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

	MakeTree(signal_file_name, signal_tree_name, signal_file_list_name, signal_training_tree_name);
	MakeTree(background_file_name, background_tree_name, background_file_list_name, background_training_tree_name);
}

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

//...
//===		~public member functions		===//
