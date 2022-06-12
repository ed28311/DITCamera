#include "blemish.hpp"

DITCameraTool::Algorithm::Blemish::Blemish(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

DITCameraTool::Algorithm::Blemish::Blemish(DITCameraTool::Config config, std::string filePath){
    imagePath = filePath;
    imageName = getImageFileName();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    debugMode = _getDebugMode();
    isImageGenerate = (std::stoi((std::string)globalConf["OutputAllImages"]));
    if(debugMode){
        _PrintVariable(isImageGenerate);
        _PrintVariable(config);
    }
    image = loadImage();
}

cv::Mat DITCameraTool::Algorithm::Blemish::loadImage() const{
    cv::Mat figure = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    cv::Size figureSize(figure.size());
    if (figure.empty()){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    int stride = std::stoi((std::string)algorithmConf["Stride"]);

    cv::Mat strideFig(figure.rows/stride, figure.cols/stride, CV_8U);
    for(int i=0; i<(figure.rows/stride); i++){
        for(int j=0; j<(figure.cols/stride); j++){
            strideFig.at<uchar>(i,j)=figure.at<uchar>(i*stride, j*stride);
        }
    }
    if(debugMode){
        _PrintVariable(stride);
        _PrintVariable(strideFig.rows);
        _PrintVariable(strideFig.cols);
    }
    return strideFig;
}

bool DITCameraTool::Algorithm::Blemish::execute(DITCameraTool::Logger& logger) const{
    DITCameraTool::Logger& DITLogger = logger;
    const_cast<Blemish*>(this)->logInitialize(DITLogger);
    if(isImageGenerate){
        std::string exportDir = DITLogger.fileDir;
        std::string fileName = DITLogger.serialNums+"-"+DITLogger.currentDatetime+"-"+(std::string)algorithmConf["configMode"]+"-"+imageName+"-"+"stride.jpg";
        if(debugMode){
            _PrintVariable(exportDir);
            _PrintVariable(fileName);
        }
        cv::imwrite(exportDir+fileName, image);
    }
    bool blemishResult = true;
    bool resultBlemish = _detectBlemish(image, DITLogger);
    blemishResult = (blemishResult&&resultBlemish);
    _PrintVariable(blemishResult);
    if(DITLogger.logEnable){
        _attachBaseLogInfo(DITLogger);
        writeLog("ITEM","BlemishDetectResult");
        writeLog("RESULT",(blemishResult)?"PASS":"FAIL");
        writeLog("IMG", imagePath);
        finishLog(logElement, DITLogger);
    }
    return blemishResult;
}


bool DITCameraTool::Algorithm::Blemish::_detectBlemish(cv::Mat image, DITCameraTool::Logger& DITLogger) const{
    cv::Mat filterValueImg = fastDifferenceFiltering(image);
    cv::Mat filterImg = cv::Mat(filterValueImg.rows, filterValueImg.cols, CV_8U);
    if (debugMode){
        std::string exportDir = DITLogger.fileDir;
        std::string fileName = DITLogger.currentDatetime+"-"+(std::string)algorithmConf["configMode"]+"-"+imageName+"-"+"fastDifferenceFilteringRawMap.jpg";
        std::string filePath = exportDir+fileName;
        if(debugMode){
            _PrintVariable(exportDir);
            _PrintVariable(fileName);
        }
        cv::imwrite(exportDir+fileName, filterValueImg);
    }
    int filterSize = std::stoi((std::string)algorithmConf["FilterSize"]);
    filterValueImg = _2DFilter(filterValueImg, filterSize);
    filterValueImg = _2DFilter(filterValueImg, 2);
    if (debugMode){
        std::string exportDir = DITLogger.fileDir;
        std::string fileName = DITLogger.currentDatetime+"-"+(std::string)algorithmConf["configMode"]+"-"+imageName+"-"+"fastDifferenceFilteringProcessed.jpg";
        std::string filePath = exportDir+fileName;
        if(debugMode){
            _PrintVariable(exportDir);
            _PrintVariable(fileName);
        }
        cv::imwrite(exportDir+fileName, filterValueImg);
    }

    bool resultBool = true;
    int pixelTolerance = std::stoi((std::string)algorithmConf["PixelTolerance"]);
    int maxPixel = 0;
    for (int i=0; i<filterImg.rows; i++){
        for (int j=0; j<filterImg.cols; j++){
            if(filterValueImg.at<uchar>(i,j)<pixelTolerance){
                filterImg.at<uchar>(i,j) = 0;
            }
            else{
                if(filterValueImg.at<uchar>(i,j)>maxPixel){
                    maxPixel = filterValueImg.at<uchar>(i,j);
                }
                filterImg.at<uchar>(i,j) = 255;
                resultBool = false;
            }
        }
    }
    if(debugMode){
        _PrintVariable(pixelTolerance);
        _PrintVariable(maxPixel);
    }


    _attachBaseLogInfo(DITLogger);
    writeLog("ITEM", "detectBlemish");
    writeLog("UCL", std::to_string(pixelTolerance));
    writeLog("VALUE", std::to_string(maxPixel));
    writeLog("RESULT", ((resultBool))?"PASS":"FAIL");
    if(isImageGenerate){
        std::string exportDir = DITLogger.fileDir;
        std::string fileName = DITLogger.currentDatetime+"-"+(std::string)algorithmConf["configMode"]+"-"+imageName+"-"+"fastDifferenceFiltering.jpg";
        std::string filePath = exportDir+fileName;
        if(debugMode){
            _PrintVariable(exportDir);
            _PrintVariable(fileName);
        }
        cv::imwrite(exportDir+fileName, filterImg);
        writeLog("IMG", (std::string)filePath);
    }
    submitLog(logElement, DITLogger);
    return resultBool;
}

/*
Reference: Blemish detection in camera production testing using fast difference filtering.
*/
cv::Mat DITCameraTool::Algorithm::Blemish::fastDifferenceFiltering(cv::Mat image) const{
    float intervalRatio = std::stof((std::string)algorithmConf["SelectIntervalRatio"]);
    if(debugMode){
        _PrintVariable(intervalRatio);
    }
    int interval = static_cast<int>(image.cols*intervalRatio);
    int rangeVal = interval/2;
    cv::Mat filterImg = cv::Mat(image.rows, image.cols, CV_8U);
    for(int i=0; i<image.rows; i++){
        for (int j=0; j<image.cols; j++){
            int xLRange = (i<rangeVal)?i:rangeVal;
            int xRRange = (i>(image.rows-rangeVal-1))?image.rows-i-1:rangeVal;
            int yBRange = (j<1)?j:1;
            int yTRange = (j>=(image.cols-1))?image.cols-j-1:1;
            std::vector<int> medianLVec = {image.at<uchar>(i-xLRange, j-yBRange), image.at<uchar>(i-xLRange, j), image.at<uchar>(i-xLRange, j+yTRange)};
            std::vector<int> medianRVec = {image.at<uchar>(i+xRRange, j-yBRange), image.at<uchar>(i+xRRange, j), image.at<uchar>(i+xRRange, j+yTRange)};
            int medianL = _findMedian(medianLVec);
            int medianR = _findMedian(medianRVec);
            int IMedian = (medianL+medianR)/2;
            int I = image.at<uchar>(i,j);
            filterImg.at<uchar>(i,j) = (IMedian<I)?0:(IMedian-I);
        }
    }
    return filterImg;
}


cv::Mat DITCameraTool::Algorithm::Blemish::_2DFilter(cv::Mat image, int filterSize) const{
    cv::Mat filteredImg(image.rows, image.cols, CV_8U);
    for (int i=0; i<image.rows; i++){
        for (int j=0; j<image.cols; j++){
            int xLRange = (j<filterSize)?j:filterSize;
            int yTRange = (i<filterSize)?i:filterSize;
            int xRRange = (j>(image.cols-filterSize-1))?image.cols-j-1:filterSize;
            int yBRange = (i>(image.rows-filterSize-1))?image.rows-i-1:filterSize;
            cv::Mat kernel = cv::Mat::ones( yBRange+yTRange+1, xLRange+xRRange+1, CV_32F)/(float)((xLRange+xRRange+1)*(yBRange+yTRange+1));
            cv::Rect imageRect = cv::Rect( j-xLRange, i-yTRange,   xLRange + xRRange+1, yTRange + yBRange+1);
            cv::Mat selectImage = image(imageRect);
            selectImage.convertTo(selectImage, CV_32F);

            float filterNum = selectImage.dot(kernel);
            filteredImg.at<uchar>(i,j) = (filterNum>255)?255:filterNum;
            // if(debugMode){
            //     _PrintVariable(image.at<uchar>(i,j));
            //     _PrintVariable((int)filterNum);
            // }
        }
    }
    return filteredImg;
}

int DITCameraTool::Algorithm::Blemish::_findMedian(std::vector<int> medianVec) const{
    int numbers = (int)medianVec.size();
    int pickLoc = (numbers%2)?(pickLoc/2+1):(pickLoc/2);
    std::sort(medianVec.begin(), medianVec.end(), std::greater<int>());
    return medianVec[pickLoc];
}

void DITCameraTool::Algorithm::Blemish::_attachBaseLogInfo(DITCameraTool::Logger& DITLogger) const{
    writeLog("SPEC_NAME",globalConf["SpecName"]);
    writeLog("DATE_TIME",DITLogger.currentDate);
    writeLog("OBJ_NAME",algorithmConf["configMode"]);
    writeLog("STATUS",std::to_string(0));
}
