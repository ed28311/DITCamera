#include <algorithmDispatcher.hpp>


algorithmDispatch::algorithmDispatch(Config config, std::string imagePath){
    DITConfig = config;
    createAlgorithm(imagePath);
    std::cout << algorithm->imagePath << std::endl;
};

bool algorithmDispatch::executeAlgorithm(){
    bool resultBool=false;
    std::cout << algorithm << std::endl;
    resultBool = algorithm->execute();
    return resultBool;
}

void algorithmDispatch::createAlgorithm(std::string imagePath){
    DITConfig = getDITConfig();
    json algorithmConf = DITConfig.getAlgorithmConf();
    std::cout << algorithm << std::endl;
    if (algorithmConf["configMode"] == "Shading"){
        algorithm = new shading(DITConfig, imagePath);
    }
    if (algorithmConf["configMode"] == "Blemish"){
        algorithm = new blemish(DITConfig, imagePath);
    }
    if (algorithmConf["configMode"] == "Flare"){
        algorithm = new flare(DITConfig, imagePath);
    }
};

Config algorithmDispatch::getDITConfig(){
    return DITConfig;
};
