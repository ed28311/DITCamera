#include "DITAlgorithm.hpp"

DITCameraTool::Algorithm::AlgorithmBase::AlgorithmBase(){
    cv::Mat image;
    std::string imagePath;
    DITCameraTool::Config DITConfig;
}

DITCameraTool::Algorithm::AlgorithmBase::AlgorithmBase(const DITCameraTool::Config config, std::string filePath){
    imagePath = filePath;
    image = loadImage();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    debugMode = _getDebugMode();
}
bool DITCameraTool::Algorithm::AlgorithmBase::execute(DITCameraTool::Logger& ) const{
    return true;
}

cv::Mat DITCameraTool::Algorithm::AlgorithmBase::loadImage() const{
    cv::Mat figure = cv::imread(imagePath);
    if (!image.data){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
};


bool DITCameraTool::Algorithm::AlgorithmBase::_getDebugMode() const{
    return (std::stoi((std::string)globalConf["OutputTestInfo"]));
}

void DITCameraTool::Algorithm::AlgorithmBase::submitLog(json logElement, DITCameraTool::Logger& DITLogger)const{
    DITCameraTool::Logger *loggerPtr = &DITLogger;
    const_cast<DITCameraTool::Logger*>(loggerPtr)->write(logElement);
    const_cast<AlgorithmBase*>(this)->logInitialize(DITLogger);
}
void DITCameraTool::Algorithm::AlgorithmBase::writeLog(std::string key, std::string val) const{
    const_cast<AlgorithmBase*>(this)->logElement[key]=val;
}

void DITCameraTool::Algorithm::AlgorithmBase::logInitialize(DITCameraTool::Logger& DITLogger){
    json logVec;
    for (int i=0; i<DITLogger.logCols.size(); i++){
        logVec[DITLogger.logCols[i]] = " ";
    }
    logElement = logVec;
}


