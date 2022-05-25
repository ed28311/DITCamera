#include <iostream>
#include "../str/method/shading.hpp"
#include "../str/configLoader.hpp"
#include "../str/DITConfig.hpp"

std::vector<std::string> parseDITMode(std::string DITMode);

int main(){
    std::string imagePath("/home/ed/DITCamera/test_pic/shading.jpg");
    std::string configPath("/home/ed/DITCamera/CameraSpec.spe");
    std::string mode("-LS[Shading]");
    DITConfig::ConfigLoader configLoader;
    printf("\nmode: %s\n", mode.c_str());
    printf("configPath: %s\n",configPath.c_str());
    printf("imagePath: %s\n", imagePath.c_str());
    printf("---------------------------\n");
    std::vector<std::string> modeVector(2);
    modeVector = parseDITMode(mode);
    Config DITConfig = configLoader.getSPEConfig(configPath, modeVector);
    json algConf = DITConfig.getAlgorithmConf();
    json globalConf = DITConfig.getGlobalConf();
    printf("Config loaded\n");
    shading algorithm(DITConfig, imagePath);
    // algorithm.execute();
}

std::vector<std::string> parseDITMode(std::string DITMode){
    std::vector<std::string> modeVector(2);
    std::regex reg("^-.*[\\w]+\\[.*[\\w]+\\]");
    std::string mode = DITMode;
    if (std::regex_match(mode, reg)){
        std::smatch sm;
        std::regex matchstring("[\\w]+");
        int i = 0;
        while(std::regex_search(mode, sm, matchstring)){
            for (std::string x:sm) {
                modeVector[i] = x;
            }
            mode = sm.suffix().str();
            i++;
            if(i>2){
                throw std::invalid_argument("Invalid arguments format ("+mode+"Wrong mode format.). (format: -mode[modeName] configPath imagePath)");
                break;
            }
        }
    }else{
        throw std::invalid_argument("Invalid arguments format ("+mode+"Wrong mode format.). (format: -mode[modeName] configPath imagePath)");
    }
    return modeVector;
}