#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


namespace DITCameraTool{
    namespace Algorithm{
        class BlemishStat: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                BlemishStat();
                BlemishStat(DITCameraTool::Config, std::string);
                virtual bool execute(DITCameraTool::Logger& ) const override;
                virtual cv::Mat loadImage() const override;
            private:
                void _statisticPixel(cv::Mat, std::vector<int>&) const;
                float _evalPixelStandardDeviation(cv::Mat, std::vector<int>) const;
                bool _detectStd(cv::Mat, std::vector<int>) const;
        };
    }
}