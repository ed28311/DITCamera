#include "shading.hpp"
#define _Append_Avg_Area_Corner(jsonAvgAreaCorner, variable) _AppendCornerAvgIntensity((jsonAvgAreaCorner), #variable, (variable))


DITCameraTool::Algorithm::Shading::Shading(DITCameraTool::Config config)
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

void DITCameraTool::Algorithm::Shading::LoadImage(std::string  image_path)
{
	m_image_path = image_path;
	m_image_name = _GetImageFileName();
	cv::Mat figure = cv::imread(m_image_path, cv::IMREAD_GRAYSCALE);
	if (figure.empty())
	{
		throw std::invalid_argument("Invalid m_image path. (" + m_image_path + ")");
	}
	int stride = std::stoi((std::string)m_algorithm_config["Stride"]);

	cv::Mat strideFig(figure.rows / stride, figure.cols / stride, CV_8U);
	for(int i = 0; i < (figure.rows / stride); i++)
	{
		for (int j = 0; j < (figure.cols / stride); j++)	
		{
			strideFig.at<uchar>(i, j) = figure.at<uchar>(i*stride, j*stride);
		}
	}
	if (m_is_debug)
	{
		_PrintVariable(stride);
		_PrintVariable(strideFig.rows);
		_PrintVariable(strideFig.cols);
	}
	*m_image = strideFig;
}

bool DITCameraTool::Algorithm::Shading::Execute(DITCameraTool::Reporter& reporter) const {
	bool result_bool = false;
	InitializeReportRow(reporter);
	const float BLOCK_RATIO = std::stof((std::string)m_algorithm_config["BlockRatio"]); //BlockRatio = 0.1;

	int image_height = m_image->cols;
	int image_width = m_image->rows;
	int block_height = image_height*BLOCK_RATIO;
	int block_width = image_width*BLOCK_RATIO;

	int avg_intensity_lt = _FetchAvgPixel(0, 0, block_width, block_height);
	int avg_intensity_lb = _FetchAvgPixel(0, image_height - block_height, block_width, block_height);
	int avg_intensity_rt = _FetchAvgPixel(image_width - block_width, 0, block_width, block_height);
	int avg_intensity_rb = _FetchAvgPixel(image_width - block_width, image_height - block_height, block_width, block_height);
	int avg_intensity_center = _FetchAvgPixel(image_width / 4, image_height / 4, image_width / 2, image_height / 2);
	json avgAreaCorner = _CollectCornerAvgIntensity(avg_intensity_lb, avg_intensity_lt, avg_intensity_rb, avg_intensity_rt);
	bool resultCentre = _DetectCenterIntensity(avg_intensity_center, reporter);
	bool resultCornerShading = _DetectCornerShading(avg_intensity_center, avgAreaCorner, reporter);
	bool resultCornerDiff = _DetectCornerDiff(avgAreaCorner, reporter);
	if (m_is_debug) {
		_PrintVariable(resultCentre);
		_PrintVariable(resultCornerShading);
		_PrintVariable(resultCornerDiff);
	}
	if (resultCentre && resultCornerDiff && resultCornerShading) {
		result_bool = true;
	}
	if (reporter.m_is_create_report) {
		_AttachReportRowBasicInfo(reporter);
		WriteReportRow("ITEM", "ShadingDetectResult");
		WriteReportRow("RESULT", (result_bool) ? "PASS" : "FAIL");
		WriteReportRow("IMG", m_image_path);
		FinishReport(m_report_row, reporter);
	}
	std::cout << (result_bool ? "PASS" : "NOT PASS") << std::endl;
	return result_bool;
}



json DITCameraTool::Algorithm::Shading::_CollectCornerAvgIntensity(int avg_intensity_lb, int avg_intensity_lt, int avg_intensity_rb, int avg_intensity_rt) const {
	json jsonAvgAreaCorner;
	_Append_Avg_Area_Corner(jsonAvgAreaCorner, avg_intensity_lb);
	_Append_Avg_Area_Corner(jsonAvgAreaCorner, avg_intensity_lt);
	_Append_Avg_Area_Corner(jsonAvgAreaCorner, avg_intensity_rb);
	_Append_Avg_Area_Corner(jsonAvgAreaCorner, avg_intensity_rt);
	return jsonAvgAreaCorner;
}

void DITCameraTool::Algorithm::Shading::_AppendCornerAvgIntensity(json &jsonAvgAreaCorner, std::string name, int val) const {
	jsonAvgAreaCorner[name] = val;
}

