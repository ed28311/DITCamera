#include <iostream>
#include <vector>
#include <regex>

#include "./str/configLoader.hpp"
#include "./str/algorithmDispatcher.hpp"

void checkDITArgument(int, char **);
std::vector<std::string> parseDITMode(std::string);

int main(int argc, char ** argv){
    try{
        DITConfig::ConfigLoader configLoader;
        checkDITArgument(argc, argv);
        std::string mode = std::string(argv[1]);
        std::string configPath = std::string(argv[2]);
        std::string imagePath = std::string(argv[3]);
        std::vector<std::string> modeVector = parseDITMode(mode);
        Config DITConfig = configLoader.getSPEConfig(configPath, modeVector);
        algorithmDispatch dispatcher(DITConfig, imagePath);
        dispatcher.executeAlgorithm();
    }catch(std::invalid_argument& e){
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}

std::vector<std::string> parseDITMode(std::string DITMode){
    std::regex reg("^-.*[\\w]+\\[.*[\\w]+\\]");
    std::string mode = DITMode;
    std::vector<std::string> modevector;
    if (regex_match(mode, reg)){
        std::smatch sm;
        std::regex matchstring("[\\w]+");
        while(std::regex_search(mode, sm, matchstring)){
            modevector.push_back(sm[0]);
            mode = sm.suffix().str();
        }
        return modevector;
    }else{
        throw std::invalid_argument("Invalid arguments format ("+mode+"Wrong mode format.). (format: -mode[modeName] configPath imagePath)");
    }
}
void checkDITArgument(int argc, char ** argv){
    if (argc!=4){
        throw std::invalid_argument("Invalid arguments format (Too little arguments.). (format: -mode[modeName] configPath imagePath)");
    }
}