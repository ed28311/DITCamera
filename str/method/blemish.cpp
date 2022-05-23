#include "blemish.hpp"

blemish::~blemish(){
    cv::Mat* imagePtr = &image;
    delete imagePtr;
}

blemish::blemish(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

blemish::blemish(Config config, std::string filePath){
    std::string imagePath(filePath);
    Config DITConfig = config;
    cv::Mat image = loadImage();
}

cv::Mat blemish::loadImage(){
    return DITAlgorithm::loadImage();
}

bool blemish::execute(){return true;}

