#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


namespace DITCameraTool{
    namespace Algorithm{
        class BlemishPixel: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                BlemishPixel();
                BlemishPixel(DITCameraTool::Config, std::string);
                virtual bool execute(DITCameraTool::Logger& ) const override;
                virtual cv::Mat loadImage() const override;
            private:
                bool _loopPixels(bool, std::vector<std::vector<int>>&) const;
                int _calcAbsVal(int) const;
        };
    }
}