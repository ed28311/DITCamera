#include "DITConfig.hpp"

DITCameraTool::Config::~Config(){
    globalConf.clear();
    algorithmConf.clear();
}

DITCameraTool::Config::Config(){
    json globalConf;
    json algorithmConf;
}

DITCameraTool::Config::Config(json globalConfig, json algorithmConfig){
    globalConf = globalConfig;
    algorithmConf = algorithmConfig;
}

json DITCameraTool::Config::getAlgorithmConf() const{
    return algorithmConf;
};

json DITCameraTool::Config::getGlobalConf() const{
    return globalConf;
};