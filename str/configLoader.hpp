#ifndef CONFIGLOAD
#define CONFIGLOAD
#include <iostream>
#include <regex> 
#include <fstream>
#include <string.h>
#include <algorithm>
#include <tuple>
#include "DITConfig.hpp"
#include "./tool/displayer.hpp"

namespace DITCameraTool{
    class ConfigLoader{
        public:
            ConfigLoader();
            DITCameraTool::Config getSPEConfig(std::string configPath, std::vector<std::string> modeVector);
            DITCameraTool::Config getJsonConfig(std::string configPath, std::vector<std::string> modeVector);
        private:
            DITCameraTool::Config config;
            std::vector<std::string> const GLOBAL_PARAMS{"ATD_OUTPUT_INFO", "ASUS_DebugInfo", "Project"};
            std::tuple<json, json> _SPE_parseConfig(std::string filePath, std::vector<std::string> modeVector, std::vector<std::string> GLOBAL_PARAMS);
            std::tuple<std::string, std::string> _SPE_parseLine(std::string readLine);
            bool _SPE_isGlobal(std::string, std::vector<std::string>);
    };
}
#endif