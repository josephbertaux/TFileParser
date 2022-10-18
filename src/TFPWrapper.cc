#include "TFPWrapper.h"

//private member functions

void TFPWrapper::MakeTFPWMap()
{
	tfpw_map.clear();
	std::string s = "\n";

	tfpw_map["-h"] =			TFPW_TPL
						{
							s +
							"./tf_parse ...\n" +
							"\tParses ROOT TFiles, TTrees and recreates their contents after applying transformations and cuts\n" +
							"\tSeveral overloads are available:\n" +
							"\t\t./tf_parse [config_file_name]\n" +
							"\t\t./tf_parse [config_file_name] [target_fullpath_name]\n" +
							"\t\t./tf_parse [config_file_name] [target_dir] [file_list_name]\n" +
							"\t\t./tf_parse [config_file_name] [target_dir] [file_list_name] [start] [stop]\n" +
							"\tCommand line arguments are prioritized over config file lines\n" +
							"\tSee 'SetTargetFile' to see how this is configured in the above overloads\n" +
							"\tSee 'SetSourceList' to see how this is configured in the above overloads\n" +
							"\tThe lines of the config file have the following format:\n" +
							"\t\tfunc" + opening_sym + "args" + delim + "..." + closing_sym + "\n" +
							"\tWhere 'func' can take the following values:\n"
							"\n" +
							"\t\tSetTargetFile\n" +
							"\t\tSetTargetNtpl\n" +
							"\t\tSetSourceName\n" +
							"\t\tSetSourceList\n" +
							"\t\t*SetStartingIndex\n" +
							"\t\t*SetStoppingIndex\n" +
							"\t\t*SetMaxWarnings\n" +
							"\t\tAddSourceTree\n" +
							"\t\t*AddSizeVar\n" +
							"\t\tAddSourceVar\n" +
							"\t\t*AddSourceCut\n" +
							"\t\tAddTargetVar\n" +
							"\t\t*AddTargetCut\n" +
							"\n" +
							"\tFuncs marked with * are optional to include in the config file\n" +
							"\tOnly one of 'SetSourceName' or 'SetSourceList' needs to be specified\n" +
							"\tFor more information about any 'func', do:\n"
							"\t\t./tf_parse -h func\n" +
							s,
							this,
							&TFPWrapper::ShowHelp
						};

	tfpw_map["SetTargetFile"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"SetTargetFile(std::string)\n" +
							"\tSpecifies the name of the target file to be created\n" +
							"\tIn overloads with [target_fullpath_name], this will be called with\n" +
							"\t\t[target_fullpath_name]\n" +
							"\t\tAfter the config file has been read\n" +
							"\tIn overloads with [target_dir], this will be called with\n" +
							"\t\t[target_dir] + 'target' + [start] + '_' + [stop] + '.root'\n" +
							"\t\tAfter the config file has been read,\n" +
							"\t\tand after [start] and [stop] have been configured by command line args if applicable\n" +
							s,
							this,
							&TFPWrapper::SetTargetFile
						};

	tfpw_map["SetTargetNtpl"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"SetTargetNtpl(std::string)\n" +
							"\tSpecifies the name of the target TNtuple to be created in the target file\n" +
							s,
							this,
							&TFPWrapper::SetTargetNtpl
						};

	tfpw_map["SetSourceName"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"SetSourceName(std::string)\n" +
							"\tSpecifies the fullpath name of a singular .root file to read data from\n" +
							"\tDoes not need to be specified if SetSourceList is set\n" +
							s,
							this,
							&TFPWrapper::SetSourceName
						};

	tfpw_map["SetSourceList"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"SetSourceList(std::string)\n" +
							"\tSpecifies the fullpath name of a file list that contains the fullpath names of .root files to read from\n" +
							"\tDoes not need to be specified if SetSourceName is set\n" +
							"\tIn overloads with [file_list_name], this will be called with\n" +
							"\t\t[file_list_name]\n" +
							"\tAfter the config file has been read\n" +
							s,
							this,
							&TFPWrapper::SetSourceList
						};


	tfpw_map["SetStartingIndex"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"SetStartingIndex(int)\n" +
							"\tIn the case of reading from a file list (SetSourceList is called)\n" +
							"\tSpecifies the line (inclusive) to start reading in files from the list\n" +
							"\tIn overloads with [start], this will be called with\n" +
							"\t\t[start]\n" +
							"\tAfter the config file has been read\n" +
							s,
							this,
							&TFPWrapper::SetStartingIndex
						};

	tfpw_map["SetStoppingIndex"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"SetStoppingIndex(int)\n" +
							"\tIn the case of reading from a file list (SetSourceList is called)\n" +
							"\tSpecifies the line (exclusive) to stop reading in files from the list\n" +
							"\tIn overloads with [stop], this will be called with\n" +
							"\t\t[stop]\n" +
							"\tAfter the config file has been read\n" +
							s,
							this,
							&TFPWrapper::SetStoppingIndex
						};

	tfpw_map["SetMaxWarnings"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"SetMaxWarnings(int)\n" +
							"\tIn the case of reading from a file list (SetSourceList is called)\n" +
							"\tSpecifies the most errors to output while reading from the file list\n" +
							"\tThis helps keep output relatively short in the case of redundant errors\n" +
							"\ti.e., a typo in a branch name\n" +
							"\tBy default is set to '-1'; setting a negative value will always include error output\n" +
							s,
							this,
							&TFPWrapper::SetMaxWarnings
						};

	tfpw_map["AddSourceTree"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"AddSourceTree(std::string)\n" +
							"\tAdds the name of a tree to look for in source files\n" +
							"\tAll found trees are friended, and then source vars (branches) are searched for\n" +
							"\tThe program doesn't end if a tree isn't found; only if a source var (branch) isn't found\n" +
							"\tThus can be used effectively as a list of possible trees to search for in the source file list\n" +
							s,
							this,
							&TFPWrapper::AddSourceTree
						};

	tfpw_map["AddSizeVar"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"AddSizeVar(std::string)\n" +
							"\tAdds the name of a branch to look for in source files\n" +
							"\tWhich is of type int and could specify the sizes of other args\n" +
							s,
							this,
							&TFPWrapper::AddSizeVar
						};

	tfpw_map["AddSourceVar"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"AddSourceVar(std::string) or AddSourceVar(std::string, std::string)\n" +
							"\tdepending on the number of args\n" +
							"\tAdds the name of a branch to look for in source files\n" +
							"\tWith one argument, that argument is the branch name\n" +
							"\tWith two arguments, the second argument is its case-independent optional integral type:\n" +
							"\t\t'int' or 'i' for int\n" +
							"\t\t'float' or 'f' for float\n" +
							"\t\t'double' or 'd' for double\n" +
							"\tAssumes the branch is type float if unspecified\n" +
							s,
							this,
							&TFPWrapper::AddSourceVar
						};

	tfpw_map["AddSourceCut"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"AddSourceCut(std::string) or AddSourceCut(std::string, std::string)\n" +
							"\tdepending on the number of args\n" +
							"\tAdds a cut expression in terms of source args\n" +
							"\tWith one argument, that argument is the cut expression and its name is made generic\n" +
							"\tWith two arguments, the first argument is the name to give the cut\n" +
							"\tWith two arguments, the second argument is the cut expression source vars\n" +
							s,
							this,
							&TFPWrapper::AddSourceCut
						};

	tfpw_map["AddTargetVar"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"AddTarget(std::string) or AddSourceCut(std::string, std::string)\n" +
							"\tdepending on the number of args\n" +
							"\tAdds a variable to compute for the target file in terms of source args\n" +
							"\tWith one argument, that argument is the both the name of branch in the recreated data\n" +
							"\tIt is assumed to be identically equal to a source var of the same name\n" +
							"\tWith two arguments, the second argument is the cut expression in terms of source vars\n" +
							s,
							this,
							&TFPWrapper::AddTargetVar
						};

	tfpw_map["AddTargetCut"] =		TFPW_TPL
						{
							s +
							"Implements\n" +
							"AddTargetCut(std::string) or AddSourceCut(std::string, std::string)\n" +
							"\tdepending on the number of args\n" +
							"\tAdds a cut expression in terms of target args\n" +
							"\tWith one argument, that argument is the cut expression and its name is made generic\n" +
							"\tWith two arguments, the first argument is the name to give the cut\n" +
							"\tWith two arguments, the second argument is the cut expression\n" +
							s,
							this,
							&TFPWrapper::AddTargetCut
						};
}

