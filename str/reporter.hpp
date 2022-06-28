#ifndef REPORTER_HPP
#define REPORTER_HPP
#include <iostream>
#include <fstream>
#include <deque>
#include <ctime>
#include <filesystem>
#include "config_object.hpp"
#include "tool/displayer.hpp"
<<<<<<< HEAD
=======

>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
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
            json m_global_config;
            std::string m_serial_num;
            std::string m_file_dir;
            std::string m_file_name;
            std::deque<std::vector<std::string>> m_report_component;
            std::vector<std::string> m_report_cols;
            std::string m_current_datetime;
            std::string m_current_date;
            void MergeReporter(DITCameraTool::Reporter);
<<<<<<< HEAD

        private:
=======
        private:
            bool _GetReportEnable();
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
            int _AddReportHeader();
            std::string _CreateCSVLine(std::vector<std::string>);
            std::vector<std::string> _UpdateReportCols(std::vector<std::string>);
            json _UpdateReportConfig(json);
<<<<<<< HEAD
            void _SetupReportEnable();
            void _SetupCurrentTime();
            void _SetupFileInfo();
=======
            void _GetCurrentTime();
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
            void _checkDirRoot(std::string);
    };
}
#endif