#include "tmva_main.h"

int main()
{
	TMVA_Trainer tmva_trainer;

	tmva_trainer.training_file_name = "/home/jbertau/Data/BD0_Training/Dum/training_file.root";

	tmva_trainer.prompt_file_name = "/home/jbertau/Data/BD0_Training/Dum/prompt0.root";
	tmva_trainer.prompt_tree_names = std::vector<std::string>{"tree"};

	tmva_trainer.raw_prompt_vars =	std::vector<std::string>{"m", "alpha", "beta", "gamma", "delta"};
	tmva_trainer.raw_nonprompt_vars =	std::vector<std::string>{"m", "alpha", "beta", "gamma", "delta"};
	tmva_trainer.raw_data_vars =	std::vector<std::string>{"m", "alpha", "beta", "gamma", "delta"};

	tmva_trainer.prompt_mass_formula =	"m";
	tmva_trainer.nonprompt_mass_formula =	"m";
	tmva_trainer.data_mass_formula =	"m";

	tmva_trainer.AddTrainingVar("e", "alpha-beta");
	tmva_trainer.AddTrainingVar("f", "gamma*delta");
	tmva_trainer.AddTrainingVar("g", "beta+gamma");
	tmva_trainer.AddTrainingVar("h", "alpha/delta");

	tmva_trainer.prompt_cut_formulas = std::vector<std::string>{"alpha+beta>3.0"};

	tmva_trainer.InitFiles();
	tmva_trainer.InitArgs();
	tmva_trainer.ReadPrompt();

	return 0;
}
