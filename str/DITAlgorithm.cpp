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
}
cv::Mat DITCameraTool::Algorithm::AlgorithmBase::loadImage() const{
    cv::Mat figure = cv::imread(imagePath);
    if (!image.data){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
};

