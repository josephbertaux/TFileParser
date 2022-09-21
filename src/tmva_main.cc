#include "tmva_main.h"

int main()
{
	TMVA_Trainer tmva_trainer;

	tmva_trainer.signal_file_name = "/home/jbertau/Data/BD0_Training/Dum/signal_file.root";
	tmva_trainer.background_file_name = "/home/jbertau/Data/BD0_Training/Dum/background_file.root";
	tmva_trainer.tmva_output_file_name = "/home/jbertau/Data/BD0_Training/Dum/tmva_output.root";

	tmva_trainer.signal_file_list_name = "/home/jbertau/Data/BD0_Training/Dum/my_dum_signal_files.list";
	tmva_trainer.background_file_list_name = "/home/jbertau/Data/BD0_Training/Dum/my_dum_background_files.list";

	tmva_trainer.signal_training_tree_name = "tree";
	tmva_trainer.background_training_tree_name = "tree";

	//Be careful here--make sure you include all the original branches the expressions you want depend on
	//For example, we want training variables "c" and "d", and these depend on branches "a" and "b"
	tmva_trainer.training_expressions.push_back("c:=a+b");
	tmva_trainer.training_expressions.push_back("d:=a-b");

	//we have to manually specify we want branches "a" and "b" here
	tmva_trainer.training_branches.push_back("a");
	tmva_trainer.training_branches.push_back("b");

	tmva_trainer.MakeTrees();
	tmva_trainer.Train();

	return 0;
}
