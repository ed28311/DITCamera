#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


class blemish: public DITAlgorithm{
    public:
        blemish();
        blemish(Config, std::string);
        bool execute() ;
        cv::Mat loadImage();
};
