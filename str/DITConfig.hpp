#ifndef DITCONFIGLOAD
#define DITCONFIGLOAD

#include <iostream>
#include "nlohmann/json.hpp"

using json=nlohmann::json;

class Config{
    public:
        ~Config();
        Config();
        Config(json, json);
        json getGlobalConf();
        json getAlgorithmConf();
    private:
        json globalConf;
        json algorithmConf;
};
#endif