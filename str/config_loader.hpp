#ifndef CONFIGLOAD
#define CONFIGLOAD
#include <iostream>
#include <regex> 
#include <fstream>
#include <string.h>
#include <algorithm>
#include <tuple>
#include "config_object.hpp"
#include "./tool/displayer.hpp"

namespace DITCameraTool{
    class ConfigLoader{
        public:
            ConfigLoader();
            std::vector<std::string> m_global_config_member;
            bool SetConfigProp(std::vector<std::string>, std::vector<std::string>);
            DITCameraTool::Config GetSPEConfig(std::string configPath, std::string algorithm_name,std::vector<std::string> global_config_member);
            DITCameraTool::Config GetJsonConfig(std::string configPath, std::string algorithm_name,std::vector<std::string> global_config_member);
        private:
            bool IsGlobalParam(std::string);
            std::tuple<json, json> _ParseSPEConfig(std::string file_path, std::string algorithm, std::string config_mode);
            std::tuple<std::string, std::string> _ParseSPEConfigLine(std::string);
            std::tuple<std::string, std::string>  _ParseModeArgument(std::string);
    };
}
#endif