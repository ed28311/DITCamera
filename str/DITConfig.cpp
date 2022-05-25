#include "DITConfig.hpp"

Config::~Config(){
    globalConf.clear();
    algorithmConf.clear();
}

Config::Config(){
    json globalConf;
    json algorithmConf;
}

Config::Config(json globalConf, json algorithmConf){
    Config::globalConf = globalConf;
    Config::algorithmConf = algorithmConf;
}

json Config::getAlgorithmConf() const{
    return Config::algorithmConf;
};

json Config::getGlobalConf() const{
    return Config::globalConf;
};