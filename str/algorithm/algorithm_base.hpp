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
                AlgorithmBase(DITCameraTool::Config);
                virtual ~AlgorithmBase();
                std::string m_image_path;
                std::string m_image_name;
                bool m_is_generate_image;
<<<<<<< HEAD
                bool m_is_print_debug_info;
=======
                bool m_is_debug;
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
                json m_report_row;
                virtual void FreeImage() const;
                virtual void LoadImage(std::string);
                virtual bool Execute(DITCameraTool::Reporter&) const;
                std::string GenerateImage(cv::Mat* image, std::string item_name, DITCameraTool::Reporter& reporter) const;

                json InitializeReportRow(DITCameraTool::Reporter&) const;
                void WriteReportRow(std::string, std::string) const;
                void SubmitReport(json, DITCameraTool::Reporter&) const;
                void FinishReport(json, DITCameraTool::Reporter&) const;
            protected:
                cv::Mat *m_image;
                json m_algorithm_config;
                json m_global_config;
                std::string _GetImageFileName() const;
                void _AttachReportRowBasicInfo(DITCameraTool::Reporter&) const;
                bool _GetDebugMode() const;
        };
    }
}
#endif