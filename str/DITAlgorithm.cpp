#include "DITAlgorithm.hpp"

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

