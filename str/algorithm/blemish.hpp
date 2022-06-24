#ifndef ALGO_BLEMISH_HPP
#define ALGO_BLEMISH_HPP
#include "algorithm_base.hpp"
namespace DITCameraTool{
    namespace Algorithm{
        class Blemish: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                Blemish();
                Blemish(DITCameraTool::Config);
                virtual bool Execute(DITCameraTool::Reporter&) const override;
                virtual void LoadImage(std::string) override;
                cv::Mat ExecuteFastDifferenceFilter(cv::Mat *) const;
            private:
                bool _detectBlemish(cv::Mat*, DITCameraTool::Reporter&) const;
                int _findMedian(std::vector<int>) const;
                cv::Mat _2DFilter(cv::Mat*, int) const;
        };
    }
}
#endif