//public member functions

TFPWrapper::TFPWrapper()
{
	MakeTFPWMap();
	tfp.Clear();
}

TFPWrapper::~TFPWrapper()
{
	//Do nothing
}

int TFPWrapper::Implement(std::string func, std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "TFPWrapper::Implement(std::string fucn, std::vector<std::string> args):" << std::endl;

	TFPW_MAP::const_iterator itr = tfpw_map.find(func);
	if(itr == tfpw_map.end())
	{
		output_str << "\tCould not find function '" << func << "'" << std::endl;
		return_val = 1;
		goto label;
	}
	if((std::get<1>(itr->second)->*std::get<2>(itr->second))(args))
	{
		output_str << "\tErrors implementing function '" << func << "'" << std::endl;
		return_val = 1;
		goto label;
	}
	
	label:
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::Config(std::string config_file_name, bool v)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "TFPWrapper::Config(std::string config_file_name):" << std::endl;

	uint u;
	size_t pos;
	int line_num = 0;
	std::string line_str = "";
	std::string config_str = "";
	std::ifstream config_file;

	std::string func;
	std::vector<std::string> args;

	if(config_file_name == "")
	{
		output_str << "\tPassed argument 'config_file_name' was empty string" << std::endl;
		return_val = 1;
		goto label;
	}

	config_file.open(config_file_name, std::ifstream::in);
	if(!config_file.is_open())
	{
		output_str << "\tCould not get list:" << std::endl;
		output_str << "\t" << config_file_name << std::endl;
		return_val = 1;
		goto label;
	}

	output_str << "\tIn file:" << std::endl;
	output_str << "\t" << config_file_name << std::endl;

	config_str = "";
	line_str = "";
	while(true)
	{
		while(true)
		{
			if(config_file.bad())
			{
				output_str << "\tBad bit set" << std::endl;
				return_val = 1;
				goto label;
			}

			config_str += line_str;

			pos = config_str.find(closing_sym);
			if(pos != std::string::npos)
			{
				config_str = config_str.substr(0, pos + closing_sym.length());
				line_str = config_str.substr(pos + closing_sym.length());
				break;
			}

			std::getline(config_file, line_str);
			for(u = 0; u < cmmnts.size(); u++)
			{
				line_str = line_str.substr(0, line_str.find(cmmnts[u]));
			}
			line_num++;

			if(config_file.eof())
			{
				if(return_val)output_str << std::endl;
				output_str << "Reached end of config file" << std::endl;
				goto label;
			}
		}

		for(u = 0; u < prunes.size(); u++)
		{
			pos = config_str.find(prunes[u]);
			while(pos < config_str.length())
			{
				config_str.erase(pos, prunes[u].length());
				pos = config_str.find(prunes[u]);
			}
		}

		if(config_str.find(opening_sym) == std::string::npos)
		{
			output_str << "\tNo opening '" << opening_sym << "' found (line " << line_num << ")" << std::endl;
			return_val = 1;
		}
		if(config_str.find(closing_sym) == std::string::npos)
		{
			output_str << "\tNo closing '" << closing_sym << "' found (line " << line_num << ")" << std::endl;
			return_val = 1;
		}
		func = config_str.substr(0, config_str.find(opening_sym));
		config_str = config_str.substr(config_str.find(opening_sym) + opening_sym.length());
		config_str = config_str.substr(0, config_str.find(closing_sym));

		args.clear();
		pos = config_str.find(delim);
		while(pos < config_str.length())
		{
			args.push_back(config_str.substr(0, pos));
			config_str = config_str.substr(pos + delim.length());
			pos = config_str.find(delim);
		}
		if(config_str != "")args.push_back(config_str);

		if(v)
		{
			std::cout << "line " << line_num << ":" << std::endl;
			std::cout << "\t" << func;
			for(u = 0; u < args.size(); u++)
			{
				std::cout << " " << args[u];
			}
			std::cout << std::endl;
		}

		if(Implement(func, args))
		{
			std::cout << "\tline " << line_num << ":" << std::endl;
			std::cout << "\t" << func;
			for(u = 0; u < args.size(); u++)
			{
				std::cout << " " << args[u];
			}
			std::cout << std::endl;
			std::cout << std::endl;

			output_str << "\tProblem with config block ending on line: " << line_num << std::endl;
			return_val = 1;
		}

		config_str = "";
	}

	label:
	if(config_file.is_open())config_file.close();
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::ShowHelp(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "TFPWrapper::ShowHelp(std::vector<std::string> args):" << std::endl;

	bool flag = false;
	TFPW_MAP::const_iterator itr;

	if(args.size() >= 1)if(args[0] != "-h")goto label;

	return_val = 1;
	itr = tfpw_map.find("-h");
	if(args.size() >= 2)itr = tfpw_map.find(args[1]);
	if(itr == tfpw_map.end())
	{
		if(args.size() >= 2)
		{
			output_str << "\tCould not find requested help option:" << std::endl;
			output_str << "\t'" << args[1] << "'" << std::endl;
			output_str << "\tTry" << std::endl;
			output_str << "\t\t ./tf_parse -h" << std::endl;
			output_str << "\tfor a full list of options" << std::endl;
		}
		flag = true;
		goto label;
	}
	std::cout << std::get<0>(itr->second);

	label:
	output_str << std::ends;
	if(flag)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::SetTargetFile(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetTargetFile(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	return_val = tfp.SetTargetFile(args[0]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::SetTargetNtpl(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetTargetNtpl(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	return_val = tfp.SetTargetNtpl(args[0]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::SetSourceName(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetSourceName(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	return_val = tfp.SetSourceName(args[0]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::SetSourceList(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetSourceList(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	return_val = tfp.SetSourceList(args[0]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::SetStartingIndex(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetStartingIndex(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	if(args[0] == "")
	{
		output_str << "\tPassed argument args[0] is empty" << std::endl;
	}
	try
	{
		return_val = tfp.SetStartingIndex(std::stoi(args[0]));
	}
	catch(const std::invalid_argument&)
	{
		output_str << "\tstd::stoi failed to deduce '" << args[0] << "' as int" << std::endl;
		return_val = 1;
		goto label;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::SetStoppingIndex(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetStoppingIndex(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	if(args[0] == "")
	{
		output_str << "\tPassed argument args[0] is empty" << std::endl;
	}
	try
	{
		return_val = tfp.SetStoppingIndex(std::stoi(args[0]));
	}
	catch(const std::invalid_argument&)
	{
		output_str << "\tstd::stoi failed to deduce '" << args[0] << "' as int" << std::endl;
		return_val = 1;
		goto label;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::SetMaxWarnings(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetMaxWarnings(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	if(args[0] == "")
	{
		output_str << "\tPassed argument args[0] is empty" << std::endl;
	}
	try
	{
		return_val = tfp.SetMaxWarnings(std::stoi(args[0]));
	}
	catch(const std::invalid_argument&)
	{
		output_str << "\tstd::stoi failed to deduce '" << args[0] << "' as int" << std::endl;
		return_val = 1;
		goto label;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::AddSourceTree(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddSourceVar(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	//could make this a loop, or the original function take a vector
	return_val = tfp.AddSourceTree(args[0]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::AddSizeVar(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddSizeVar(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	return_val = tfp.AddSizeVar(args[0]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::AddSourceVar(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddSourceVar(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	if(args.size() == 1)return_val = tfp.AddSourceVar(args[0]);
	if(args.size() == 2)return_val = tfp.AddSourceVar(args[0], args[1]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::AddSourceCut(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddSourceCut(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	if(args.size() == 1)return_val = tfp.AddSourceCut(args[0]);
	if(args.size() == 2)return_val = tfp.AddSourceCut(args[0], args[1]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::AddTargetVar(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddTargetVar(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	if(args.size() == 1)return_val = tfp.AddTargetVar(args[0]);
	if(args.size() == 2)return_val = tfp.AddTargetVar(args[0], args[1]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int TFPWrapper::AddTargetCut(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddTargetCut(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	if(args.size() == 1)return_val = tfp.AddTargetCut(args[0]);
	if(args.size() == 2)return_val = tfp.AddTargetCut(args[0], args[1]);

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}
