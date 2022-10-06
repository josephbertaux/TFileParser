#ifndef TFP_WRAPPER_H
#define TFP_WRAPPER_H

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "TFileParser.h"

class TFPWrapper
{
public:
	typedef int(TFPWrapper::*TFPW_MMBR_FUNC)(std::vector<std::string>);
	typedef std::tuple<std::string, TFPWrapper*, TFPW_MMBR_FUNC> TFPW_TPL;
	typedef std::map<std::string, TFPW_TPL> TFPW_MAP;

protected:
	TFPW_MAP tfpw_map;
	void MakeTFPWMap();

	std::vector<std::string> cmmnts = {"//", "#"};
	std::vector<std::string> prunes = {" ", "\t", "\n"};

	std::string opening_sym = "{";
	std::string closing_sym = "}";
	std::string delim = ";";
public:
	TFileParser tfp;

	TFPWrapper();
	~TFPWrapper();

	int Implement(std::string, std::vector<std::string>);
	int Config(std::string, bool);

	//Reimplementations of TFileParser member functions through the Wrapper
	int ShowHelp(std::vector<std::string>);

	int SetTargetFile(std::vector<std::string>);
	int SetTargetNtpl(std::vector<std::string>);
	int SetSourceName(std::vector<std::string>);
	int SetSourceList(std::vector<std::string>);
	int SetSizeVar(std::vector<std::string>);

	int SetMaxSize(std::vector<std::string>);
	int SetStartingIndex(std::vector<std::string>);
	int SetStoppingIndex(std::vector<std::string>);
	int SetMaxWarnings(std::vector<std::string>);

	int AddSourceTree(std::vector<std::string>);
	int AddSourceVar(std::vector<std::string>);
	int AddSourceCut(std::vector<std::string>);
	int AddTargetVar(std::vector<std::string>);
	int AddTargetCut(std::vector<std::string>);
};

#endif
