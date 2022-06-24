#include "blemish.hpp"

DITCameraTool::Algorithm::Blemish::Blemish() 
{

}

DITCameraTool::Algorithm::Blemish::Blemish(DITCameraTool::Config config)
{
	m_algorithm_config = config.GetAlgorithmConf();
	m_global_config = config.GetGlobalConf();
	m_is_debug = _GetDebugMode();
	m_is_generate_image = (std::stoi((std::string)m_global_config["OutputAllImages"]));
	if (m_is_debug)
	{
		_PrintVariable(m_is_generate_image);
		_PrintVariable(config);
	}
}

void DITCameraTool::Algorithm::Blemish::LoadImage(std::string  image_path)
{
	m_image_path = image_path;
	m_image_name = _GetImageFileName();
	cv::Mat figure = cv::imread(m_image_path, cv::IMREAD_GRAYSCALE);
	if (figure.empty())
	{
		throw std::invalid_argument("Invalid m_image path. ("+ m_image_path+ ")");
	}
	int STRIDE = std::stoi((std::string)m_algorithm_config["Stride"]);

	cv::Mat strideFig(figure.rows/STRIDE, figure.cols/STRIDE, CV_8U);
	for(int i = 0; i<(figure.rows/STRIDE); i++)
	{
		for (int j = 0; j<(figure.cols/STRIDE); j++)	
		{
			strideFig.at<uchar>(i, j) = figure.at<uchar>(i*STRIDE, j*STRIDE);
		}
	}
	if (m_is_debug)
	{
		_PrintVariable(STRIDE);
		_PrintVariable(strideFig.rows);
		_PrintVariable(strideFig.cols);
	}
	FreeImage();
	m_image = new cv::Mat(strideFig);
}

bool DITCameraTool::Algorithm::Blemish::Execute(DITCameraTool::Reporter& reporter) const
{
	InitializeReportRow(reporter);
	GenerateImage(m_image, "stride", reporter);
	bool result_bool = true;
	bool resultBlemish = _detectBlemish(m_image, reporter);
	result_bool = (result_bool&&resultBlemish);
	printf("%s\n", result_bool? "Pass": "Not Pass");

	if (reporter.m_is_create_report)
	{
		_AttachReportRowBasicInfo(reporter);
		WriteReportRow("ITEM", "BlemishDetectResult");
		WriteReportRow("RESULT", (result_bool)? "PASS": "FAIL");
		WriteReportRow("IMG", m_image_path);
		FinishReport(m_report_row, reporter);
	}
	return result_bool;
}

bool DITCameraTool::Algorithm::Blemish::_detectBlemish(cv::Mat* image, DITCameraTool::Reporter& reporter) const
{
	cv::Mat image_copy = cv::Mat(*image);
	bool resultBool = true;
	int maxPixel = 0;
	const int INTENSITY_TOLERANCE = std::stoi((std::string)m_algorithm_config["IntensityTolerance"]);
	const int FILTER_SIZE = std::stoi((std::string)m_algorithm_config["FilterSize"]);
	
	image_copy = ExecuteFastDifferenceFilter(&image_copy);
	GenerateImage(&image_copy, "fastDiffFilterRaw", reporter);
	image_copy = _2DFilter(&image_copy, FILTER_SIZE);
	image_copy = _2DFilter(&image_copy, 2);
	GenerateImage(&image_copy, "fastDiffFilterProcessed", reporter);
	for (int i = 0; i<image_copy.rows; i++)
	{
		for (int j = 0; j<image_copy.cols; j++)
		{
			if (image_copy.at<uchar>(i, j)<INTENSITY_TOLERANCE)
			{
				image_copy.at<uchar>(i, j) = 0;
			}
			else
			{
				if (image_copy.at<uchar>(i, j)>maxPixel)
				{
					maxPixel = image_copy.at<uchar>(i, j);
				}
				image_copy.at<uchar>(i, j) = 255;
				resultBool = false;
			}
		}
	}
	if (m_is_debug)
	{
		_PrintVariable(INTENSITY_TOLERANCE);
		_PrintVariable(maxPixel);
	}

	std::string file_path = GenerateImage(&image_copy, "final", reporter);
	_AttachReportRowBasicInfo(reporter);
	WriteReportRow("IMG", file_path);
	WriteReportRow("ITEM", "detectBlemish");
	WriteReportRow("UCL", std::to_string(INTENSITY_TOLERANCE));
	WriteReportRow("VALUE", std::to_string(maxPixel));
	WriteReportRow("RESULT", ((resultBool))? "PASS": "FAIL");
	SubmitReport(m_report_row, reporter);
	return resultBool;
}

