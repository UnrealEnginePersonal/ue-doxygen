#include <utility>

#include "Models/DoxygenArguments.h"
namespace doxykds
{
  DoxygenArguments::DoxygenArguments(std::string configFilePath,
                                     std::string traceName_ /*= ""*/,
                                     const bool  shortList_ /*= false*/,
                                     const bool  traceTiming_ /*= false*/,
                                     const bool  updateConfig_ /*= false*/,
                                     const bool  quiet_ /*= false*/,
                                     const bool  genConfig_ /*= false*/
                                     ) :
      configFilePath(std::move(configFilePath)), traceName(std::move(traceName_)), shortList(shortList_),
      traceTiming(traceTiming_), updateConfig(updateConfig_), quiet(quiet_), genConfig(genConfig_)
  {
  }
} // namespace doxykds
