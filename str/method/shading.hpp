#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


class shading: public DITAlgorithm{
    public:
        shading();
        shading(Config, std::string);
        cv::Mat loadImage();
        bool execute();
    private:
        int _avgPixel(int, int, int, int);
        bool _detectCentre(int);
        bool _detectCornerShading(std::vector<int>);
        bool _detectCornerDiff(std::vector<int>);
};