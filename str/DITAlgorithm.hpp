#ifndef DITALGOLOAD
#define DITALGOLOAD
#include <iostream>
#include <memory>
#include "DITConfig.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

class DITAlgorithm{
    public:
        DITAlgorithm(Config, std::string);
        virtual bool execute();
        virtual cv::Mat loadImage();
        cv::Mat image;
        std::string imagePath;
        Config DITConfig;
    protected:
        json algorithmConf;
        json globalConf;
};
#endif