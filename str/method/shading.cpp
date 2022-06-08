#include "shading.hpp"
#define _AppendAvgAreaCorner(jsonAvgAreaCorner, variable) _appendAvgAreaCorner((jsonAvgAreaCorner), #variable, (variable))


DITCameraTool::Algorithm::Shading::Shading(DITCameraTool::Config config, std::string filePath){
    imagePath = filePath;
    image = loadImage();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    debugMode = _getDebugMode();
    _PrintVariable(debugMode);
    if(debugMode){
        _PrintVariable(globalConf.dump(4));
        _PrintVariable(algorithmConf.dump(4));
    }
}

cv::Mat DITCameraTool::Algorithm::Shading::loadImage() const {
    cv::Mat figure = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    cv::Size figureSize(figure.size());
    if (figure.empty()){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
}

bool DITCameraTool::Algorithm::Shading::execute(DITCameraTool::Logger& logger) const {
    DITCameraTool::Logger& DITLogger = logger;
    const_cast<Shading*>(this)->logInitialize(DITLogger);
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
    bool resultCentre = _detectCentre(avgAreaCentre, DITLogger);
    bool resultCornerShading = _detectCornerShading(avgAreaCentre, avgAreaCorner, DITLogger);
    bool resultCornerDiff = _detectCornerDiff(avgAreaCorner, DITLogger);
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

bool DITCameraTool::Algorithm::Shading::_detectCentre (int avgAreaCentre, DITCameraTool::Logger& DITLogger) const{
    bool resultBool = false;
    int CENTER_UP = std::stoi((std::string)algorithmConf["Center_Up"]);
    int CENTER_LOW = std::stoi((std::string)algorithmConf["Center_Low"]);
    if(avgAreaCentre<CENTER_UP && avgAreaCentre>CENTER_LOW){
        resultBool = true;
    }
    if(DITLogger.logEnable){
        _attachBaseLogInfo(DITLogger);
        writeLog("UCL",std::to_string(CENTER_UP));
        writeLog("LCL",std::to_string(CENTER_LOW));
        writeLog("ITEM","detectCentre");
        writeLog("VALUE", std::to_string(avgAreaCentre));
        writeLog("RESULT",(resultBool)?"PASS":"FAIL");
        submitLog(logElement, DITLogger);
    }
    return resultBool;
}


bool DITCameraTool::Algorithm::Shading::_detectCornerShading(int avgAreaCentre, json avgAreaList, DITCameraTool::Logger& DITLogger) const{
    int PASSLEVEL = std::stoi((std::string)algorithmConf["PassLevel"]);
    int PASSLEVEL_UP = std::stoi((std::string)algorithmConf["PassLevel_Up"]);
    bool resultBool = true;
    for (const auto& item:avgAreaList.items()){
        int avgArea = item.value();
        std::string avgAreaKey = item.key();
        double avgAreaShading = 100*(static_cast<double>(avgArea)/static_cast<double>(avgAreaCentre));
        resultBool = ( resultBool&& (avgAreaShading>PASSLEVEL )&& ( avgAreaShading<PASSLEVEL_UP ) );
        if(DITLogger.logEnable){
            _attachBaseLogInfo(DITLogger);
            writeLog("UCL",std::to_string(PASSLEVEL));
            writeLog("LCL",std::to_string(PASSLEVEL_UP));
            writeLog("ITEM",avgAreaKey);
            writeLog("VALUE", std::to_string(avgAreaShading));
            writeLog("RESULT", ((avgAreaShading>PASSLEVEL )&& ( avgAreaShading<PASSLEVEL_UP ))?"PASS":"FAIL");
            submitLog(logElement, DITLogger);
        }
    }
    if(DITLogger.logEnable){
        _attachBaseLogInfo(DITLogger);
        writeLog("UCL",std::to_string(PASSLEVEL));
        writeLog("LCL",std::to_string(PASSLEVEL_UP));
        writeLog("ITEM","detectCornerShading");
        writeLog("RESULT",(resultBool)?"PASS":"FAIL");
        submitLog(logElement, DITLogger);
    }
    return resultBool;
}

bool DITCameraTool::Algorithm::Shading::_detectCornerDiff(json avgAreaList, DITCameraTool::Logger& DITLogger) const {
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
    if ((maxVal-minVal)< DIFF){
        detectResult = true;
    }
    if(DITLogger.logEnable){
        _attachBaseLogInfo(DITLogger);
        writeLog("UCL",std::to_string(DIFF));
        writeLog("ITEM","detectCornerDiff");
        writeLog("VALUE", std::to_string(maxVal-minVal));
        writeLog("RESULT", ((detectResult))?"PASS":"FAIL");
        submitLog(logElement, DITLogger);
    }
    return detectResult;
}

void DITCameraTool::Algorithm::Shading::_attachBaseLogInfo(DITCameraTool::Logger& DITLogger) const{
    writeLog("SPEC_NAME",globalConf["SpecName"]);
    writeLog("DATE_TIME",DITLogger.currentDate);
    writeLog("OBJ_NAME",algorithmConf["mode"]);
    writeLog("STATUS",std::to_string(0));
}

