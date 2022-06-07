#include "DITAlgorithm.hpp"

DITCameraTool::Algorithm::AlgorithmBase::AlgorithmBase(){
    cv::Mat image;
    std::string imagePath;
    DITCameraTool::Config DITConfig;
}

bool DITCameraTool::Algorithm::AlgorithmBase::execute () const{
    return true;
}

DITCameraTool::Algorithm::AlgorithmBase::AlgorithmBase(const DITCameraTool::Config config, std::string filePath){
    imagePath = filePath;
    image = loadImage();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    debugMode = _getDebugMode();
    DITLogger = _loadLogger();
    logElement = logInitialize();
}

json DITCameraTool::Algorithm::AlgorithmBase::logInitialize() const{
    json logVec;
    for (int i=0; i<DITLogger.logCols.size(); i++){
        logVec[DITLogger.logCols[i]] = " ";
    }
    return logVec;
}

DITCameraTool::Logger DITCameraTool::Algorithm::AlgorithmBase::_loadLogger(){
    return DITCameraTool::Logger(globalConf);
}

bool DITCameraTool::Algorithm::AlgorithmBase::_getDebugMode() const{
    return (std::stoi((std::string)globalConf["OutputTestInfo"]));
}

cv::Mat DITCameraTool::Algorithm::AlgorithmBase::loadImage() const{
    cv::Mat figure = cv::imread(imagePath);
    if (!image.data){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
};


