#include "Models/RunOptions.h"

RunOptions::RunOptions(std::string configName_, std::string traceName_, const bool shortList_, const bool traceTiming_, const bool updateConfig_, const bool quiet_, const bool genConfig_)
: configName(std::move(configName_)), traceName(std::move(traceName_)), shortList(shortList_),
  traceTiming(traceTiming_), updateConfig(updateConfig_), quiet(quiet_), genConfig(genConfig_)
{
}