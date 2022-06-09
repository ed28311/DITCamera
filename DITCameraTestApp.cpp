#include <iostream>
#include <regex>

#include "./str/configLoader.hpp"
#include "./str/algorithmDispatcher.hpp"
#include "./str/tool/displayer.hpp"

void checkDITArgument(int, char **);
std::vector<std::string>  parseDITMode( std::string);

int main(int argc, char * argv[]){
    try{
        DITCameraTool::ConfigLoader configLoader;
        DITCameraTool::Logger totalLogger;
        std::vector<std::string> argvDebug(argv, argv+argc);
        for (int i=0; i<(argc/3); i++){
            checkDITArgument(argc, argv);
            std::string mode = std::string(argv[3*i+1]);
            std::string configPath = std::string(argv[3*i+2]);
            std::string imagePath = std::string(argv[3*i+3]);
            std::vector<std::string> modeVector(2);
            modeVector = parseDITMode(mode);
            DITCameraTool::Config DITConfig = configLoader.getSPEConfig(configPath, modeVector);
            json globalConfig = DITConfig.getGlobalConf();
            DITCameraTool::Logger logger(globalConfig);
            if(std::stoi((std::string)globalConfig["OutputTestInfo"])){
                printf("mode: %s\n", mode.c_str());
                printf("configPath: %s\n",configPath.c_str());
                printf("imagePath: %s\n", imagePath.c_str());
                printf("---------------------------\n");
            }
            DITCameraTool::AlgorithmDispatch* dispatcher = new DITCameraTool::AlgorithmDispatch(DITConfig, imagePath, logger);
            dispatcher->executeAlgorithm();
            totalLogger.mergeLogger(logger);
        }
        bool logResult = totalLogger.generateCSV();
            _PrintVariable(logResult);
    }catch(std::invalid_argument& e){
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
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
void checkDITArgument(int argc, char ** argv){
    if ((argc%3)!=1){
        throw std::invalid_argument("Invalid arguments format (Too little arguments.). (format: -mode[modeName] configPath imagePath)");
    }
}