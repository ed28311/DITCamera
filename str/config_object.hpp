#ifndef DITCONFIGLOAD
#define DITCONFIGLOAD

#include <iostream>
#include "nlohmann/json.hpp"

using json=nlohmann::json;

namespace DITCameraTool
{
    class Config
    {
        public:
            ~Config();
            Config();
            Config(json, json);
            json GetGlobalConf() const;
            json GetAlgorithmConf() const;
        private:
            json m_global_config;
            json m_algorithm_config;
    };
}
#endif