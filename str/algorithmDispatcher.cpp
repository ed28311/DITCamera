#include <algorithmDispatcher.hpp>


algorithmDispatch::algorithmDispatch(Config config, std::string imagePath){
    DITConfig = config;
    createAlgorithm(imagePath);
    printf("Create algorithm completed.\n");
    std::cout << algorithm->imagePath << std::endl;
};

bool algorithmDispatch::executeAlgorithm(){
    bool resultBool=false;
    resultBool = algorithm->execute();
    return resultBool;
}

void algorithmDispatch::createAlgorithm(std::string imagePath){
    DITConfig = getDITConfig();
    json algorithmConf = DITConfig.getAlgorithmConf();
    if (algorithmConf["mode"] == "LS"){
        algorithm = new shading(DITConfig, imagePath);
    }
    if (algorithmConf["mode"] == "BL"){
        algorithm = new blemish(DITConfig, imagePath);
    }
    if (algorithmConf["mode"] == "FL"){
        algorithm = new flare(DITConfig, imagePath);
    }
};

Config algorithmDispatch::getDITConfig(){
    return DITConfig;
};
