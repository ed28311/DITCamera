#include "blemish.hpp"


DITCameraTool::Algorithm::Blemish::Blemish(){
    cv::Mat image;
    std::string imagePath;
}

DITCameraTool::Algorithm::Blemish::Blemish(DITCameraTool::Config config, std::string filePath){
    std::string imagePath(filePath);
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    std::cout << "Loading Completed." << std::endl;
}

cv::Mat DITCameraTool::Algorithm::Blemish::loadImage() const{
    return cv::Mat();
}

bool DITCameraTool::Algorithm::Blemish::execute() const{return true;}

