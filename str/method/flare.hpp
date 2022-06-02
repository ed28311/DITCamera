#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif

namespace DITCameraTool{
    namespace Algorithm{
        class Flare: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                Flare();
                Flare(DITCameraTool::Config, std::string);
                virtual bool execute() const override;
                virtual cv::Mat loadImage() const override;
        };
    }
}