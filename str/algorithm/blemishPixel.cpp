#include "blemishPixel.hpp"


DITCameraTool::Algorithm::BlemishPixel::BlemishPixel(DITCameraTool::Config config) {
	m_algorithm_config = config.GetAlgorithmConf();
	m_global_config = config.GetGlobalConf();
	m_is_print_debug_info = _GetDebugMode();
	m_is_generate_image = (std::stoi((std::string)m_global_config["OutputAllImages"]));
	if (m_is_print_debug_info)
	{
		_PrintVariable(m_is_generate_image);
		_PrintVariable(config);
	}
}

void DITCameraTool::Algorithm::BlemishPixel::LoadImage(std::string image_path) {
	const_cast<BlemishPixel*>(this)->m_image_path = image_path;
	const_cast<BlemishPixel*>(this)->m_image_name = const_cast<BlemishPixel*>(this)->_GetImageFileName();
	cv::Mat figure = cv::imread(m_image_path, cv::IMREAD_GRAYSCALE);
	if (figure.empty())
	{
		throw std::invalid_argument("Invalid mp_image path. (" + m_image_path + ")");
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

	if (m_is_print_debug_info)
	{
		_PrintVariable(stride);
		_PrintVariable(stride_figure.rows);
		_PrintVariable(stride_figure.cols);
	}
	FreeImage();
	mp_image = new cv::Mat(stride_figure);
}

bool DITCameraTool::Algorithm::BlemishPixel::Execute(DITCameraTool::Reporter& logger) const{
    DITCameraTool::Reporter& DITLogger = logger;
    std::vector<std::vector<int>> logObject;
    bool resultBool = _loopPixels(true, logObject);
    std::cout << resultBool << std::endl;
    return resultBool;
}


bool DITCameraTool::Algorithm::BlemishPixel::_loopPixels(bool displaySelectPixel, std::vector<std::vector<int>> &logObject) const {
    int stride = std::stoi((std::string)m_algorithm_config["Stride"]);
    int diffThres = std::stoi((std::string)m_algorithm_config["Diff_Tolarence"]);
    int selectPixel;
    int targetPixel;
    int maxPixelDiff = 0;
    int minPixelDiff = 255;
    int diffPixel;
    bool resultBool = true;
    for(int i=0; i<mp_image->rows; i+=stride){
        for(int j=0; j<mp_image->cols-1; j+=stride){
            int selectPixel = mp_image->at<uchar>(i,j);
            if((j+stride)>mp_image->cols){
                targetPixel = mp_image->at<uchar>(i,j+stride);
            }
            else{
                targetPixel = mp_image->at<uchar>(i,mp_image->cols);
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