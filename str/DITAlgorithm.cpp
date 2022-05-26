#include "DITAlgorithm.hpp"

DITAlgorithm::DITAlgorithm(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

bool DITAlgorithm::execute () const{
    return true;
}

DITAlgorithm::DITAlgorithm(const Config config, std::string filePath){
    imagePath = filePath;
    image = loadImage();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
}
cv::Mat DITAlgorithm::loadImage() const{
    cv::Mat figure = cv::imread(imagePath);
    if (!image.data){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
};

