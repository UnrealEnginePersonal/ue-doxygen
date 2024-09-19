//
// Created by Kasper de Bruin on 19/09/2024.
//

#include "uedoxygen.h"

#include <doxygen.h>
#include <Models/RunOptions.h>

namespace doxygen::unreal
{
	void uedoxygen::runDoxygen(const RunOptions& runOptions)
	{
		initDoxygen();
		parseMainArgs(runOptions);

		checkConfiguration();
		adjustConfiguration();

		parseInput();
		generateOutput();
	}
} // unreal
// doxygen
