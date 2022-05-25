#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


class shading: public DITAlgorithm{
    public:
        shading();
        shading(Config, std::string);
        virtual cv::Mat loadImage() const override;
        virtual bool execute() const override;
    private:
        int _avgPixel(int, int, int, int) const;
        bool _detectCentre(int) const;
        bool _detectCornerShading(int, std::vector<int>) const;
        bool _detectCornerDiff(std::vector<int>) const;
};