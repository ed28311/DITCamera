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
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    std::cout << "Loading Completed." << std::endl;

}
bool flare::execute() const {
    return true;
}


cv::Mat flare::loadImage() const {
    return DITAlgorithm::loadImage();
}
