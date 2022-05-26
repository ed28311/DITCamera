#ifndef DITALGOLOAD
#define DITALGOLOAD
#define LOGGER(name) DITLogger(#name, (name))
#include <iostream>
#include "DITConfig.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

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
            protected:
                json algorithmConf;
                json globalConf;
        };
    }
}
#endif