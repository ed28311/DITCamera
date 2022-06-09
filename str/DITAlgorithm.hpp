#ifndef DITALGOLOAD
#define DITALGOLOAD
#define LOGGER(name) DITLogger(#name, (name))
#include <iostream>
#include "DITConfig.hpp"
#include "logger.hpp"
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
                cv::Mat image;
                bool debugMode;
                json logElement;

                virtual bool execute (DITCameraTool::Logger&) const;
                virtual void submitLog(json, DITCameraTool::Logger&) const;
                virtual void writeLog(std::string, std::string) const;
                virtual void finishLog(json, DITCameraTool::Logger&) const;
                void logInitialize(DITCameraTool::Logger&);
            protected:
                DITCameraTool::Logger _loadLogger();
                json algorithmConf;
                json globalConf;
                bool _getDebugMode() const;
        };
    }
}
#endif