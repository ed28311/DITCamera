#include "blemishStat.hpp"

DITCameraTool::Algorithm::BlemishStat::BlemishStat(DITCameraTool::Config config) {
	m_is_generate_image = (std::stoi((std::string)m_global_config["OutputAllImages"]));
	if (m_is_print_debug_info)
	{
		_PrintVariable(m_is_generate_image);
		_PrintVariable(config);
	}
}

void DITCameraTool::Algorithm::BlemishStat::LoadImage(std::string  image_path)
{
	m_image_path = image_path;
	m_image_name = _GetImageFileName();
	cv::Mat figure = cv::imread(m_image_path, cv::IMREAD_GRAYSCALE);
	if (figure.empty())
	{
		throw std::invalid_argument("Invalid m_p_image path. ("+ m_image_path+ ")");
	}
	int STRIDE = std::stoi((std::string)m_algorithm_config["Stride"]);

	cv::Mat stride_figure(figure.rows/STRIDE, figure.cols/STRIDE, CV_8U);
	for(int i = 0; i<(figure.rows/STRIDE); i++)
	{
		for (int j = 0; j<(figure.cols/STRIDE); j++)	
		{
			stride_figure.at<uchar>(i, j) = figure.at<uchar>(i*STRIDE, j*STRIDE);
		}
	}
	if (m_is_print_debug_info)
	{
		_PrintVariable(STRIDE);
		_PrintVariable(stride_figure.rows);
		_PrintVariable(stride_figure.cols);
	}
	FreeImage();
	m_p_image = new cv::Mat(stride_figure);
}

bool DITCameraTool::Algorithm::BlemishStat::Execute() const{
    cv::Mat hist;
    bool resultBool = false;
    int splitPartitions = std::stoi((std::string)m_algorithm_config["Split_Partition"]);
    int splitRows = m_p_image->rows/splitPartitions;
    int splitCols = m_p_image->cols/splitPartitions;
    int height;
    int width;
    if(m_is_print_debug_info){
        _PrintVariable(m_p_image->rows);
        _PrintVariable(m_p_image->cols);
        printf("-----\n");
    }
    for (int i=0; i<splitPartitions; i++){
        for(int j=0; j<splitPartitions; j++){
            std::vector<int> pixelStat(256, 0);
            height = splitRows;
            width = splitCols;
            
            if(i==splitPartitions-1){
                height = m_p_image->rows-i*splitRows;
            }
            if(j==splitPartitions-1){
                width = m_p_image->cols-j*splitCols;
            }
            if(m_is_print_debug_info){
                _PrintVariable(i); 
                _PrintVariable(j);
                _PrintVariable(width);
                _PrintVariable(height);
                printf("-----\n");
            }   
            cv::Rect splitRectangle(j*splitCols, i*splitRows, width, height);
            cv::Mat splitImage= (*m_p_image)(splitRectangle);
            _statisticPixel(splitImage, pixelStat);
            bool detectResult = _detectStd(splitImage, pixelStat);
            resultBool = (resultBool||detectResult);
        }
    }
    
    
    printf("%s\n", resultBool?"Pass":"Not Pass");
    return resultBool;
}


void DITCameraTool::Algorithm::BlemishStat::_statisticPixel(cv::Mat image, std::vector<int> &pixelStat) const {
    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++){
            pixelStat[image.at<uchar>(i, j)] += 1;
        }
    }
}

bool DITCameraTool::Algorithm::BlemishStat::_detectStd(cv::Mat figure, std::vector<int> pixelArray) const {
    bool resultBool = false;
    float pixelStd = _evalPixelStandardDeviation(figure, pixelArray);
    float stdThreshold = std::stof((std::string)m_algorithm_config["Std_Threshold"]);
    if (m_is_print_debug_info){
        _PrintVariable(pixelStd);
        printf("-----Done\n");
    }
    if (pixelStd < stdThreshold){
        resultBool = true;
    }
    return resultBool;
}

float DITCameraTool::Algorithm::BlemishStat::_evalPixelStandardDeviation(cv::Mat figure, std::vector<int> intArray) const {
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
    if(m_is_print_debug_info){
        _PrintVariable((int)intArray.size());
        _PrintVariable(mean);
        _PrintVariable(meanSquare);
        _PrintVariable(meanSquare - (mean*mean));
        printf("-----.\n");
    }
    float standardDeviation = sqrt(meanSquare - (mean*mean));
    return standardDeviation;
}

