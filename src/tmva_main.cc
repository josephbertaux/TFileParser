#include "tmva_train.h"

int main()
{
	TMVA_Trainer tmva_trainer;

	tmva_trainer.file_name_training =	"/scratch/halstead/j/jbertau/BD0_training_data_Sept_2022/training_data.root";

	tmva_trainer.file_name_prompt =		"/scratch/halstead/m/mstojano/d0outputs/Dfinder_prompt/miniAOD/output.root";
	tmva_trainer.file_name_nonprompt =	"/scratch/halstead/m/mstojano/d0outputs/Dfinder_nonPrompt/miniAOD/output.root";

	tmva_trainer.tree_names_prompt =	std::vector<std::string>{"Dfinder/ntDkpi"};
	tmva_trainer.tree_names_nonprompt =	std::vector<std::string>{"Dfinder/ntDkpi"};
	tmva_trainer.tree_names_data =		std::vector<std::string>{"Dfinder/ntDkpi"};

	tmva_trainer.MAX_SIZE = 30000;

	tmva_trainer.raw_size_var_prompt =	"Dsize";
	tmva_trainer.raw_size_var_nonprompt =	"Dsize";
	tmva_trainer.raw_size_var_data =	"Dsize";

	tmva_trainer.AddRawVar("mass",				"F");
	tmva_trainer.AddRawVar("pT",				"F");
	tmva_trainer.AddRawVar("centrality",			"I");
	tmva_trainer.AddRawVar("y",				"F");
	tmva_trainer.AddRawVar("VtxProb",			"F");
	tmva_trainer.AddRawVar("3DDecayLengthSignificance",	"F");
	tmva_trainer.AddRawVar("3DDecayLength",			"F");
	tmva_trainer.AddRawVar("3DPointingAngle",		"F");
	tmva_trainer.AddRawVar("Dtrk1Pt",			"F");
	tmva_trainer.AddRawVar("Dtrk2Pt",			"F");
	tmva_trainer.AddRawVar("xyDCASignificanceDaugther1",	"F");
	tmva_trainer.AddRawVar("xyDCASignificanceDaugther2",	"F");
	tmva_trainer.AddRawVar("zDCASignificanceDaugther1",	"F");
	tmva_trainer.AddRawVar("zDCASignificanceDaugther2",	"F");
	tmva_trainer.AddRawVar("Dgen",				"I");

	tmva_trainer.raw_cut_formulas_prompt =		std::vector<std::string>{"Dgen==23333"};
	tmva_trainer.raw_cut_formulas_nonprompt =	std::vector<std::string>{"Dgen==23333"};
	tmva_trainer.raw_cut_formulas_data =		std::vector<std::string>{"Dgen!=23333", "Dgen!=23344"};

	tmva_trainer.mass_formula_prompt =	"mass";
	tmva_trainer.mass_formula_nonprompt =	"mass";
	tmva_trainer.mass_formula_data =	"mass";

	tmva_trainer.AddTrainingVar("y",		"y");
	tmva_trainer.AddTrainingVar("VP",		"VtxProb");
	tmva_trainer.AddTrainingVar("logDLS",		"log10(3DDecayLengthSignificance)");
	tmva_trainer.AddTrainingVar("logDL",		"log10(3DDecayLength)");
	tmva_trainer.AddTrainingVar("PA",		"3DPointingAngle");
	tmva_trainer.AddTrainingVar("pT_D1",		"Dtrk1Pt");
	tmva_trainer.AddTrainingVar("pT_D2",		"Dtrk2Pt");
	tmva_trainer.AddTrainingVar("xyDCAS_D1",	"xyDCASignificanceDaugther1");
	tmva_trainer.AddTrainingVar("xyDCAS_D2",	"xyDCASignificanceDaugther2");
	tmva_trainer.AddTrainingVar("zDCAS_D1",		"zDCASignificanceDaugther1");
	tmva_trainer.AddTrainingVar("zDCAS_D2",		"zDCASignificanceDaugther2");

	tmva_trainer.AddTrainingCutVar("pT", "pT");
	tmva_trainer.AddTrainingCutVar("cent", "centrality");


	tmva_trainer.InitFiles();
	tmva_trainer.InitArgs();

	std::cout << std::endl;
	tmva_trainer.ReadPrompt();

	//do a mass fit

	return 0;
}
