#include "tmva_train.h"

void tmva_train()
{
	TMVA::Tools::Instance();

	TFile* input = 0x0;

	input = TFile::Open(TRAINING_FILE_NAME, "r");

	if(!input)return;
}
