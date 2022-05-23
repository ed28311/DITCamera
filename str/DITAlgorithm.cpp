#include "DITAlgorithm.hpp"

DITAlgorithm::DITAlgorithm(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

DITAlgorithm::DITAlgorithm(Config config, std::string filePath){
    DITConfig = config;
    imagePath = filePath;
    algorithmConf = DITConfig.getAlgorithmConf();
    globalConf = DITConfig.getGlobalConf();
}
cv::Mat DITAlgorithm::loadImage(){
    image = cv::imread(imagePath);
    if (!image.data){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return image;
};

