//
// Created by Kasper de Bruin on 19/09/2024.
//

#include <windows.h>

#ifndef UEDOXYGEN_TEMP_PATH
#define UEDOXYGEN_TEMP_PATH "%localappdata%\\Temp\\Doxyfile"
#endif

#define UNREAL_DOXY_TEMPLATE_PROJECT_NAME "@PROJECT_NAME@"
#define UNREAL_DOXY_TEMPLATE_OUTPUT_DIR "@OUTPUT_DIR@"
#define UNREAL_DOXY_TEMPLATE_INPUT_SRC "@INPUT_SRC@"
#define UNREAL_DOXY_TEMPLATE_PROJECT_EXCLUDES "@EXCLUDES@"
#define UNREAL_DOXY_TEMPLATE_INPUT_TAGFILES "@INPUT_TAGFILES@"
#define UNREAL_DOXY_TEMPLATE_OUT_TAGFILE "@OUT_TAGFILE@"

#include "uedoxygen.h"

#include <Models/DoxygenArguments.h>
#include <resourcemgr.h>

#include <doxygen.h>
#include <fileapi.h>
#include <filesystem>
#include <sstream>

namespace doxykds::unreal
{
  inline static bool G_doxygenInitialized = false;

  static void init()
  {
    if (G_doxygenInitialized)
    {
      UEDOXYGEN_ASSERT(false, "Doxygen already initialized.");
    }
    initDoxygen();
    G_doxygenInitialized = true;
  }

  static std::string getTempPath()
  {
    char tempPath[MAX_PATH];

    if (const DWORD pathLen = GetTempPath(MAX_PATH, tempPath); pathLen > 0 && pathLen < MAX_PATH)
    {
      return {tempPath};
    }

    // Handle error (could throw an exception or return an empty string)
    return std::filesystem::temp_directory_path().string();
  }

  static void makeSureInitialized()
  {
    if (!G_doxygenInitialized)
    {
      init();
    }
  }

  static void findAndReplace(QCString &doxyFileTemplate, const QCString &placeHolder, const std::string &replacement)
  {
    auto placeHolderIndex = doxyFileTemplate.find(placeHolder);

    if (placeHolderIndex != std::string::npos)
    {
      doxyFileTemplate.replace(placeHolderIndex, placeHolder.size(), replacement.c_str());
    }
    else
    {
      UEDOXYGEN_ASSERT(false, (R"(Failed to find placeholder: "%s")", placeHolder));
      return;
    }
  }


  static std::string generate_tagfile_str(const std::string &selfTagOut, const std::vector<std::string> &files)
  {
    // Check if the files list is empty
    if (files.empty())
    {
      return "TAGFILES  = ";
    }

    // If there's only one file, handle separately
    if (files.size() == 1)
    {
      if (files[0] == selfTagOut)
      {
        return "TAGFILES  =";
      }
      else
      {
        return "TAGFILES  = " + files[0];
      }
    }

    // Handle case where the first file is the same as selfTagOut
    std::vector<std::string> fileList = files;
    if (files[0] == selfTagOut)
    {
      fileList.erase(fileList.begin());
    }

    // Generate the string with tagfiles
    std::ostringstream result;
    result << "TAGFILES  = " << fileList[0] << "\n";

    for (size_t i = 1; i < fileList.size(); ++i)
    {
      if (fileList[i] == selfTagOut)
      {
        continue;
      }
      result << "TAGFILES  += " << fileList[i] << "\n";
    }

    return result.str();
  }

  static std::string generate_input_str(const std::vector<std::string> &inputs)
  {
    UEDOXYGEN_ASSERT(!inputs.empty(), "Input files list is empty.");

    // Generate the string with input directories
    std::ostringstream result;
    result << "INPUT  = " << inputs[0] << "\n";

    for (size_t i = 1; i < inputs.size(); ++i)
    {
      result << "INPUT  += " << inputs[i] << "\n";
    }

    return result.str();
  }

