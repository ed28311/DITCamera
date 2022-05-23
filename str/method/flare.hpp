#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


class flare: public DITAlgorithm{
    public:
        flare();
        flare(Config, std::string);
        ~flare();
        virtual bool execute() override;
        virtual cv::Mat loadImage() override;
};