cv::Mat DITCameraTool::Algorithm::Blemish::ExecuteFastDifferenceFilter(cv::Mat* image) const
{
	/*
	Reference: Blemish detection in camera production testing using fast difference filtering.
	*/
	float INSPECT_RATIO = std::stof((std::string)m_algorithm_config["InspectRatio"]);
	if (m_is_debug)
	{
		_PrintVariable(INSPECT_RATIO);
	}
	int interval = static_cast<int>(m_image->cols*INSPECT_RATIO);
	int rangeVal = interval/2;
	cv::Mat filterImg = cv::Mat(image->rows, image->cols, CV_8U);
	for (int i = 0; i<image->rows; i++)
	{
		for (int j = 0; j<image->cols; j++)
		{
			int xLRange = (i<rangeVal)? i: rangeVal;
			int xRRange = (i>(image->rows-rangeVal-1))? image->rows-i-1: rangeVal;
			int yBRange = (j<1)? j: 1;
			int yTRange = (j >= (image->cols-1))? image->cols-j-1: 1;
			std::vector<int> medianLVec = { image->at<uchar>(i-xLRange, j-yBRange), image->at<uchar>(i-xLRange, j), image->at<uchar>(i-xLRange, j+ yTRange) };
			std::vector<int> medianRVec = { image->at<uchar>(i+ xRRange, j-yBRange), image->at<uchar>(i+ xRRange, j), image->at<uchar>(i+ xRRange, j+ yTRange) };
			int medianL = _findMedian(medianLVec);
			int medianR = _findMedian(medianRVec);
			int IMedian = (medianL+ medianR)/2;
			int I = image->at<uchar>(i, j);
			filterImg.at<uchar>(i, j) = (IMedian<I)? 0: (IMedian-I);
		}
	}
	return filterImg;
}


cv::Mat DITCameraTool::Algorithm::Blemish::_2DFilter(cv::Mat* image, int FILTER_SIZE) const
{
	cv::Mat filteredImg(image->rows, image->cols, CV_8U);
	for (int i = 0; i<image->rows; i++)
	{
		for (int j = 0; j<image->cols; j++)
		{
			int xLRange = (j<FILTER_SIZE)? j: FILTER_SIZE;
			int yTRange = (i<FILTER_SIZE)? i: FILTER_SIZE;
			int xRRange = (j>(image->cols-FILTER_SIZE-1))? image->cols-j-1: FILTER_SIZE;
			int yBRange = (i>(image->rows-FILTER_SIZE-1))? image->rows-i-1: FILTER_SIZE;
			int filterEnhancement = 2;
			cv::Mat kernel = cv::Mat::ones(yBRange+ yTRange+ 1, xLRange+ xRRange+ 1, CV_32F)/(float)((xLRange+ xRRange+ 1)*(yBRange+ yTRange+ 1))*filterEnhancement;
			cv::Rect imageRect = cv::Rect(j-xLRange, i-yTRange, xLRange+ xRRange+ 1, yTRange+ yBRange+ 1);
			cv::Mat selectImage = (*image)(imageRect);
			selectImage.convertTo(selectImage, CV_32F);

			float filterNum = selectImage.dot(kernel);
			filteredImg.at<uchar>(i, j) = (filterNum>255)? 255: filterNum;
			// if(m_is_debug){
			//     _PrintVariable(image.at<uchar>(i,j));
			//     _PrintVariable((int)filterNum);
			// }
		}
	}
	return filteredImg;
}

int DITCameraTool::Algorithm::Blemish::_findMedian(std::vector<int> medianVec) const
{
	int numbers = (int)medianVec.size();
	int pickLoc = (numbers%2)? (pickLoc/2+ 1): (pickLoc/2);
	std::sort(medianVec.begin(), medianVec.end(), std::greater<int>());
	return medianVec[pickLoc];
}


