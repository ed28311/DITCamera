#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


class shading: public DITAlgorithm{
    public:
        ~shading();
        shading();
        shading(Config, std::string);
        virtual cv::Mat loadImage() override;
        virtual bool execute() override;
    private:
        int _avgPixel(int, int, int, int);
        bool _detectCentre(int);
        bool _detectCornerShading(int, std::vector<int>);
        bool _detectCornerDiff(std::vector<int>);
};