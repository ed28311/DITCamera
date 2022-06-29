#include "blemish.hpp"

DITCameraTool::Algorithm::Blemish::Blemish() 
{

}

DITCameraTool::Algorithm::Blemish::Blemish(DITCameraTool::Config config)
{
	m_algorithm_config = config.GetAlgorithmConf();
	m_global_config = config.GetGlobalConf();
	m_is_print_debug_info = _GetDebugMode();
	m_is_generate_image = (std::stoi(const_cast<Blemish*>(this)->m_global_config.LoadJsonKey("OutputAllImages")));
	if (m_is_print_debug_info)
	{
		_PrintVariable(m_is_generate_image);
		_PrintVariable(config);
	}
}

void DITCameraTool::Algorithm::Blemish::LoadImage(std::string  image_path)
{
	FreeImage();
	m_image_path = image_path;
	m_image_name = _GetImageFileName();
	cv::Mat figure = cv::imread(m_image_path, cv::IMREAD_GRAYSCALE);
	if (figure.empty())
	{
		throw std::invalid_argument("Invalid image path. ("+ m_image_path+ ")");
	}
	const int STRIDE = std::stoi(m_algorithm_config.LoadJsonKey("Stride"));

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
	mp_image = new cv::Mat(stride_figure);
}

bool DITCameraTool::Algorithm::Blemish::Execute(DITCameraTool::Reporter& reporter) const
{
	InitializeReportRow(reporter);
	GenerateImage(mp_image, "stride", reporter);
	bool result_bool = true;
	bool resultBlemish = _DetectBlemish(mp_image, reporter);
	result_bool = (result_bool&&resultBlemish);
	printf("%s\n", result_bool? "Pass": "Not Pass");

	if (reporter.m_is_create_report)
	{
		_AttachReportRowBasicInfo(reporter);
		WriteReportRow("ITEM", "BlemishDetectResult");
		WriteReportRow("RESULT", (result_bool)? "PASS": "FAIL");
		WriteReportRow("IMG", m_image_path);
		std::string spec_info = m_algorithm_config.dump(-1);
		std::replace(spec_info.begin(), spec_info.end(), ',', ' ');
		WriteReportRow("OTHERS", spec_info);
		WriteReportRow("OTHERS", spec_info);
		FinishReport(m_report_row, reporter);
	}
	return result_bool;
}

bool DITCameraTool::Algorithm::Blemish::_DetectBlemish(cv::Mat* image, DITCameraTool::Reporter& reporter) const
{
	cv::Mat image_copy = cv::Mat(*image);
	bool resultBool = true;
	int maxPixel = 0;
	const int INTENSITY_TOLERANCE = std::stoi(const_cast<Blemish*>(this)->m_algorithm_config.LoadJsonKey("IntensityTolerance"));
	const int FILTER_SIZE = std::stoi(const_cast<Blemish*>(this)->m_algorithm_config.LoadJsonKey("FilterSize"));
	
	image_copy = ExecuteFastDifferenceFilter(&image_copy);
	GenerateImage(&image_copy, "fastDiffFilterRaw", reporter);
	image_copy = _Pass2DFilter(&image_copy, FILTER_SIZE);
	image_copy = _Pass2DFilter(&image_copy, 2);
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
	if (m_is_print_debug_info)
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
	float INSPECT_RATIO = std::stof(const_cast<Blemish*>(this)->m_algorithm_config.LoadJsonKey("InspectRatio"));
	if (m_is_print_debug_info)
	{
		_PrintVariable(INSPECT_RATIO);
	}
	int interval = static_cast<int>(mp_image->cols*INSPECT_RATIO);
	int expand_distance = interval/2;
	cv::Mat filted_image = cv::Mat(image->rows, image->cols, CV_8U);
	for (int i = 0; i<image->rows; i++)
	{
		for (int j = 0; j<image->cols; j++)
		{
			int x_l = (i<expand_distance)? i: expand_distance;
			int x_r = (i>(image->rows-expand_distance-1))? image->rows-i-1: expand_distance;
			int y_b = (j<1)? j: 1;
			int x_t = (j >= (image->cols-1))? image->cols-j-1: 1;
			std::vector<int> median_left_vec = { image->at<uchar>(i-x_l, j-y_b), image->at<uchar>(i-x_l, j), image->at<uchar>(i-x_l, j+ x_t) };
			std::vector<int> median_right_vec = { image->at<uchar>(i+ x_r, j-y_b), image->at<uchar>(i+ x_r, j), image->at<uchar>(i+ x_r, j+ x_t) };
			int median_l = _FindMedian(median_left_vec);
			int median_r = _FindMedian(median_right_vec);
			int I_median = (median_l+ median_r)/2;
			int I = image->at<uchar>(i, j);
			filted_image.at<uchar>(i, j) = (I_median<I)? 0: (I_median-I);
		}
	}
	return filted_image;
}


cv::Mat DITCameraTool::Algorithm::Blemish::_Pass2DFilter(cv::Mat* image, int FILTER_SIZE) const
{
	cv::Mat filtered_image(image->rows, image->cols, CV_8U);
	for (int i = 0; i<image->rows; i++)
	{
		for (int j = 0; j<image->cols; j++)
		{
			int x_l = (j<FILTER_SIZE)? j: FILTER_SIZE;
			int x_t = (i<FILTER_SIZE)? i: FILTER_SIZE;
			int x_r = (j>(image->cols-FILTER_SIZE-1))? image->cols-j-1: FILTER_SIZE;
			int y_b = (i>(image->rows-FILTER_SIZE-1))? image->rows-i-1: FILTER_SIZE;
			const int FILTER_ENHANCE = 2;
			cv::Mat kernel = cv::Mat::ones(y_b+ x_t+ 1, x_l+ x_r+ 1, CV_32F)/(float)((x_l+ x_r+ 1)*(y_b+ x_t+ 1))*FILTER_ENHANCE;
			cv::Rect image_rect = cv::Rect(j-x_l, i-x_t, x_l+ x_r+ 1, x_t+ y_b+ 1);
			cv::Mat image_crop = (*image)(image_rect);
			image_crop.convertTo(image_crop, CV_32F);

			float intesity_value = image_crop.dot(kernel);
			filtered_image.at<uchar>(i, j) = (intesity_value>255)? 255: intesity_value;
			// if(m_is_print_debug_info){
			//     _PrintVariable(image.at<uchar>(i,j));
			//     _PrintVariable((int)intesity_value);
			// }
		}
	}
	return filtered_image;
}

int DITCameraTool::Algorithm::Blemish::_FindMedian(std::vector<int> intesity_vec) const
{
	int vec_nums = (int)intesity_vec.size();
	int pick_loc = (vec_nums%2)? (vec_nums/2+ 1): (vec_nums/2);
	std::sort(intesity_vec.begin(), intesity_vec.end(), std::greater<int>());
	return intesity_vec[pick_loc];
}


