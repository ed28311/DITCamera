#include "blemishPixel.hpp"


DITCameraTool::Algorithm::BlemishPixel::BlemishPixel(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

DITCameraTool::Algorithm::BlemishPixel::BlemishPixel(DITCameraTool::Config config, std::string filePath){
    imagePath = filePath;
    image = loadImage();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
}

cv::Mat DITCameraTool::Algorithm::BlemishPixel::loadImage() const{
    cv::Mat figure = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    cv::Size figureSize(figure.size());
    if (figure.empty()){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
}

bool DITCameraTool::Algorithm::BlemishPixel::execute() const{
    std::vector<std::vector<int>> logObject;
    bool resultBool = _loopPixels(true, logObject);
    std::cout << resultBool << std::endl;
    return resultBool;
}


bool DITCameraTool::Algorithm::BlemishPixel::_loopPixels(bool displaySelectPixel, std::vector<std::vector<int>> &logObject) const {
    int stride = std::stoi((std::string)algorithmConf["Stride"]);
    int diffThres = std::stoi((std::string)algorithmConf["Diff_Tolarence"]);
    int selectPixel;
    int targetPixel;
    int maxPixelDiff = 0;
    int minPixelDiff = 255;
    int diffPixel;
    bool resultBool = true;
    for(int i=0; i<image.rows; i+=stride){
        for(int j=0; j<image.cols-1; j+=stride){
            int selectPixel = image.at<uchar>(i,j);
            if((j+stride)>image.cols){
                targetPixel = image.at<uchar>(i,j+stride);
            }
            else{
                targetPixel = image.at<uchar>(i,image.cols);
            }
            diffPixel = _calcAbsVal(targetPixel-selectPixel);
            if (diffPixel>maxPixelDiff){
                maxPixelDiff = diffPixel;
                printf("maxPixelUpdated at (%d, %d) and (%d, %d): Diff between %d and %d: %d\n", i, j, i, j+stride, selectPixel, targetPixel, diffPixel);
            }
            if(diffPixel<minPixelDiff){
                minPixelDiff = diffPixel;
                printf("minPixelUpdated at (%d, %d) and (%d, %d): Diff between %d and %d: %d\n", i, j, i, j+stride, selectPixel, targetPixel, diffPixel);
            }
            if(diffPixel>diffThres){
                resultBool = false;
            }
        }
    }
    printf("maxPixelDiff:%d, minPixelDiff:%d\n", maxPixelDiff, minPixelDiff);
    return resultBool;
}


int DITCameraTool::Algorithm::BlemishPixel::_calcAbsVal(int val) const{
    if(val < 0){
        return -1*val;
    }
    return val;
}