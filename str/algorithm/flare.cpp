#include "flare.hpp"
#include <cmath>

DITCameraTool::Algorithm::Flare::Flare(){}

DITCameraTool::Algorithm::Flare::Flare(DITCameraTool::Config config)
{
	m_algorithm_config = config.GetAlgorithmConf();
	m_global_config = config.GetGlobalConf();
	m_is_print_debug_info = _GetDebugMode();
	m_is_generate_image = (std::stoi(const_cast<Flare*>(this)->m_global_config.LoadJsonKey("OutputAllImages")));
	if (m_is_print_debug_info)
	{
		_PrintVariable(m_is_generate_image);
		_PrintVariable(config);
	}
}

void DITCameraTool::Algorithm::Flare::LoadImage(std::string  image_path)
{
	m_image_path = image_path;
	m_image_name = _GetImageFileName();
	cv::Mat figure = cv::imread(m_image_path, cv::IMREAD_GRAYSCALE);
	if (figure.empty())
	{
		throw std::invalid_argument("Invalid mp_image path. ("+ m_image_path+ ")");
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
	mp_image = new cv::Mat(stride_figure);
}

bool DITCameraTool::Algorithm::Flare::Execute(DITCameraTool::Reporter& reporter) const
{
	InitializeReportRow(reporter);
	bool result_bool = _DetectIntensityStd(mp_image, reporter);
	if (reporter.m_is_create_report)
	{
		_AttachReportRowBasicInfo(reporter);
		WriteReportRow("ITEM", "FlareDetectResult");
		WriteReportRow("RESULT", (result_bool) ? "PASS" : "FAIL");
		WriteReportRow("IMG", m_image_path);
		std::string spec_info = m_algorithm_config.dump(-1);
		std::replace(spec_info.begin(), spec_info.end(), ',', ' ');
		WriteReportRow("OTHERS", spec_info);
		FinishReport(m_report_row, reporter);
	}
	printf("%s\n", result_bool ? "Pass" : "Not Pass");
	return result_bool;
}

std::vector<int> DITCameraTool::Algorithm::Flare::_StatisticIntensity(cv::Mat* image, std::vector<int> intensity_stat) const {
	std::vector<int> intensity_stat_copy = std::vector<int>(intensity_stat);
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			intensity_stat_copy[image->at<uchar>(i, j)] += 1;
		}
	}
	return intensity_stat_copy;
}

bool DITCameraTool::Algorithm::Flare::_DetectIntensityStd(cv::Mat* image, DITCameraTool::Reporter& reporter) const {
	std::vector<int> intensity_stat(256, 0);
	std::string function_name = "DetectIntensityStd";
	bool result_bool = false;

	intensity_stat = _StatisticIntensity(image, intensity_stat);
	int num_of_pixel = image->rows*image->cols;
	float intensity_std = _EvalIntensityStandardDeviation(intensity_stat, num_of_pixel);
	float std_threshold = std::stof(const_cast<Flare*>(this)->m_algorithm_config.LoadJsonKey("Std_Threshold"));
	if (m_is_print_debug_info)
	{
		_PrintVariable(std_threshold);
		_PrintVariable(intensity_std);
	}
	if (intensity_std > std_threshold)
	{
		result_bool = true;
	}

	if (reporter.m_is_create_report)
	{
		_AttachReportRowBasicInfo(reporter);
		WriteReportRow("ITEM", function_name);
		WriteReportRow("VALUE", std::to_string(intensity_std));
		WriteReportRow("LCL", std::to_string(std_threshold));
		WriteReportRow("RESULT", (result_bool) ? "PASS" : "FAIL");
		SubmitReport(m_report_row, reporter);
	}
	return result_bool;
}

float DITCameraTool::Algorithm::Flare::_EvalIntensityStandardDeviation(std::vector<int> intensityDist, int num_of_pixel) const {
	float mean = 0;
	float mean_square = 0;
	for (int i = 0; i < intensityDist.size(); i++)
	{
		mean += (intensityDist[i] / static_cast<float>(num_of_pixel))*i;
		mean_square += ((intensityDist[i] / static_cast<float>(num_of_pixel))*(i*i));
	}
	if (m_is_print_debug_info)
	{
		_PrintVariable((int)intensityDist.size());
		_PrintVariable(mean);
		_PrintVariable(mean_square);
		_PrintVariable(mean_square - (mean*mean));
	}
	float standard_deviation = sqrt(mean_square - (mean*mean));
	return standard_deviation;
}
