#ifndef DITCONFIGLOAD
#define DITCONFIGLOAD

#include <iostream>
#include "nlohmann/json.hpp"

using json=nlohmann::json;

namespace DITCameraTool{
    class Config{
        public:
            ~Config();
            Config();
            Config(json, json);
            json getGlobalConf() const;
            json getAlgorithmConf() const;
        private:
            json globalConf;
            json algorithmConf;
    };
}
#endif