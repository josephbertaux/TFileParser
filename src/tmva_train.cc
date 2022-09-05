#include "tmva_train.h"

void TMVA_Trainer::MakeTrees()
{
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
		pos = training_expressions[u].find("=");

		if(pos == std::string::npos)
		{
			std::cout << "Bad expression" << std::endl;
			std::cout << "Expression " << u << " does not contain '='" << std::endl;
			std::cout << "Aborting" << std::endl;

			return;
		}

		temp1 = training_expressions[u].substr(0, pos);
		temp2 = training_expressions[u].substr(pos + 1);

		expression_vals.push_back(0.0);
		expression_args.push_back(RooFormulaVar(temp1.c_str(), temp2.c_str(), training_args));
	}



	std::string file_name = "";
	TFile* current_file = 0x0;
	TTree* current_tree = 0x0;
	bool missing_branch_flag = false;
	Long64_t n;

	if(!signal_tree)
	{
		signal_tree = new TTree("signal_tree", "signal_tree");
		for(u = 0; u < training_expressions.size(); u++)
		{
			pos = training_expressions[u].find("=");
	
			if(pos == std::string::npos)continue;

			temp1 = training_expressions[u].substr(0, pos);
			signal_tree->Branch(temp1.c_str(), &expression_vals[u]); 
		}
		signal_tree->ResetBranchAddresses();
	}

	for(u = 0; u < expression_vals.size(); u++)
	{
		pos = training_expressions[u].find("=");
	
		if(pos == std::string::npos)continue;

		temp1 = training_expressions[u].substr(0, pos);

		signal_tree->SetBranchStatus(temp1.c_str(), 1);
		signal_tree->SetBranchAddress(temp1.c_str(), &(expression_vals[u]));
	}

	std::ifstream file_list(signal_list_file_name.c_str(), std::ios_base::in);
	while(file_list.is_open())
	{
		if(file_list.bad())break;

		file_list >> file_name;

		if(file_list.eof())break;

		current_file = TFile::Open(file_name.c_str(), "r");
		if(!current_file)
		{
			std::cout << std::endl;
			std::cout << "Could not open file:" << std::endl;
			std::cout << "\t" << file_name << std::endl;
			std::cout << "continuing" << std::endl;
			std::cout << std::endl;

			continue;
		}

		current_tree = (TTree*)current_file->Get(training_tree_name.c_str());
		if(!current_tree)
		{
			std::cout << std::endl;
                        std::cout << "Could not get tree:" << std::endl;
			std::cout << "\t" << training_tree_name << std::endl;
			std::cout << "From file:" << std::endl;
                        std::cout << "\t" << file_name << std::endl;
                        std::cout << "continuing" << std::endl;
			std::cout << std::endl;

			continue;
		}

		missing_branch_flag = false;
		current_tree->SetBranchStatus("*", 0);
		for(u = 0; u < training_vals.size(); u++)
		{
			if(!current_tree->GetBranch(training_branches[u].c_str()))
			{
				std::cout << std::endl;
				std::cout << "Could not get branch:" << std::endl;
				std::cout << "\t" << training_branches[u] << std::endl;
				std::cout << "From tree:" << std::endl;
				std::cout << "\t" << training_tree_name << std::endl;
				std::cout << "In file:" << std::endl;
				std::cout << "\t" << file_name << std::endl;
				std::cout << "continuing" << std::endl;
				std::cout << std::endl;

				missing_branch_flag = true;
			}
			current_tree->SetBranchStatus(training_branches[u].c_str(), 1);
			current_tree->SetBranchAddress(training_branches[u].c_str(), &(training_vals[u]));
		}
		if(missing_branch_flag)continue;

		std::cout << std::endl;
		std::cout << "In file:" << std::endl;
		std::cout << "\t" << file_name << std::endl;
		std::cout << std::endl;

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

			signal_tree->Fill();
		}
	}

	file_list.close();

	for(u = 0; u < training_vals.size(); u++)
	{
		delete &training_args[u];
	}


	//copy-paste but with background tree, or generalize this slightly

}

void TMVA_Trainer :: MiscDebug()
{
	if(!signal_tree)return;
	signal_tree->Print();
	signal_tree->Scan();
}

//...
