#include "shading.hpp"


shading::shading(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

shading::shading(Config config, std::string filePath){
    std::string imagePath(filePath);
    Config DITConfig = config;
    cv::Mat image = loadImage();
}

cv::Mat shading::loadImage(){
    image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    if (!image.data){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return image;
}

bool shading::execute(){
    bool resultBool = false;
    image = loadImage();
    json algorithmConf = DITConfig.getAlgorithmConf();
    float BLOCKRATIO = algorithmConf["BlockRatio"]; //BlockRatio = 0.1;
    int imageH = image.rows;
    int imageW = image.cols;
    int blockH = static_cast<float>(imageH)*BLOCKRATIO;
    int blockW = static_cast<float>(imageW)*BLOCKRATIO;
    int avgAreaLT = _avgPixel(0, 0, blockW, blockH);
    int avgAreaLB = _avgPixel(0, imageH-blockH, blockW, blockH);
    int avgAreaRT = _avgPixel(imageW-blockW, 0, blockW, blockH);
    int avgAreaRB = _avgPixel(imageW-blockW, imageH-blockH, blockW, blockH);
    int avgAreaCentre = _avgPixel(imageW/4, imageH/4, imageW/2, imageH/2);
    std::vector<int> avgAreaCorner{avgAreaLB, avgAreaLT, avgAreaRB, avgAreaRT};
    bool resultCentre = _detectCentre(avgAreaCentre);
    bool resultCornerShading = _detectCornerShading(avgAreaCorner);
    bool resultCornerDiff = _detectCornerDiff(avgAreaCorner);
    if(resultCentre && resultCornerDiff && resultCornerShading){
        resultBool = true;
    }
    return resultBool;
}

int shading::_avgPixel(int begin_x, int begin_y, int rectWidth, int rectHeight){
    cv::Rect imageRect(begin_x, begin_y, rectWidth, rectHeight);
    cv::Mat imageMat = image(imageRect);
    unsigned long long int accumulatePixel=0;
    int numPixel = rectWidth*rectHeight;
    for(int i=0; i<rectHeight; i++){
        for (int j=0; j<rectWidth; j++){
            accumulatePixel += imageMat.at<int>(i, j);
        }
    }
    int avgPixel =  accumulatePixel/numPixel;
    return avgPixel;
};

bool shading::_detectCentre(int avgAreaCentre){
    int CENTER_UP = static_cast<int>(algorithmConf["Center_Up"]);
    int CENTER_LOW = static_cast<int>(algorithmConf["Center_Low"]);
    if(avgAreaCentre<CENTER_UP && avgAreaCentre>CENTER_LOW){
        return true;
    } else {
        return false;
    }
}

bool shading::_detectCornerShading(std::vector<int> avgAreaVec){
    int PASSLEVEL = algorithmConf["PassLevel"];
    int PASSLEVEL_UP = algorithmConf["PassLevel_Up"];
    bool detectResult = true;
    for (int avgArea:avgAreaVec){
        detectResult = ( detectResult&& (avgArea>PASSLEVEL )&& ( avgArea<PASSLEVEL_UP ) );
    }
    return detectResult;
}

bool shading::_detectCornerDiff(std::vector<int> avgAreaVec){
    int DIFF = algorithmConf["Diff"];
    int maxVal = 0;
    int maxLoc = 0;
    int minVal = 256;
    int minLoc = 0;
    bool detectResult = false;
    for(int i = 0; i<avgAreaVec.size(); i++){
        if (avgAreaVec[i]>maxVal){
            maxVal = avgAreaVec[i];
            maxLoc = i;
        }
        if (avgAreaVec[i]<minVal){
            minVal = avgAreaVec[i];
            minLoc = i;
        }
    }
    if ((maxVal-minVal)< DIFF){
        detectResult = true;
    }
    return detectResult;
}
