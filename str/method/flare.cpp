#include "flare.hpp"


flare::flare(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

flare::~flare(){
    cv::Mat* imagePtr = &image;
    delete imagePtr;
}

flare::flare(Config config, std::string filePath){
    std::string imagePath(filePath);
    Config DITConfig = config;
    cv::Mat image = loadImage();
}
bool flare::execute(){
    return true;
}


cv::Mat flare::loadImage(){
    return DITAlgorithm::loadImage();
}
