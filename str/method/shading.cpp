#include "shading.hpp"
#define _AppendAvgAreaCorner(jsonAvgAreaCorner, variable) _appendAvgAreaCorner((jsonAvgAreaCorner), #variable, (variable))

DITCameraTool::Algorithm::Shading::Shading(){
    cv::Mat image;
    std::string imagePath;
    DITCameraTool::Config DITConfig;
}


DITCameraTool::Algorithm::Shading::Shading(DITCameraTool::Config config, std::string filePath){
    imagePath = filePath;
    image = loadImage();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    debugMode = _getDebugMode();
}

cv::Mat DITCameraTool::Algorithm::Shading::loadImage() const {
    cv::Mat figure = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    cv::Size figureSize(figure.size());
    if (figure.empty()){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
}

bool DITCameraTool::Algorithm::Shading::execute() const {
    bool resultBool = false;
    float BLOCKRATIO = std::stof((std::string)algorithmConf["BlockRatio"]); //BlockRatio = 0.1;
    cv::Size imageSize(image.size());
    int imageH = imageSize.height;
    int imageW = imageSize.width;
    int blockH = static_cast<float>(imageH)*BLOCKRATIO;
    int blockW = static_cast<float>(imageW)*BLOCKRATIO;
    int avgAreaLT = _fetchAvgPixel(0, 0, blockW, blockH);
    int avgAreaLB = _fetchAvgPixel(0, imageH-blockH, blockW, blockH);
    int avgAreaRT = _fetchAvgPixel(imageW-blockW, 0, blockW, blockH);
    int avgAreaRB = _fetchAvgPixel(imageW-blockW, imageH-blockH, blockW, blockH);
    int avgAreaCentre = _fetchAvgPixel(imageW/4, imageH/4, imageW/2, imageH/2);
    json avgAreaCorner;
    _collectAvgAreaCorner(avgAreaCorner, avgAreaLB, avgAreaLT, avgAreaRB, avgAreaRT);
    // std::vector<int> avgAreaCorner{avgAreaLB, avgAreaLT, avgAreaRB, avgAreaRT};
    bool resultCentre = _detectCentre(avgAreaCentre);
    bool resultCornerShading = _detectCornerShading(avgAreaCentre, avgAreaCorner);
    bool resultCornerDiff = _detectCornerDiff(avgAreaCorner);
    if(debugMode){
        _PrintVariable(resultCentre);
        _PrintVariable(resultCornerShading);
        _PrintVariable(resultCornerDiff);
    }
    if(resultCentre && resultCornerDiff && resultCornerShading){
        resultBool = true;
    }
    std::cout << (resultBool?"PASS":"NOT PASS") << std::endl;
    return resultBool;
}



void DITCameraTool::Algorithm::Shading::_collectAvgAreaCorner(json &jsonAvgAreaCorner, int avgAreaLB, int avgAreaLT, int avgAreaRB, int avgAreaRT) const{
    _AppendAvgAreaCorner(jsonAvgAreaCorner, avgAreaLB);
    _AppendAvgAreaCorner(jsonAvgAreaCorner, avgAreaLT);
    _AppendAvgAreaCorner(jsonAvgAreaCorner, avgAreaRB);
    _AppendAvgAreaCorner(jsonAvgAreaCorner, avgAreaRT);
}
void DITCameraTool::Algorithm::Shading::_appendAvgAreaCorner(json &jsonAvgAreaCorner,std::string name, int val) const{
    jsonAvgAreaCorner[name] = val;
}

int DITCameraTool::Algorithm::Shading::_fetchAvgPixel(int begin_x, int begin_y, int rectWidth, int rectHeight) const{
    cv::Rect imageRect(begin_x, begin_y, rectWidth, rectHeight);
    cv::Mat imageMat = image(imageRect);
    unsigned long long int accumulatePixel=0;
    int numPixel = rectWidth*rectHeight;
    for(int i=0; i<rectHeight; i++){
        for (int j=0; j<rectWidth; j++){
            accumulatePixel += imageMat.at<uchar>(i, j);
        }
    }
    int avgPixel =  accumulatePixel/numPixel;
    return avgPixel;
};

bool DITCameraTool::Algorithm::Shading::_detectCentre (int avgAreaCentre) const{
    bool resultBool = false;
    int CENTER_UP = std::stoi((std::string)algorithmConf["Center_Up"]);
    int CENTER_LOW = std::stoi((std::string)algorithmConf["Center_Low"]);
    if(avgAreaCentre<CENTER_UP && avgAreaCentre>CENTER_LOW){
        resultBool = true;
    }
    logElement.at("UCL").get_to(CENTER_UP);
    logElement["LCL"] = CENTER_LOW;
    logElement["ITEM"] = std::string("_detectCentre");

    return resultBool;
}

bool DITCameraTool::Algorithm::Shading::_detectCornerShading(int avgAreaCentre, json avgAreaVec) const{
    int PASSLEVEL = std::stoi((std::string)algorithmConf["PassLevel"]);
    int PASSLEVEL_UP = std::stoi((std::string)algorithmConf["PassLevel_Up"]);
    bool detectResult = true;
    for (int avgArea:avgAreaVec){
        double avgAreaShading = 100*(static_cast<double>(avgArea)/static_cast<double>(avgAreaCentre));
        detectResult = ( detectResult&& (avgAreaShading>PASSLEVEL )&& ( avgAreaShading<PASSLEVEL_UP ) );
    }
    return detectResult;
}

bool DITCameraTool::Algorithm::Shading::_detectCornerDiff(json avgAreaList) const {
    int DIFF = std::stoi((std::string)algorithmConf["Diff"]);
    int maxVal = 0;
    std::string maxKey;
    int minVal = 256;
    std::string minKey;
    bool detectResult = false;
    for(const auto& item:avgAreaList.items()){
        if (item.value()>maxVal){
            maxKey = item.key();
            maxVal = item.value();
        }
        if (item.value()<minVal){
            minKey = item.key();
            minVal = item.value();
        }
    }
    if (debugMode){
        printf("maxKey: %s, minKey: %s\n", maxKey.c_str(), minKey.c_str());
    }
    if ((maxVal-minVal)< DIFF){
        detectResult = true;
    }
    return detectResult;
}
