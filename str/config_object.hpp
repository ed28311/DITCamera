#ifndef DITCONFIGLOAD
#define DITCONFIGLOAD

#include <iostream>
<<<<<<< HEAD
#include "tool/json_extension.hpp"
=======
#include "nlohmann/json.hpp"

using json=nlohmann::json;

>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
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