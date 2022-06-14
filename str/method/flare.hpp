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
                virtual bool execute(DITCameraTool::Logger& ) const override;
                virtual cv::Mat loadImage() const override;
            private:
                void _statisticPixel(std::vector<int>&, cv::Mat) const;
                float _evalPixelStandardDeviation(std::vector<int>, int) const;
                bool _detectStd(std::vector<int>, DITCameraTool::Logger&) const;
                void _attachBaseLogInfo(DITCameraTool::Logger& ) const;
        };  
    }
}