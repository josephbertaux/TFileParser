#include "tmva_train.h"

int main()
{
	TMVA_Trainer tmva_trainer;

	tmva_trainer.file_name_training = "/home/jbertau/Data/BD0_Training/Dum/training_file.root";

	tmva_trainer.file_name_prompt = "/home/jbertau/Data/BD0_Training/Dum/prompt0.root";
	tmva_trainer.tree_names_prompt = std::vector<std::string>{"tree"};

	tmva_trainer.raw_vars_prompt =		std::vector<std::string>{"m", "alpha", "beta", "gamma", "delta"};
	tmva_trainer.raw_vars_nonprompt =	std::vector<std::string>{"m", "alpha", "beta", "gamma", "delta"};
	tmva_trainer.raw_vars_data =		std::vector<std::string>{"m", "alpha", "beta", "gamma", "delta"};

	tmva_trainer.raw_cut_formulas_prompt =	std::vector<std::string>{"alpha+beta>3.0"};

	tmva_trainer.mass_formula_prompt =	"m";
	tmva_trainer.mass_formula_nonprompt =	"m";
	tmva_trainer.mass_formula_data =	"m";

	tmva_trainer.AddTrainingVar("e", "alpha-beta");
	tmva_trainer.AddTrainingVar("f", "gamma*delta");
	tmva_trainer.AddTrainingVar("g", "beta+gamma");
	tmva_trainer.AddTrainingVar("h", "alpha/delta");

	tmva_trainer.AddTrainingVar("pt", "alpha/beta");
	tmva_trainer.AddTrainingVar("cent", "gamma*delta");


	tmva_trainer.InitFiles();
	tmva_trainer.InitArgs();
	tmva_trainer.ReadPrompt();

	return 0;
}
