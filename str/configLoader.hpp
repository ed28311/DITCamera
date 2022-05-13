#include <iostream>
#include <regex>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <tuple>
#include "DITConfig.hpp"

namespace DITConfig{
    class ConfigLoader{
        public:
            ConfigLoader();
            ~ConfigLoader();
            Config getSPEConfig(std::string configPath, std::string mode);
            Config getJsonConfig(std::string configPath, std::string mode);
        private:
            Config config;
            std::vector<std::string> const GLOBAL_PARAMS = {"ATD_OUTPUT_INFO", "ASUS_DebugInfo", "Project"};
            std::tuple<json, json> _SPE_parseConfig(std::string, std::string, std::vector<std::string>);
            std::tuple<std::string, std::string> _SPE_parseLine(std::string readLine);
            bool _SPE_isGlobal(std::string, std::vector<std::string>);
    };
}