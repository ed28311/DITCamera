#include "logger.hpp"

DITCameraTool::Logger::Logger(json global_config, std::string message, DITCameraTool::LOG_LEVELS level):m_LEVEL(level), m_message(message)
{
    m_global_config = global_config;
    GetLogInfo();
    LoadLevelTransformer();
}

void DITCameraTool::Logger::GetLogInfo(){
    m_is_print_message = std::stoi(m_global_config.LoadJsonKey("OutputPrintDebugInfo"));
    m_is_write_log = std::stoi(m_global_config.LoadJsonKey("OutputLog"));
    m_log_directory = m_global_config.LoadJsonKey("LogDirectory");
    m_lowest_level = m_global_config.LoadJsonKey("LogLevel");
}

void DITCameraTool::Logger::LoadLevelTransformer()
{
    m_level_transformer['1'] = "DEBUG";
    m_level_transformer['2'] = " INFO";
    m_level_transformer['3'] = " WARN";
    m_level_transformer['4'] = "ERROR";
    m_level_transformer["5"] = "FATEL";
}
