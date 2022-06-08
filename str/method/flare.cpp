#include "flare.hpp"
#include <cmath>

DITCameraTool::Algorithm::Flare::Flare(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

cv::Mat DITCameraTool::Algorithm::Flare::loadImage() const {
    if(false){
        std::cout << imagePath << std::endl;
    }
    cv::Mat figure = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    cv::Size figureSize(figure.size());
    if (figure.empty()){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
}

DITCameraTool::Algorithm::Flare::Flare(DITCameraTool::Config config, std::string filePath){
    imagePath = filePath;
    image = loadImage();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    debugMode = _getDebugMode();
}

bool DITCameraTool::Algorithm::Flare::execute(DITCameraTool::Logger& logger) const {
    DITCameraTool::Logger& DITLogger = logger;
    cv::Mat hist;
    std::vector<int> pixelStat(256, 0);
    _statisticPixel(pixelStat, image);
    bool resultBool = _detectStd(pixelStat);
    printf("%s\n", resultBool?"Pass":"Not Pass");
    return resultBool;
}

void DITCameraTool::Algorithm::Flare::_statisticPixel(std::vector<int> &pixelStat, cv::Mat image) const {
    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++){
            pixelStat[image.at<uchar>(i, j)] += 1;
        }
    }
}

bool DITCameraTool::Algorithm::Flare::_detectStd(std::vector<int> pixelArray) const {
    bool resultBool = false;
    int imagePixelNums = image.rows*image.cols;
    float pixelStd = _evalPixelStandardDeviation(pixelArray, imagePixelNums);
    float stdThreshold = std::stof((std::string)algorithmConf["Std_Threshold"]);
    if (false){
        std::cout << stdThreshold << std::endl;
        printf("imagePath: %s \npixelStd:%.2e\n", imagePath.c_str(), pixelStd);
        printf("%.2f > %.2f: %s\n", pixelStd, stdThreshold, (pixelStd > stdThreshold)?"true":"false");
    }
    if (pixelStd > stdThreshold){
        resultBool = true;
    }
    return resultBool;
}

float DITCameraTool::Algorithm::Flare::_evalPixelStandardDeviation(std::vector<int> intArray, int imagePixelNums) const {
    float imagePixelNumsF = static_cast<float>(imagePixelNums);
    float mean = 0;
    float meanSquare = 0;
    for (int i=0; i<intArray.size(); i++){
        mean += (intArray[i]/imagePixelNumsF)*i;
        meanSquare += ((intArray[i]/imagePixelNumsF)*(i*i));
    }
    if(false){
        std::cout << intArray.size() << std::endl;
        std::cout << mean << std::endl;
        std::cout << meanSquare << std::endl;
        std::cout << meanSquare - (mean*mean) << std::endl;
    }
    float standardDeviation = sqrt(meanSquare - (mean*mean));
    return standardDeviation;
}

