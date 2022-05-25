#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


class blemish: public DITAlgorithm{
    public:
        blemish();
        blemish(Config, std::string);
        virtual bool execute() const override;
        virtual cv::Mat loadImage() const override;
};
