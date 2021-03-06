#ifndef REPORTER_HPP
#define REPORTER_HPP
#include <iostream>
#include <fstream>
#include <deque>
#include <ctime>
#include <filesystem>
#include "config_object.hpp"
#include "tool/displayer.hpp"
namespace DITCameraTool
{
    class Reporter
    {
        public:
            Reporter();
            Reporter(DITCameraTool::Config, std::vector<std::string>);
            void WriteBack(json);
            void WriteFront(json);
            bool GenerateCSV();
            int m_max_column;
            bool m_is_create_report;
            std::string m_serial_num;
            std::string m_file_dir;
            std::string m_file_name;
            std::deque<std::vector<std::string>> m_report_component;
            std::vector<std::string> m_report_cols;
            std::string m_current_datetime;
            std::string m_current_date;
            void MergeReporter(DITCameraTool::Reporter);

        private:
            json m_global_config;
            int AddReportHeader();
            std::string CreateCSVLine(std::vector<std::string>);
            std::vector<std::string> UpdateReportCols(std::vector<std::string>);
            json UpdateReportConfig(json);
            void SetupReportEnable();
            void SetupCurrentTime();
            void SetupFileInfo();
            void CheckDirRoot(std::string);
    };
}
#endif