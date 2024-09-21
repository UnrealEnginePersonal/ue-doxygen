//
// Created by User on 19/09/2024.
//

#ifndef RUNOPTIONS_H
#define RUNOPTIONS_H

#include <string>

namespace doxykds
{
  struct DoxygenArguments
  {
    const std::string configFilePath;
    const std::string traceName;
    const bool        shortList    = false;
    const bool        traceTiming  = false;
    const bool        updateConfig = false;
    const bool        quiet        = false;
    const bool        genConfig    = false;

    explicit DoxygenArguments(std::string configFilePath,
                                 std::string traceName_    = "",
                                 bool        shortList_    = false,
                                 bool        traceTiming_  = false,
                                 bool        updateConfig_ = false,
                                 bool        quiet_        = false,
                                 bool        genConfig_    = false);
  };
} // namespace doxygen::unreal
#endif // RUNOPTIONS_H
