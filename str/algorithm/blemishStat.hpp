#include "algorithm_base.hpp"

namespace DITCameraTool{
    namespace Algorithm{
        class BlemishStat: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                BlemishStat();
                BlemishStat(DITCameraTool::Config);
                virtual bool Execute() const override;
                virtual void LoadImage(std::string) override;
            private:
                void _statisticPixel(cv::Mat, std::vector<int>&) const;
                float _evalPixelStandardDeviation(cv::Mat, std::vector<int>) const;
                bool _detectStd(cv::Mat, std::vector<int>) const;
        };
    }
}