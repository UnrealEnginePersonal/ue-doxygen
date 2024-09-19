//
// Created by User on 19/09/2024.
//

#ifndef RUNOPTIONS_H
#define RUNOPTIONS_H

#include <string>

struct RunOptions
{
	std::string configName;
	std::string traceName;
	bool shortList = false;
	bool traceTiming = false;
	bool updateConfig = false;
	bool quiet = false;
	bool genConfig = false;

	RunOptions(std::string configName_, std::string traceName_, const bool shortList_, const bool traceTiming_, const bool updateConfig_, const bool quiet_, const bool genConfig_);
};

#endif //RUNOPTIONS_H
