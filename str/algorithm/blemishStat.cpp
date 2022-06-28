#include "blemishStat.hpp"

DITCameraTool::Algorithm::BlemishStat::BlemishStat(DITCameraTool::Config config) {
	m_algorithm_config = config.GetAlgorithmConf();
	m_global_config = config.GetGlobalConf();
<<<<<<< HEAD
	m_is_print_debug_info = _GetDebugMode();
	m_is_generate_image = (std::stoi((std::string)m_global_config["OutputAllImages"]));
	if (m_is_print_debug_info)
=======
	m_is_debug = _GetDebugMode();
	m_is_generate_image = (std::stoi((std::string)m_global_config["OutputAllImages"]));
	if (m_is_debug)
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
	{
		_PrintVariable(m_is_generate_image);
		_PrintVariable(config);
	}
}

<<<<<<< HEAD
void DITCameraTool::Algorithm::BlemishStat::LoadImage(std::string  image_path)
{
=======
void DITCameraTool::Algorithm::BlemishStat::LoadImage(std::string image_path) {
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
	m_image_path = image_path;
	m_image_name = _GetImageFileName();
	cv::Mat figure = cv::imread(m_image_path, cv::IMREAD_GRAYSCALE);
	if (figure.empty())
	{
<<<<<<< HEAD
		throw std::invalid_argument("Invalid m_image path. ("+ m_image_path+ ")");
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
	m_image = new cv::Mat(stride_figure);
}

=======
		throw std::invalid_argument("Invalid m_image path. (" + m_image_path + ")");
	}
	int stride = std::stoi((std::string)m_algorithm_config["Stride"]);

	cv::Mat stride_figure(figure.rows / stride, figure.cols / stride, CV_8U);
	for(int i = 0; i < (figure.rows / stride); i++)
	{
		for (int j = 0; j < (figure.cols / stride); j++)	
		{
			stride_figure.at<uchar>(i, j) = figure.at<uchar>(i*stride, j*stride);
		}
	}

	if (m_is_debug)
	{
		_PrintVariable(stride);
		_PrintVariable(stride_figure.rows);
		_PrintVariable(stride_figure.cols);
	}
	*m_image = stride_figure;
}


>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
bool DITCameraTool::Algorithm::BlemishStat::Execute(DITCameraTool::Reporter& reporter) const{
    cv::Mat hist;
    bool resultBool = false;
    int splitPartitions = std::stoi((std::string)m_algorithm_config["Split_Partition"]);
    int splitRows = m_image->rows/splitPartitions;
    int splitCols = m_image->cols/splitPartitions;
    int height;
    int width;
<<<<<<< HEAD
    if(m_is_print_debug_info){
=======
    if(m_is_debug){
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
        _PrintVariable(m_image->rows);
        _PrintVariable(m_image->cols);
        printf("-----\n");
    }
    for (int i=0; i<splitPartitions; i++){
        for(int j=0; j<splitPartitions; j++){
            std::vector<int> pixelStat(256, 0);
            height = splitRows;
            width = splitCols;
            
            if(i==splitPartitions-1){
                height = m_image->rows-i*splitRows;
            }
            if(j==splitPartitions-1){
                width = m_image->cols-j*splitCols;
            }
<<<<<<< HEAD
            if(m_is_print_debug_info){
=======
            if(m_is_debug){
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
                _PrintVariable(i); 
                _PrintVariable(j);
                _PrintVariable(width);
                _PrintVariable(height);
                printf("-----\n");
            }   
            cv::Rect splitRectangle(j*splitCols, i*splitRows, width, height);
            cv::Mat splitImage= (*m_image)(splitRectangle);
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
<<<<<<< HEAD
    if (m_is_print_debug_info){
=======
    if (m_is_debug){
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
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
<<<<<<< HEAD
    if(m_is_print_debug_info){
=======
    if(m_is_debug){
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
        _PrintVariable((int)intArray.size());
        _PrintVariable(mean);
        _PrintVariable(meanSquare);
        _PrintVariable(meanSquare - (mean*mean));
        printf("-----.\n");
    }
    float standardDeviation = sqrt(meanSquare - (mean*mean));
    return standardDeviation;
}

