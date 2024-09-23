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
#define UNREAL_DOXY_TEMPLATE_INPUT_TAGFILES "@INPUT_TAGFILES@"
#define UNREAL_DOXY_TEMPLATE_OUT_TAGFILE "@OUTPUT_TAGFILE@"

#include "uedoxygen.h"

#include <Models/DoxygenArguments.h>
#include <resourcemgr.h>

#include <doxygen.h>
#include <fileapi.h>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <NFLogging.h>

/**************
 *DOCYGEM MAIN.cpp Steps
initDoxygen();
readConfiguration(argc,argv);
checkConfiguration();
adjustConfiguration();
parseInput();
generateOutput();
***************/
#define UEDOXLOG_T(msg) nf::log::log("UEDOXYGEN: ", msg, NFLOG_LVL::Info)
namespace doxykds::unreal
{
  inline static bool G_doxygenInitialized = false;

  static void Log(const std::string &msg)
  {
    UEDOXLOG_T(msg);
  }

  static void init()
  {
    //UEDOXYGEN_ASSERT(G_doxygenInitialized, "Doxygen already initialized.");

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

  static bool createDirectoryRecursive(const std::string &dirPath)
  {
    std::string currentPath = "";
    size_t      pos         = 0;
    bool        result      = true;

    // Iterate over each directory in the path
    while ((pos = dirPath.find_first_of("\\/", pos)) != std::string::npos)
    {
      currentPath = dirPath.substr(0, pos++);
      if (!currentPath.empty() && GetFileAttributes(currentPath.c_str()) == INVALID_FILE_ATTRIBUTES)
      {
        // Create directory if it doesn't exist
        if (!CreateDirectory(currentPath.c_str(), NULL))
        {
          /*kds::uelog::uelogger::log("Failed to create directory: " + currentPath);*/
          nf::log::error((R"(Failed to create directory: ")" + currentPath));
          result = false;
          break;
        }
      }
    }

    // Create the final directory in the path
    if (result && GetFileAttributes(dirPath.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
      result = CreateDirectory(dirPath.c_str(), NULL);
      if (!result)
      {
        nf::log::error((R"(Failed to create directory: ")" + dirPath));
      }
    }

    return result;
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
    UEDOXLOG_T("Generating tagfile string.");
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
    UEDOXLOG_T("Generating input string.");
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
  static void writeTextFile(const std::filesystem::path &filepath, const QCString &filedata)
  {
    UEDOXLOG_T(("Writing file: " + filepath.string()));

    if (std::ofstream file(filepath); file.is_open())
    {
      file << filedata;
      file.close();
    }
    else
    {
      nf::log::error(("Failed to write file: " + filepath.string()));
    }
  }

  static bool checkIfOutputDirExistsElseCreate(const std::string &outputPath)
  {
    if (GetFileAttributes(outputPath.c_str()) == INVALID_FILE_ATTRIBUTES)
    {
      if (!createDirectoryRecursive(outputPath))
      {
        nf::log::error((R"(Failed to create output directory: ")" + outputPath));
        return false;
      }
    }
    return true;
  }

  static std::filesystem::path generateTempUEDoxyFile(
    const std::string               projectName,
    const std::string               projectVersion,
    const std::string               tagFileOutputPath,
    const std::string               outputPath,
    const std::vector<std::string> &inputFiles,
    const std::vector<std::string> &tagFiles)
  {

    makeSureInitialized();
    UEDOXLOG_T("Generating temporary Doxyfile.");

    const ResourceMgr &instance         = ResourceMgr::instance();
    QCString           doxyFileTemplate = instance.getAsString("DoxyFile");

    if (doxyFileTemplate.isEmpty())
    {
      UEDOXYGEN_ASSERT(false, (R"(Failed to read Doxyfile template: "%s")", "DoxyFile"));
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

    const std::filesystem::path tempConfigFilePath = getTempPath() + "\\Doxyfile";
    UEDOXLOG_T(R"(Writing temporary Doxyfile to: "%s")" + tempConfigFilePath.string());

    writeTextFile(tempConfigFilePath, doxyFileTemplate);
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
        generateTempUEDoxyFile(m_ProjectName, m_ProjectVersion, m_TagOutputPath, m_OutputPath, m_InputFiles, m_TagFiles)
          .string()))
  {
  }

  DocsGenerator::DocsGenerator(const GeneratorProperties &runOptions) :
      p_Properties(std::make_shared<GeneratorProperties>(runOptions))
  {
    UEDOXLOG_T("DocsGenerator objecthb tfsza created.");
  }

  void DocsGenerator::initialize()
  {
    UEDOXLOG_T("Creating DocsGenerator.");
    UEDOXLOG_T("Output Path: " + p_Properties->getOutputPath());
    checkIfOutputDirExistsElseCreate(p_Properties->getOutputPath());

    UEDOXLOG_T("Initializing Doxygen.");
    makeSureInitialized();

    UEDOXLOG_T("Parsing main arguments.");
    parseMainArgs(p_Properties->getDoxygenRunArgs());
  }


  void DocsGenerator::checkGeneratedConfig()
  {
    UEDOXLOG_T("Checking configuration.");
    checkConfiguration();

    UEDOXLOG_T("Configuration checked.");
  }

  void DocsGenerator::adjustConfig()
  {
    UEDOXLOG_T("Adjusting configuration.");
    adjustConfiguration();


    UEDOXLOG_T("Configuration adjusted.");
  }

  void DocsGenerator::generate()
  {
    UEDOXLOG_T("Generating output.");
    parseInput();
    generateOutput();


    UEDOXLOG_T("Doxygen run completed.");
  }

  void DocsGenerator::clear()
  {
    UEDOXLOG_T("Clearing all.");
    clearAll();
    G_doxygenInitialized = false;
    UEDOXLOG_T("Doxygen cleared.");

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
void DoxygenLogForwarder(const char *logMessage)
{
  UEDOXLOG_T(logMessage);
}
