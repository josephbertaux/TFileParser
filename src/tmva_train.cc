#include "tmva_train.h"

void TMVA_Trainer::MakeTrees()
{
	if(!signal_tree)
	{
		training_branch_values.clear();

		signal_tree = new TTree("signal_tree", "signal_tree");
		for(uint u = 0; u < training_expressions.size(); u++)
		{
			training_branch_values.push_back(0.0);
	
			signal_tree->Branch(Form("expr%d", u));
		}
	}

	
}
