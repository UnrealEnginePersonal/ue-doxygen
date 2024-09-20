//
// Created by Kasper de Bruin on 19/09/2024.
//

#include "uedoxygen.h"

#include <doxygen.h>

namespace doxygen::unreal
{


  std::string uedoxygen::generateTempConfigFile(
    const std::string &             projectName,
    const std::string &             projectVersion,
    const std::vector<std::string> &inputFiles,
    const std::vector<std::string> &tagFiles,
    const std::string &             tagOutputPath,
    const std::string &             outputPath
    )
  {


  }

  void uedoxygen::runDoxygen(const RunOptions &runOptions)
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
