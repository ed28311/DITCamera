#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


class blemish: public DITAlgorithm{
    public:
        blemish();
        ~blemish();
        blemish(Config, std::string);
        virtual bool execute() override;
        virtual cv::Mat loadImage() override;
};
