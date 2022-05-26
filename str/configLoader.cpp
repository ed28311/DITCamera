#include "configLoader.hpp"

DITConfig::ConfigLoader::ConfigLoader(){
    json config;
}


Config DITConfig::ConfigLoader::getJsonConfig(std::string filePath, std::vector<std::string> modeVector){
    return Config();
};

Config DITConfig::ConfigLoader::getSPEConfig(std::string filePath, std::vector<std::string> modeVector){
    json globalConf;
    json algorithmConf;
    std::tie(globalConf, algorithmConf) = _SPE_parseConfig(filePath, modeVector, GLOBAL_PARAMS);
    config = Config(globalConf, algorithmConf);
    return config;
};

std::tuple<json, json> DITConfig::ConfigLoader::_SPE_parseConfig(std::string filePath, std::vector<std::string> modeVector, std::vector<std::string> GLOBAL_PARAMS){
    std::ifstream inFile(filePath);
    std::string readLine;
    json globalConf;
    json algorithmConf;
    std::string inspect("null");
    bool detectAlgorithm = false;
    algorithmConf["mode"] = modeVector.at(0);
    std::string configMode = modeVector.at(1); 
    algorithmConf["configMode"] = configMode;
    while(std::getline(inFile, readLine)){
        if (std::regex_match(readLine, std::regex("\\[.*[\\w]+\\]"))){
            std::smatch sm;
            std::regex_search(readLine, sm, std::regex("\\w+"));
            inspect = sm[0];
        }
        if (std::regex_match(readLine, std::regex("[\\w]+[\\=\\ ]+[\\w\\.]+"))){
            if (_SPE_isGlobal(inspect, GLOBAL_PARAMS)){
                std::string param;
                std::string value;
                std::tie(param, value) = _SPE_parseLine(readLine);
                globalConf[param] = value;
            }else{
                if(inspect==configMode){
                    detectAlgorithm = true;
                    std::string param;
                    std::string value;
                    std::tie(param, value) = _SPE_parseLine(readLine);
                    algorithmConf[param] = value;
                }
            };
        }   
    };
    if (!detectAlgorithm){
        throw std::invalid_argument("Invalid mode. (mode: "+configMode+").");
    }
    return std::make_tuple(globalConf, algorithmConf);
};

std::tuple<std::string, std::string> DITConfig::ConfigLoader::_SPE_parseLine(std::string readLine){
    /*
    Assumption: 
        1. the readLine only contain a singal equal sign, and space is meanless.
        2. Parameter in config is all integer format.
    */
    std::string param;
    std::string value;
    std::string parseLine = readLine;
    parseLine.erase(std::remove(parseLine.begin(), parseLine.end(), ' '), parseLine.end());
    int parseLineSize = parseLine.size();
    int paramEndLoc = parseLine.find_first_of("=");
    int valueBeginLoc = parseLine.find_last_of("=");
    param = parseLine.substr(0, paramEndLoc);
    value = parseLine.substr(valueBeginLoc+1, parseLineSize);
    return std::make_tuple(param, value);
};

bool DITConfig::ConfigLoader::_SPE_isGlobal(std::string inspect, std::vector<std::string> GLOBAL_PARAMS){
    for (int i=0; i<GLOBAL_PARAMS.size(); i++){
        if (inspect == GLOBAL_PARAMS[i]){
            return true;
        }
    }
    return false;
};