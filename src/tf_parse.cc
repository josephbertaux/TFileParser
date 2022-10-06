#include "TFPWrapper.h"

#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	TFPWrapper tfpw;

	std::vector<std::string> args;
	for(int i = 0; i < argc; i++)
	{
		args.push_back(argv[i]);
	}
	args.erase(args.begin());

	if(tfpw.ShowHelp(args))return 0;
	if(tfpw.Config(args[0], true))return 0;

	//move these to a TFPWrapper::CommandConfig(std::vector<std::string>) method potentially
	int return_val = 0;
	if(args.size() != 1 and args.size() != 2 and args.size() != 3 and args.size() != 5)
	{
		std::cout << "\tNumber of command line args (" << args.size() << ") not recognized for overloaded methods" << std::endl;
		return_val = 1;
		//goto label
		
	}
	if(args.size() == 2)
	{
		if(tfpw.tfp.SetTargetFile(args[1]))return_val = 1;
	}
	if(args.size() == 3)
	{
		if(tfpw.tfp.SetTargetFile(args[1] + "/target.root"))return_val = 1;
		if(tfpw.tfp.SetSourceList(args[2]))return_val = 1;
	}
	if(args.size() == 5)
	{
		if(tfpw.tfp.SetSourceList(args[2]))return_val = 1;
		if(tfpw.SetStartingIndex(std::vector<std::string>{args[3]}))return_val = 1;
		if(tfpw.SetStoppingIndex(std::vector<std::string>{args[4]}))return_val = 1;
		
		if(tfpw.tfp.SetTargetFile(args[1] + "/target" + std::to_string(tfpw.tfp.GetStartingIndex()) + "_" +  std::to_string(tfpw.tfp.GetStoppingIndex()) + ".root"))return_val = 1;

	}

	if(return_val)
	{
		std::cout << "\tProblem configuring with command line args:" << std::endl;
		std::cout << "\t" << args[0];
		for(uint u = 1; u < args.size(); u++)
		{
			std::cout << " " << args[u];
		}
		std::cout << std::endl;

		return 0;
	}
	//...
	
	//Check on tfpw.tfp's members...
	std::cout << "Member values:" << std::endl;
	std::cout << tfpw.tfp.GetTargetFile() << std::endl;
	std::cout << tfpw.tfp.GetTargetNtpl() << std::endl;
	std::cout << tfpw.tfp.GetMaxSize() << std::endl;
	std::cout << "Done" << std::endl;

	return 0;
}
