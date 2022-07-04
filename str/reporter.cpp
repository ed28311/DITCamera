#include "reporter.hpp"

DITCameraTool::Reporter::Reporter() 
{
	m_is_create_report = false;
	m_max_column = m_report_cols.size();
	SetupCurrentTime();
}

DITCameraTool::Reporter::Reporter(DITCameraTool::Config config, std::vector<std::string> basic_column) 
{
	m_global_config = config.GetGlobalConf();
	for(std::string item:basic_column)
	{
		std::cout << item << " ";
	}
	std::cout <<std::endl;
	m_report_cols = basic_column;
	m_max_column = m_report_cols.size();
	SetupReportEnable();
	SetupCurrentTime();
	SetupFileInfo();

}

void DITCameraTool::Reporter::WriteBack(json logElement) 
{
	std::vector<std::string> log_vec;
	for (std::string key : m_report_cols) 
	{
		log_vec.push_back((std::string)logElement[key]);
	}
	this->m_report_component.push_back(log_vec);
}
void DITCameraTool::Reporter::WriteFront(json logElement) 
{
	std::vector<std::string> log_vec;
	for (std::string key : m_report_cols) 
	{
		log_vec.push_back((std::string)logElement[key]);
	}
	this->m_report_component.push_front(log_vec);
}

std::string DITCameraTool::Reporter::CreateCSVLine(std::vector<std::string> log_vec) 
{
	std::string outputString = "";
	for (int i = 0; i < log_vec.size(); i++) 
	{
		outputString += (log_vec[i] + ",");
	}
	if (outputString.size() > 0) 
	{
		outputString.pop_back();
		outputString += "\n";
	}
	return outputString;
}
bool DITCameraTool::Reporter::GenerateCSV()
{
	AddReportHeader();
	bool resultBool = false;
	if (!m_is_create_report) 
	{
		return resultBool;
	}
	std::ofstream logFile;
	std::string csvPath = m_file_dir + m_file_name + ".csv";
	_PrintVariable(csvPath);
	logFile.open(csvPath);
	for (int i = 0; i < m_report_component.size(); i++) 
	{
		logFile << CreateCSVLine(m_report_component[i]);
	}
	logFile.close();
	resultBool = true;
	return resultBool;
}
void DITCameraTool::Reporter::MergeReporter(DITCameraTool::Reporter reporter) 
{
	m_is_create_report = (m_is_create_report || reporter.m_is_create_report);
	m_global_config = UpdateReportConfig(reporter.m_global_config);
	m_report_cols = UpdateReportCols(reporter.m_report_cols);
	m_max_column = m_report_cols.size();
	m_file_dir = (m_file_dir.size() == 0) ? (reporter.m_file_dir) : (m_file_dir);
	m_file_name = (m_file_name.size() == 0) ? (reporter.m_file_name) : (m_file_name);
	m_report_component.insert(m_report_component.end(), reporter.m_report_component.begin(), reporter.m_report_component.end());
}



int DITCameraTool::Reporter::AddReportHeader() 
{
	std::vector<std::string> reportDeclare(m_max_column);
	if (m_max_column > 2)
	{
		reportDeclare[0] = "Report_Version:" + m_global_config.LoadJsonKey("Version");
		reportDeclare[1] = "Project_Name:" + m_global_config.LoadJsonKey("Name");
	}
	this->m_report_component.push_front(m_report_cols);
	this->m_report_component.push_front(reportDeclare);
	return m_max_column;
}
void DITCameraTool::Reporter::CheckDirRoot(std::string m_file_dir) 
{
	#if (__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L) && (_MSC_VER >= 1913))
		std::filesystem::create_directories(m_file_dir);
	#else
		std::experimental::filesystem::create_directories(m_file_dir);
	#endif
};
void DITCameraTool::Reporter::SetupFileInfo()
{
	m_file_dir = m_global_config.LoadJsonKey("ReportDirectory") + "/" + m_current_date + "/";
	CheckDirRoot(m_file_dir);
	m_serial_num = m_global_config.LoadJsonKey("SerialNum");
	m_file_name = "Report-" + m_serial_num + "-" + m_current_datetime;
}
void DITCameraTool::Reporter::SetupCurrentTime() 
{
	std::time_t t = std::time(0);
	std::tm *now = std::localtime(&t);
	std::string currentYear = std::to_string(now->tm_year + 1900);
	std::string currentMonth = std::to_string(now->tm_mon + 1);
	if (currentMonth.size() == 1) currentMonth = "0" + currentMonth;
	std::string currentDay = std::to_string(now->tm_mday);
	if (currentDay.size() == 1) currentDay = "0" + currentDay;
	std::string currentHour = std::to_string(now->tm_hour);
	std::string currentMin = std::to_string(now->tm_min);
	std::string currentSec = std::to_string(now->tm_sec);
	m_current_date = currentYear + currentMonth + currentDay;
	m_current_datetime = currentYear + currentMonth + currentDay + currentHour + currentMin + currentSec;
}
void DITCameraTool::Reporter::SetupReportEnable() 
{
	m_is_create_report = std::stoi(m_global_config.LoadJsonKey("OutputDebugInfo"));
}

std::vector<std::string> DITCameraTool::Reporter::UpdateReportCols(std::vector<std::string> report_cols)
{	
	std::vector<std::string> m_report_cols_copy = std::vector<std::string>(m_report_cols);
	for (std::string item :report_cols) 
	{
		if (std::find(m_report_cols_copy.begin(), m_report_cols_copy.end(), item) == m_report_cols_copy.end()) 
		{
			m_report_cols_copy.push_back(item);
		}
	}
	return m_report_cols_copy;
}
json DITCameraTool::Reporter::UpdateReportConfig(json report_config)
{
	json m_config_copy = json(m_global_config);
	for (auto& item : report_config.items()) 
	{
		auto result = m_config_copy.find(item.key());
		if (result == m_config_copy.end()) 
		{
			m_config_copy[item.key()] = item.value();
		}
	}
	return m_config_copy;
}

