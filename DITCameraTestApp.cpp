#include <iostream>
#include <vector>
#include <regex>

#include "./str/configLoader.hpp"
#include "./str/algorithmDispatcher.hpp"

void checkDITArgument(int, char **);
std::vector<std::string>  parseDITMode( std::string);

int main(int argc, char ** argv){
    try{
        DITCameraTool::ConfigLoader configLoader;
        checkDITArgument(argc, argv);
        std::string mode = std::string(argv[1]);
        std::string configPath = std::string(argv[2]);
        std::string imagePath = std::string(argv[3]);
        std::vector<std::string> modeVector(2);
        modeVector = parseDITMode(mode);
        DITCameraTool::Config DITConfig = configLoader.getSPEConfig(configPath, modeVector);
        json globalConfig = DITConfig.getGlobalConf();
        DITCameraTool::Logger logger(globalConfig);
        printf("mode: %s\n", mode.c_str());
        printf("configPath: %s\n",configPath.c_str());
        printf("imagePath: %s\n", imagePath.c_str());
        printf("---------------------------\n");
        DITCameraTool::AlgorithmDispatch dispatcher(DITConfig, imagePath, logger);
        dispatcher.executeAlgorithm();
        bool logResult = logger.generateCSV();
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
    if (argc!=4){
        throw std::invalid_argument("Invalid arguments format (Too little arguments.). (format: -mode[modeName] configPath imagePath)");
    }
}