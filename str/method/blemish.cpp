#include "blemish.hpp"

DITCameraTool::Algorithm::Blemish::Blemish(){
    cv::Mat image;
    std::string imagePath;
    Config DITConfig;
}

DITCameraTool::Algorithm::Blemish::Blemish(DITCameraTool::Config config, std::string filePath){
    imagePath = filePath;
    image = loadImage();
    algorithmConf = config.getAlgorithmConf();
    globalConf = config.getGlobalConf();
    debugMode = _getDebugMode();
}

cv::Mat DITCameraTool::Algorithm::Blemish::loadImage() const{
    cv::Mat figure = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    cv::Size figureSize(figure.size());
    if (figure.empty()){
        throw std::invalid_argument("Invalid image path. ("+imagePath+")");
    }
    return figure;
}

bool DITCameraTool::Algorithm::Blemish::execute() const{
    cv::Mat hist;
    bool resultBool = false;
    int splitPartitions = std::stoi((std::string)algorithmConf["Split_Partition"]);
    int splitRows = image.rows/splitPartitions;
    int splitCols = image.cols/splitPartitions;
    int height;
    int width;
    if(debugMode){
        _PrintVariable(image.rows);
        _PrintVariable(image.cols);
        printf("-----\n");
    }
    for (int i=0; i<splitPartitions; i++){
        for(int j=0; j<splitPartitions; j++){
            std::vector<int> pixelStat(256, 0);
            height = splitRows;
            width = splitCols;
            
            if(i==splitPartitions-1){
                height = image.rows-i*splitRows;
            }
            if(j==splitPartitions-1){
                width = image.cols-j*splitCols;
            }
            if(debugMode){
                _PrintVariable(i); 
                _PrintVariable(j);
                _PrintVariable(width);
                _PrintVariable(height);
                printf("-----\n");
            }   
            cv::Rect splitRectangle(j*splitCols, i*splitRows, width, height);
            cv::Mat splitImage= image(splitRectangle);
            _statisticPixel(splitImage, pixelStat);
            bool detectResult = _detectStd(splitImage, pixelStat);
            resultBool = (resultBool||detectResult);
        }
    }
    
    
    printf("%s\n", resultBool?"Pass":"Not Pass");
    return resultBool;
}


void DITCameraTool::Algorithm::Blemish::_statisticPixel(cv::Mat image, std::vector<int> &pixelStat) const {
    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++){
            pixelStat[image.at<uchar>(i, j)] += 1;
        }
    }
}

bool DITCameraTool::Algorithm::Blemish::_detectStd(cv::Mat figure, std::vector<int> pixelArray) const {
    bool resultBool = false;
    float pixelStd = _evalPixelStandardDeviation(figure, pixelArray);
    float stdThreshold = std::stof((std::string)algorithmConf["Std_Threshold"]);
    if (debugMode){
        _PrintVariable(pixelStd);
        printf("-----Done\n");
    }
    if (pixelStd < stdThreshold){
        resultBool = true;
    }
    return resultBool;
}

float DITCameraTool::Algorithm::Blemish::_evalPixelStandardDeviation(cv::Mat figure, std::vector<int> intArray) const {
    int imagePixelNums = figure.rows*figure.cols;
    float imagePixelNumsF = static_cast<float>(imagePixelNums);
    float mean = 0;
    float meanSquare = 0;
    for (int i=0; i<intArray.size(); i++){
        mean += (intArray[i]/imagePixelNumsF)*i;
        meanSquare += ((intArray[i]/imagePixelNumsF)*(i*i));
        if(false){
            printf("Pixel: %d, # of pxiels: %d\n", i, intArray[i]);
        }
    }
    if(debugMode){
        _PrintVariable((int)intArray.size());
        _PrintVariable(mean);
        _PrintVariable(meanSquare);
        _PrintVariable(meanSquare - (mean*mean));
        printf("-----.\n");
    }
    float standardDeviation = sqrt(meanSquare - (mean*mean));
    return standardDeviation;
}

