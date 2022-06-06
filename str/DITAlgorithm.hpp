#ifndef DITALGOLOAD
#define DITALGOLOAD
#define LOGGER(name) DITLogger(#name, (name))
#include <iostream>
#include "DITConfig.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "tool/displayer.hpp"
#define _PrintVariable(variable) DITCameraTool::Display().printVariable(#variable, (variable))

namespace DITCameraTool{
    namespace Algorithm{
        class AlgorithmBase{
            public:
                AlgorithmBase();
                AlgorithmBase(DITCameraTool::Config, std::string);
                virtual ~AlgorithmBase() = default;
                virtual bool execute () const;
                virtual cv::Mat loadImage() const;
                std::string imagePath;
                cv::Mat image;
                bool test_mode;
            protected:
                json algorithmConf;
                json globalConf;
        };
    }
}
#endif