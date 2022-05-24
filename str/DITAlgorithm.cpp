#include "DITAlgorithm.hpp"

DITAlgorithm::DITAlgorithm(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

DITAlgorithm::~DITAlgorithm(){
    Config *configPtr = &DITConfig;
    cv::Mat *imagePtr = &image;
    delete imagePtr;
    delete configPtr;
}
bool DITAlgorithm::execute(){
    return true;
}

DITAlgorithm::DITAlgorithm(Config config, std::string filePath){
    DITConfig = config;
    imagePath = filePath;
    algorithmConf = DITConfig.getAlgorithmConf();
    globalConf = DITConfig.getGlobalConf();
}
cv::Mat DITAlgorithm::loadImage(){
    cv::Mat figure = cv::imread(imagePath);
    if (!image.data){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
};

