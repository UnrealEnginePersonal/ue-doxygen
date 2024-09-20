//
// Created by User on 19/09/2024.
//

#ifndef UEDOXYGEN_H
#define UEDOXYGEN_H

#include <string>
#include <vector>
#include <Models/RunOptions.h>

namespace doxygen::unreal
{
  class uedoxygen
  {
  public:

    /**
    * Generates a temporary Doxygen configuration file.
    *
    * @param projectName The name of the project.
    * @param projectVersion The version of the project.
    * @param inputFiles The input files to be documented.
    * @param tagFiles The tag files to be used.
    * @param tagOutputPath The path to the tag file output.
    * @param outputPath The path to the output directory.
    *
    * @returns The path to the temporary configuration file.
    */
    static std::string generateTempConfigFile(
      const std::string& projectName,
      const std::string& projectVersion,
      const std::vector<std::string>& inputFiles,
      const std::vector<std::string>& tagFiles,
      const std::string& tagOutputPath,
      const std::string& outputPath
    );

    static void runDoxygen(const RunOptions& runOptions);
  };
} // unreal
// doxygen

#endif //UEDOXYGEN_H
