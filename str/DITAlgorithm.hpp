#ifndef DITALGOLOAD
#define DITALGOLOAD
#include <iostream>
#include "DITConfig.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

class DITAlgorithm{
    public:
        DITAlgorithm();
        DITAlgorithm(Config, std::string);

        virtual ~DITAlgorithm() = default;
        virtual bool execute () const;
        virtual cv::Mat loadImage() const;
        std::string imagePath;
        cv::Mat image;
    protected:
        json algorithmConf;
        json globalConf;
};
#endif