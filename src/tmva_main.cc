#include "tmva_main.h"

int main()
{
	TMVA_Trainer tmva_trainer;

	std::cout << "hello" << std::endl;
	
	tmva_trainer.signal_list_file_name = "/home/jbertau/Data/BD0_Training/Dum/my_dum_signal_files.list";
	tmva_trainer.training_tree_name = "tree";

	tmva_trainer.training_branches.push_back("a");
	tmva_trainer.training_branches.push_back("b");

	tmva_trainer.training_expressions.push_back("c=a+b");
	tmva_trainer.training_expressions.push_back("d=a*b");

	tmva_trainer.MakeTrees();
	tmva_trainer.MiscDebug();

	return 0;
}
