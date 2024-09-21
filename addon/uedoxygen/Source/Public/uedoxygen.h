//
// Created by User on 19/09/2024.
//

#ifndef UEDOXYGEN_H
#define UEDOXYGEN_H

#include <UeDoxConfig.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>


// You can override the default assert handler by editing uedoxygenconfig/source/uedoxygenconfig.h
/*#ifndef UEDOXYGEN_ASSERT_OR_RETURN
#include <cassert>
#define UEDOXYGEN_ASSERT_OR_RETURN(_EXPR) assert(_EXPR)
#endif*/

#ifndef UEDOXYGEN_ASSERT
#include <cassert>
#define UEDOXYGEN_ASSERT(_EXPR, _MSG) assert(_EXPR && _MSG)
#endif

#ifndef UEDOXYGEN_INFO
#define UEDOXYGEN_INFO(_MSG) std::cout << _MSG << std::endl;
#endif

namespace doxykds
{
  struct DoxygenArguments;

  namespace unreal
  {
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

    struct GeneratorProperties
    {
    public:
      GeneratorProperties(
        std::string projectName,
        std::string projectVersion,
        std::string tagOutputPath,
        std::string outputPath,

        const std::vector<std::string> &inputFiles,
        const std::vector<std::string> &tagFiles);

      [[nodiscard]] const std::string &getProjectName() const
      {
        return m_ProjectName;
      }
      [[nodiscard]] const std::string &getProjectVersion() const
      {
        return m_ProjectVersion;
      }
      [[nodiscard]] const std::string &getTagOutputPath() const
      {
        return m_TagOutputPath;
      }
      [[nodiscard]] const std::string &getOutputPath() const
      {
        return m_OutputPath;
      }
      [[nodiscard]] DoxygenArguments &getDoxygenRunArgs() const
      {
        return *p_DoxygenRunArgs;
      }

    private:
      const std::string m_ProjectName;
      const std::string m_ProjectVersion;
      const std::string m_TagOutputPath;
      const std::string m_OutputPath;

      const std::vector<std::string> m_InputFiles;
      const std::vector<std::string> m_TagFiles;

      const std::shared_ptr<DoxygenArguments> p_DoxygenRunArgs;
    };

    class DocsGenerator
    {
    public:
      explicit DocsGenerator(const GeneratorProperties &runOptions);
      static void reinitializeShouldAssert();

    private:
      const std::shared_ptr<GeneratorProperties> p_Properties;
    };
  }; // namespace unreal
} // namespace doxygen
// doxygen

#endif // UEDOXYGEN_H
