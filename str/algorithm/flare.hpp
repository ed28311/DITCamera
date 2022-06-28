#include "algorithm_base.hpp"

namespace DITCameraTool{
    namespace Algorithm{
        class Flare: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                Flare();
                Flare(DITCameraTool::Config);
                virtual bool Execute(DITCameraTool::Reporter&) const override;
                virtual void LoadImage(std::string) override;
            private:
                std::vector<int> _StatisticIntensity(cv::Mat*, std::vector<int>) const;
                float _EvalIntensityStandardDeviation(std::vector<int>, int) const;
                bool _DetectIntensityStd(cv::Mat*, DITCameraTool::Reporter&) const;
        };  
    }
}