#include "DITConfig.hpp"


Config::Config(json globalConf, json algorithmConf){
    Config::globalConf = globalConf;
    Config::algorithmConf = algorithmConf;
}

json Config::getAlgorithmConf(){
    return Config::algorithmConf;
};

json Config::getGlobalConf(){
    return Config::globalConf;
};