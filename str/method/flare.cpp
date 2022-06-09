#include "flare.hpp"
#include <cmath>

DITCameraTool::Algorithm::Flare::Flare(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

cv::Mat DITCameraTool::Algorithm::Flare::loadImage() const {
    cv::Mat figure = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    cv::Size figureSize(figure.size());
    if(debugMode){
        _PrintVariable(figureSize.width);
        _PrintVariable(figureSize.height);
    }
    if (figure.empty()){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
}

DITCameraTool::Algorithm::Flare::Flare(DITCameraTool::Config config, std::string filePath){
    imagePath = filePath;
    if(debugMode){
        _PrintVariable(imagePath);
    }
    image = loadImage();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    debugMode = _getDebugMode();
    if(debugMode){
        _PrintVariable(globalConf.dump(4));
        _PrintVariable(algorithmConf.dump(4));
    }
}

bool DITCameraTool::Algorithm::Flare::execute(DITCameraTool::Logger& logger) const {
    DITCameraTool::Logger& DITLogger = logger;
    const_cast<Flare*>(this)->logInitialize(DITLogger);
    cv::Mat hist;
    std::vector<int> pixelStat(256, 0);
    _statisticPixel(pixelStat, image);
    bool resultStd = _detectStd(pixelStat, logger);
    if(DITLogger.logEnable){
        _attachBaseLogInfo(DITLogger);
        writeLog("ITEM","FlareDetectResult");
        writeLog("RESULT",(resultStd)?"PASS":"FAIL");
        writeLog("IMG", imagePath);
        finishLog(logElement, DITLogger);
    }
    if(debugMode){
        _PrintVariable((int)logger.logComponent.size());
    }
    printf("%s\n", resultStd?"Pass":"Not Pass");
    return resultStd;
}

void DITCameraTool::Algorithm::Flare::_statisticPixel(std::vector<int> &pixelStat, cv::Mat image) const {
    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++){
            pixelStat[image.at<uchar>(i, j)] += 1;
        }
    }
}

bool DITCameraTool::Algorithm::Flare::_detectStd(std::vector<int> pixelArray, DITCameraTool::Logger& DITLogger) const {
    bool resultBool = false;
    int imagePixelNums = image.rows*image.cols;
    float pixelStd = _evalPixelStandardDeviation(pixelArray, imagePixelNums);
    float stdThreshold = std::stof((std::string)algorithmConf["Std_Threshold"]);
    if (debugMode){
        _PrintVariable(stdThreshold);
        _PrintVariable(pixelStd);   
    }
    if (pixelStd > stdThreshold){
        resultBool = true;
    }

    if(DITLogger.logEnable){
        _attachBaseLogInfo(DITLogger);
        writeLog("ITEM", "detectStd");
        writeLog("VALUE", std::to_string(pixelStd));
        writeLog("LCL", std::to_string(stdThreshold));
        writeLog("RESULT", (resultBool)?"PASS":"FAIL");
        submitLog(logElement, DITLogger);
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
    if(debugMode){
        _PrintVariable((int)intArray.size());
        _PrintVariable(mean);
        _PrintVariable(meanSquare);
        _PrintVariable(meanSquare - (mean*mean));
    }
    float standardDeviation = sqrt(meanSquare - (mean*mean));
    return standardDeviation;
}

void DITCameraTool::Algorithm::Flare::_attachBaseLogInfo(DITCameraTool::Logger& DITLogger) const{
    writeLog("SPEC_NAME",globalConf["SpecName"]);
    writeLog("DATE_TIME",DITLogger.currentDate);
    writeLog("OBJ_NAME",algorithmConf["configMode"]);
    writeLog("STATUS",std::to_string(0));
}