int DITCameraTool::Algorithm::Shading::_FetchAvgPixel(int begin_x, int begin_y, int rectWidth, int rectHeight) const {
	cv::Rect imageRect(begin_x, begin_y, rectWidth, rectHeight);
	cv::Mat imageMat = (*m_image)(imageRect);
	unsigned long long int accumulatePixel = 0;
	int numPixel = rectWidth * rectHeight;
	for (int i = 0; i < rectHeight; i++) {
		for (int j = 0; j < rectWidth; j++) {
			accumulatePixel += imageMat.at<uchar>(i, j);
		}
	}
	int avgPixel = accumulatePixel / numPixel;
	return avgPixel;
};

bool DITCameraTool::Algorithm::Shading::_DetectCenterIntensity(int avg_intensity_center, DITCameraTool::Reporter& reporter) const {
	bool result_bool = false;
	int CENTER_UP = std::stoi((std::string)m_algorithm_config["Center_Up"]);
	int CENTER_LOW = std::stoi((std::string)m_algorithm_config["Center_Low"]);
	if (avg_intensity_center<CENTER_UP && avg_intensity_center>CENTER_LOW) {
		result_bool = true;
	}
	if (reporter.m_is_create_report) {
		_AttachReportRowBasicInfo(reporter);
		WriteReportRow("UCL", std::to_string(CENTER_UP));
		WriteReportRow("LCL", std::to_string(CENTER_LOW));
		WriteReportRow("ITEM", "detectCentre");
		WriteReportRow("VALUE", std::to_string(avg_intensity_center));
		WriteReportRow("RESULT", (result_bool) ? "PASS" : "FAIL");
		SubmitReport(m_report_row, reporter);
	}
	return result_bool;
}


bool DITCameraTool::Algorithm::Shading::_DetectCornerShading(int avg_intensity_center, json avgAreaList, DITCameraTool::Reporter& reporter) const {
	bool result_bool = true;
	int PASSLEVEL = std::stoi((std::string)m_algorithm_config["PassLevel"]);
	int PASSLEVEL_UP = std::stoi((std::string)m_algorithm_config["PassLevel_Up"]);
	
	for (const auto& item : avgAreaList.items()) {
		int avgArea = item.value();
		std::string avgAreaKey = item.key();
		double avgAreaShading = 100 * (static_cast<double>(avgArea) / static_cast<double>(avg_intensity_center));
		result_bool = (result_bool && (avgAreaShading > PASSLEVEL) && (avgAreaShading < PASSLEVEL_UP));
		if (reporter.m_is_create_report) {
			_AttachReportRowBasicInfo(reporter);
			WriteReportRow("UCL", std::to_string(PASSLEVEL));
			WriteReportRow("LCL", std::to_string(PASSLEVEL_UP));
			WriteReportRow("ITEM", avgAreaKey);
			WriteReportRow("VALUE", std::to_string(avgAreaShading));
			WriteReportRow("RESULT", ((avgAreaShading > PASSLEVEL) && (avgAreaShading < PASSLEVEL_UP)) ? "PASS" : "FAIL");
			SubmitReport(m_report_row, reporter);
		}
	}
	if (reporter.m_is_create_report) {
		_AttachReportRowBasicInfo(reporter);
		WriteReportRow("UCL", std::to_string(PASSLEVEL));
		WriteReportRow("LCL", std::to_string(PASSLEVEL_UP));
		WriteReportRow("ITEM", "detectCornerShading");
		WriteReportRow("RESULT", (result_bool) ? "PASS" : "FAIL");
		SubmitReport(m_report_row, reporter);
	}
	return result_bool;
}

bool DITCameraTool::Algorithm::Shading::_DetectCornerDiff(json avgAreaList, DITCameraTool::Reporter& reporter) const {
	int DIFF = std::stoi((std::string)m_algorithm_config["Diff"]);
	int maxVal = 0;
	std::string maxKey;
	int minVal = 256;
	std::string minKey;
	bool detectResult = false;
	for (const auto& item : avgAreaList.items()) {
		if (item.value() > maxVal) {
			maxKey = item.key();
			maxVal = item.value();
		}
		if (item.value() < minVal) {
			minKey = item.key();
			minVal = item.value();
		}
	}
	if ((maxVal - minVal) < DIFF) {
		detectResult = true;
	}
	if (reporter.m_is_create_report) {
		_AttachReportRowBasicInfo(reporter);
		WriteReportRow("UCL", std::to_string(DIFF));
		WriteReportRow("ITEM", "detectCornerDiff");
		WriteReportRow("VALUE", std::to_string(maxVal - minVal));
		WriteReportRow("RESULT", ((detectResult)) ? "PASS" : "FAIL");
		SubmitReport(m_report_row, reporter);
	}
	return detectResult;
}
