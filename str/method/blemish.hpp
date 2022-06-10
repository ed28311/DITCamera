#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


namespace DITCameraTool{
    namespace Algorithm{
        class Blemish: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                Blemish();
                Blemish(DITCameraTool::Config, std::string);
                bool isImageGenerate;
                virtual bool execute(DITCameraTool::Logger& ) const override;
                virtual cv::Mat loadImage() const override;
                cv::Mat fastDifferenceFiltering(cv::Mat) const;
                std::string imageName;
            private:
                bool _detectBlemish(cv::Mat, DITCameraTool::Logger&) const;
                int _findMedian(std::vector<int>) const;
                void _attachBaseLogInfo(DITCameraTool::Logger& ) const;
        };
    }
}