#include "blemish.hpp"


blemish::blemish(){
    cv::Mat image;
    std::string imagePath;
}

blemish::blemish(Config config, std::string filePath){
    std::string imagePath(filePath);
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    std::cout << "Loading Completed." << std::endl;
}

cv::Mat blemish::loadImage() const{
    return DITAlgorithm::loadImage();
}

bool blemish::execute() const{return true;}

