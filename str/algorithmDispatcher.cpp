#include <algorithmDispatcher.hpp>


algorithmDispatch::algorithmDispatch(Config config, std::string imagePath){
    DITConfig = getDITConfig();
    createAlgorithm(imagePath);
};

bool algorithmDispatch::executeAlgorithm(){
    bool resultBool=false;
    resultBool = algorithm->execute();
    return resultBool;
}

void algorithmDispatch::createAlgorithm(std::string imagePath){
    DITConfig = getDITConfig();
    json algorithmConf = DITConfig.getAlgorithmConf();
    if (algorithmConf['mode'] == "Shading"){
        algorithm = new shading(DITConfig, imagePath);
    }
    if (algorithmConf['mode'] == "Blemish"){
        algorithm = new blemish(DITConfig, imagePath);
    }
    if (algorithmConf['mode'] == "Flare"){
        algorithm = new flare(DITConfig, imagePath);
    }
};

Config algorithmDispatch::getDITConfig(){
    return DITConfig;
};