  /*static */
  static std::filesystem::path generateTempUEDoxyFile(
    const std::string              &projectName,
    const std::string              &projectVersion,
    const std::string              &tagFileOutputPath,
    const std::string              &outputPath,
    const std::vector<std::string> &inputFiles,
    const std::vector<std::string> &tagFiles)
  {
    makeSureInitialized();
    QCString                    doxyFileTemplate = ResourceMgr::instance().getAsString("Doxyfile");

    if (doxyFileTemplate.isEmpty())
    {
      UEDOXYGEN_ASSERT(false, (R"(Failed to read Doxyfile template: "%s")", "Doxyfile"));
      return {};
    }

    findAndReplace(doxyFileTemplate, UNREAL_DOXY_TEMPLATE_PROJECT_NAME, projectName);
    findAndReplace(doxyFileTemplate, UNREAL_DOXY_TEMPLATE_OUTPUT_DIR, outputPath);

    const std::string inputStr = generate_input_str(inputFiles);
    findAndReplace(doxyFileTemplate, UNREAL_DOXY_TEMPLATE_INPUT_SRC, inputStr);

    const std::string outputTagFile = tagFileOutputPath.empty() ? "" : tagFileOutputPath;
    findAndReplace(doxyFileTemplate, UNREAL_DOXY_TEMPLATE_OUT_TAGFILE, outputTagFile);

    const std::string externalTagFiles = tagFiles.empty() == 0 ? "" : generate_tagfile_str(tagFileOutputPath, tagFiles);
    findAndReplace(doxyFileTemplate, UNREAL_DOXY_TEMPLATE_INPUT_TAGFILES, externalTagFiles);

    const std::filesystem::path tempConfigFilePath   = getTempPath() + "/Doxyfile";

    UEDOXYGEN_INFO((R"(Writing temporary Doxyfile to: "%s")", tempConfigFilePath.string()));
    return tempConfigFilePath;
  }

  GeneratorProperties::GeneratorProperties(
    std::string                     projectName,
    std::string                     projectVersion,
    std::string                     tagOutputPath,
    std::string                     outputPath,
    const std::vector<std::string> &inputFiles,
    const std::vector<std::string> &tagFiles) :
      m_ProjectName(std::move(projectName)),
      m_ProjectVersion(std::move(projectVersion)),
      m_TagOutputPath(std::move(tagOutputPath)),
      m_OutputPath(std::move(outputPath)),
      m_InputFiles(inputFiles),
      m_TagFiles(tagFiles),
      p_DoxygenRunArgs(std::make_shared<DoxygenArguments>(
        generateTempUEDoxyFile(projectName, projectVersion, tagOutputPath, outputPath, inputFiles, tagFiles).string()))
  {
  }

  DocsGenerator::DocsGenerator(const GeneratorProperties &runOptions) :
      p_Properties(std::make_shared<GeneratorProperties>(runOptions))
  {
    if (!p_Properties)
    {
      UEDOXYGEN_ASSERT(false, "Failed to create DocsGenerator.");
      return;
    }

    parseMainArgs(p_Properties->getDoxygenRunArgs());
    checkConfiguration();
    adjustConfiguration();
    parseInput();
    generateOutput();
  }
  void DocsGenerator::reinitializeShouldAssert()
  {
    UEDOXYGEN_INFO("This is an test and should throw an assert");
    UEDOXYGEN_INFO("FIRST RUN (In this function)");
    initDoxygen();
    UEDOXYGEN_INFO("DIDNT ASSERT YET? TRY AGAIN");
    initDoxygen();
  }

  /*initDoxygen();*/
  /*parseMainArgs(runOptions);*/
  /*checkConfiguration();*/
  /*adjustConfiguration();*/
  /*parseInput();*/
  /*generateOutput();*/

  // void DocsGenerator::runDoxygen(const RunOptions &runOptions)
  // {
  //   checkConfiguration();
  //   adjustConfiguration();
  //   parseInput();
  //   generateOutput();
  // }

} // namespace doxykds::unreal
// doxykds
