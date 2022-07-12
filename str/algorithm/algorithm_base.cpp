#include "algorithm_base.hpp"


DITCameraTool::Algorithm::AlgorithmBase::AlgorithmBase() 
{
	m_p_reporter = NULL;
	m_is_print_debug_info = false;
	m_p_image = NULL;
}
DITCameraTool::Algorithm::AlgorithmBase::AlgorithmBase(const DITCameraTool::Config config) 
{
	m_algorithm_config = config.GetAlgorithmConf();
	m_global_config = config.GetGlobalConf();
	m_is_print_debug_info = GetDebugMode();
	m_is_generate_image = (std::stoi(const_cast<AlgorithmBase*>(this)->m_global_config.LoadJsonKey("OutputAllImages")));
	if (m_is_print_debug_info)
	{
		_PrintVariable(m_is_generate_image);
		_PrintVariable(config);
	}
	delete m_p_reporter;
	m_p_reporter = NULL;
	m_p_reporter = new DITCameraTool::Reporter(config, REPORT_BASIC_PROPERTY);
}
DITCameraTool::Algorithm::AlgorithmBase::~AlgorithmBase(){
	FreeImage();
	if(m_p_reporter != NULL)
	{
		delete m_p_reporter;
		m_p_reporter = NULL;
	}
}
DITCameraTool::Reporter DITCameraTool::Algorithm::AlgorithmBase::GetReporter()
{
	
	return *m_p_reporter;
}
void DITCameraTool::Algorithm::AlgorithmBase::FreeImage() const{
	if (m_p_image!=NULL)
	{
		delete m_p_image;
		const_cast<AlgorithmBase*>(this)->m_p_image = NULL;
	}
	}
std::string DITCameraTool::Algorithm::AlgorithmBase::GenerateImage(cv::Mat* image, std::string item_name) const{
	std::string file_generate_path;
	if (m_is_generate_image)
	{
		std::string export_dir = m_p_reporter->m_file_dir;
		std::string file_name = m_p_reporter->m_serial_num + "-" + m_p_reporter->m_current_datetime + "-" + const_cast<AlgorithmBase*>(this)->m_algorithm_config.LoadJsonKey("ConfigMode") + "-" + m_image_name + "-" + item_name + ".jpg";
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

json DITCameraTool::Algorithm::AlgorithmBase::InitializeReportRow() const
{
	json new_report_row;
	for (int i = 0; i < const_cast<DITCameraTool::Reporter*>(m_p_reporter)->m_report_cols.size(); i++) 
	{
		new_report_row[m_p_reporter->m_report_cols[i]] = " ";
	}
	const_cast<AlgorithmBase*>(this)->m_report_row = new_report_row;
	return new_report_row;
}
void DITCameraTool::Algorithm::AlgorithmBase::WriteReportRow(std::string key, std::string val) const
{
	const_cast<AlgorithmBase*>(this)->m_report_row[key] = val;
}
void DITCameraTool::Algorithm::AlgorithmBase::SubmitReport(json m_report_row) const
{
	m_p_reporter->WriteBack(m_report_row);
	m_report_row = InitializeReportRow();
}
void DITCameraTool::Algorithm::AlgorithmBase::FinishReport(json m_report_row) const
{
	m_p_reporter->WriteFront(m_report_row);
	m_report_row = InitializeReportRow();
	// add new line.
	m_p_reporter->WriteBack(this->m_report_row);
}


std::string DITCameraTool::Algorithm::AlgorithmBase::GetImageFileName() const
{
	std::smatch sm;
	std::regex_search(m_image_path, sm, std::regex("(?=([^\\/^\\\\]+$))[\\w]+"));
	std::string imageName = sm[0];
	return imageName;
}
bool DITCameraTool::Algorithm::AlgorithmBase::GetDebugMode() const 
{
	return std::stoi(const_cast<AlgorithmBase*>(this)->m_global_config.LoadJsonKey("OutputPrintDebugInfo"));
}

void DITCameraTool::Algorithm::AlgorithmBase::AttachReportRowBasicInfo() const
{
	WriteReportRow("SPEC_NAME", const_cast<AlgorithmBase*>(this)->m_global_config.LoadJsonKey("SpecName"));
	WriteReportRow("DATE_TIME", m_p_reporter->m_current_date);
	WriteReportRow("OBJ_NAME", const_cast<AlgorithmBase*>(this)->m_algorithm_config.LoadJsonKey("ConfigMode"));
	WriteReportRow("STATUS", std::to_string(0));
}