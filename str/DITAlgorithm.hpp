#ifndef DITALGOLOAD
#define DITALGOLOAD
#include <iostream>
#include "DITConfig.hpp"
#include "logger.hpp"
#include <regex>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp> 
#include "tool/displayer.hpp"

namespace DITCameraTool{
    namespace Algorithm{
        class AlgorithmBase{
            public:
                AlgorithmBase();
                AlgorithmBase(DITCameraTool::Config, std::string);
                // virtual ~AlgorithmBase() = default;
                virtual cv::Mat loadImage() const;
                std::string imagePath;
                std::string imageName;
                cv::Mat image;
                bool debugMode;
                json logElement;

                virtual bool execute (DITCameraTool::Logger&) const;
                virtual void submitLog(json, DITCameraTool::Logger&) const;
                virtual void writeLog(std::string, std::string) const;
                virtual void finishLog(json, DITCameraTool::Logger&) const;
                void logInitialize(DITCameraTool::Logger&);
            protected:
                std::string getImageFileName();
                DITCameraTool::Logger _loadLogger();
                json algorithmConf;
                json globalConf;
                bool _getDebugMode() const;
        };
    }
}
#endif