#include "algorithm_base.hpp"


DITCameraTool::Algorithm::AlgorithmBase::AlgorithmBase() 
{
	m_is_print_debug_info = false;
	m_image = NULL;
}
DITCameraTool::Algorithm::AlgorithmBase::AlgorithmBase(const DITCameraTool::Config config) 
{
	m_image = NULL;
	m_is_print_debug_info = _GetDebugMode();
	m_algorithm_config = config.GetAlgorithmConf();
	m_global_config = config.GetGlobalConf();
}
DITCameraTool::Algorithm::AlgorithmBase::~AlgorithmBase(){
	FreeImage();
}
bool DITCameraTool::Algorithm::AlgorithmBase::Execute(DITCameraTool::Reporter&) const
{
	return true;
}

void DITCameraTool::Algorithm::AlgorithmBase::LoadImage(std::string  image_path)
{
	m_image_path = image_path;
	m_image_name = _GetImageFileName();
	cv::Mat figure = cv::imread(m_image_path, cv::IMREAD_GRAYSCALE);
	if (figure.empty())
	{
		throw std::invalid_argument("Invalid m_image path. ("+ m_image_path+ ")");
	}
	int STRIDE = std::stoi(const_cast<AlgorithmBase*>(this)->m_algorithm_config.LoadJsonKey("Stride"));

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

void DITCameraTool::Algorithm::AlgorithmBase::FreeImage() const{
	delete m_image;
	const_cast<AlgorithmBase*>(this)->m_image = NULL;
}
std::string DITCameraTool::Algorithm::AlgorithmBase::GenerateImage(cv::Mat* image, std::string item_name, DITCameraTool::Reporter& reporter) const{
	std::string file_generate_path;
	if (m_is_generate_image)
	{
		std::string export_dir = reporter.m_file_dir;
		std::string file_name = reporter.m_serial_num + "-" + reporter.m_current_datetime + "-" + const_cast<AlgorithmBase*>(this)->m_algorithm_config.LoadJsonKey("ConfigMode") + "-" + m_image_name + "-" + item_name + ".jpg";
		if (m_is_print_debug_info)
		{
			_PrintVariable(export_dir);
			_PrintVariable(file_name);
		}
		file_generate_path = export_dir + file_name;
		cv::imwrite(export_dir + file_name, *image);
	}
	return file_generate_path;
}

json DITCameraTool::Algorithm::AlgorithmBase::InitializeReportRow(DITCameraTool::Reporter& reporter) const
{
	json new_report_row;
	for (int i = 0; i < reporter.m_report_cols.size(); i++) 
	{
		new_report_row[reporter.m_report_cols[i]] = " ";
	}
	const_cast<AlgorithmBase*>(this)->m_report_row = new_report_row;
	return new_report_row;
}
void DITCameraTool::Algorithm::AlgorithmBase::WriteReportRow(std::string key, std::string val) const
{
	const_cast<AlgorithmBase*>(this)->m_report_row[key] = val;
}
void DITCameraTool::Algorithm::AlgorithmBase::SubmitReport(json m_report_row, DITCameraTool::Reporter& reporter) const
{
	DITCameraTool::Reporter *reporter_p = &reporter;
	reporter.WriteBack(m_report_row);
	m_report_row = InitializeReportRow(reporter);
}
void DITCameraTool::Algorithm::AlgorithmBase::FinishReport(json m_report_row, DITCameraTool::Reporter& reporter) const
{
	reporter.WriteFront(m_report_row);
	m_report_row = InitializeReportRow(reporter);
	// add new line.
	reporter.WriteBack(this->m_report_row);
}


std::string DITCameraTool::Algorithm::AlgorithmBase::_GetImageFileName() const
{
	std::smatch sm;
	std::regex_search(m_image_path, sm, std::regex("(?=([^\\/^\\\\]+$))[\\w]+"));
	std::string imageName = sm[0];
	return imageName;
}
bool DITCameraTool::Algorithm::AlgorithmBase::_GetDebugMode() const 
{
	return std::stoi(const_cast<AlgorithmBase*>(this)->m_global_config.LoadJsonKey("OutputPrintDebugInfo"));
}

void DITCameraTool::Algorithm::AlgorithmBase::_AttachReportRowBasicInfo(DITCameraTool::Reporter& reporter) const
{
	WriteReportRow("SPEC_NAME", const_cast<AlgorithmBase*>(this)->m_global_config.LoadJsonKey("SpecName"));
	WriteReportRow("DATE_TIME", reporter.m_current_date);
	WriteReportRow("OBJ_NAME", const_cast<AlgorithmBase*>(this)->m_algorithm_config.LoadJsonKey("ConfigMode"));
	WriteReportRow("STATUS", std::to_string(0));
}