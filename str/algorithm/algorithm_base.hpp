#ifndef ALGORITHM_BASE_H
#define ALGORITHM_BASE_H
#include <iostream>
#include <regex>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp> 
#include "../tool/displayer.hpp"
#include "../config_object.hpp"
#include "../reporter.hpp"

namespace DITCameraTool
{
    namespace Algorithm
    {
        class AlgorithmBase
        {
            public:
                AlgorithmBase();
                AlgorithmBase(const DITCameraTool::Config);
                virtual ~AlgorithmBase(); // why virtual.
                virtual void FreeImage() const;
                virtual void LoadFigure(std::string) = 0;
                virtual bool Execute() const=0;
                std::string GenerateImage(cv::Mat* image, std::string item_name) const;

                json InitializeReportRow() const;
                void WriteReportRow(std::string key, std::string val) const;
                void SubmitReport(json) const;
                void FinishReport(json) const;
                DITCameraTool::Reporter GetReporter();
            protected:            
                std::vector<std::string> report_basic_property = {"ITEM", "STATUS", "VALUE", "UCL", "LCL", "RESULT", "SPEC_NAME", "DATE_TIME", "OBJ_NAME", "COMMENT", "OTHERS", "IMG" };
                std::string m_image_path;
                std::string m_image_name;
                bool m_is_generate_image;
                bool m_is_print_debug_info;
                cv::Mat *m_p_image;
                json m_algorithm_config;
                json m_global_config;
                std::string _GetImageFileName() const;
                bool GetDebugMode() const;

                DITCameraTool::Reporter* m_p_reporter=NULL;
                json m_report_row;
                void AttachReportRowBasicInfo() const;
        };
    }
}
#endif