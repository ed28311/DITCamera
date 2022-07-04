#ifndef ALGO_BLEMISH_HPP
#define ALGO_BLEMISH_HPP
#include "algorithm_base.hpp"
namespace DITCameraTool{
    namespace Algorithm{
        class Blemish: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                Blemish();
                Blemish(DITCameraTool::Config);
                virtual bool Execute() const override;
                virtual void LoadImage(std::string) override;
                cv::Mat ExecuteFastDifferenceFilter(cv::Mat *) const;
            private:
                bool DetectBlemish(cv::Mat*) const;
                int FindMedian(std::vector<int>) const;
                cv::Mat Pass2DFilter(cv::Mat*, int) const;
        };
    }
}
#endif