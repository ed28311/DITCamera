#include <iostream>
#include "config_object.hpp"

namespace DITCameraTool
{
    enum LOG_LEVELS
    {
        DEBUG = '1',
        INFO = '2',
        WARN = '3',
        ERROR = '4',
        FATAL = '5'
    };
    class Logger
    {
        public:
            Logger(json, std::string, DITCameraTool::LOG_LEVELS);
            const LOG_LEVELS m_LEVEL;
            const std::string m_message;
            bool m_is_print_message;
            bool m_is_write_log;
            bool WriteLog();
        private:
            json m_level_transformer;
            json m_global_config;
            std::string m_lowest_level;
            std::string m_log_directory;
            std::string m_current_datetime;
            std::string m_current_date;
            void LoadLevelTransformer();
            void GetLogInfo();
    };
}