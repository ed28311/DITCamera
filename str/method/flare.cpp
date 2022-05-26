#include "flare.hpp"


DITCameraTool::Algorithm::Flare::Flare(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}


DITCameraTool::Algorithm::Flare::Flare(DITCameraTool::Config config, std::string filePath){
    std::string imagePath(filePath);
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    std::cout << "Loading Completed." << std::endl;

}
bool DITCameraTool::Algorithm::Flare::execute() const {
    return true;
}


cv::Mat DITCameraTool::Algorithm::Flare::loadImage() const {
    return cv::Mat();
}
