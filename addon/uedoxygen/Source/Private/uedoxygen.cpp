//
// Created by Kasper de Bruin on 19/09/2024.
//

#include "uedoxygen.h"

#include <config.h>
#include <doxygen.h>

namespace doxygen::unreal
{
	void uedoxygen::runDoxygen(const std::string& configFilePath)
	{

		const QCString configName = configFilePath.c_str();
		const QCString traceName = "doxygen";

		const RunOptions runOptions = RunOptions(
			configName, traceName, false, false,
			Config::CompareMode::Full, false, false, false);

		initDoxygen();
		parseMainArgs(runOptions);

		checkConfiguration();
		adjustConfiguration();

		parseInput();
		generateOutput();
	}
} // unreal
// doxygen
