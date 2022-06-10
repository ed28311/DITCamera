#include <algorithmDispatcher.hpp>


DITCameraTool::AlgorithmDispatch::AlgorithmDispatch(DITCameraTool::Config config, std::string imagePath, DITCameraTool::Logger &logger):DITLogger(logger){
    DITConfig = config;
    createAlgorithm(imagePath);
};

bool DITCameraTool::AlgorithmDispatch::executeAlgorithm(){
    bool resultBool=false;
    resultBool = algorithm->execute(DITLogger);
    return resultBool;
}

void DITCameraTool::AlgorithmDispatch::createAlgorithm(std::string imagePath){
    DITConfig = getDITConfig();
    json algorithmConf = DITConfig.getAlgorithmConf();
    if (algorithmConf["mode"] == "LS"){
        algorithm = new DITCameraTool::Algorithm::Shading(DITConfig, imagePath);
    }
    if (algorithmConf["mode"] == "BL"){
        algorithm = new DITCameraTool::Algorithm::Blemish(DITConfig, imagePath);
    }
    if (algorithmConf["mode"] == "FL"){
        algorithm = new DITCameraTool::Algorithm::Flare(DITConfig, imagePath);
    }
    if (algorithmConf["mode"] == "BLPX"){
        algorithm = new DITCameraTool::Algorithm::BlemishPixel(DITConfig, imagePath);
    }
    if (algorithmConf["mode"] == "BLST"){
        algorithm = new DITCameraTool::Algorithm::BlemishStat(DITConfig, imagePath);
    }
};

DITCameraTool::Config DITCameraTool::AlgorithmDispatch::getDITConfig(){
    return DITConfig;
};
