#include "algorithm_base.hpp"

namespace DITCameraTool{
    namespace Algorithm{
        class Flare: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                Flare();
                Flare(const DITCameraTool::Config);
                virtual bool Execute() const override;
                virtual void LoadFigure(std::string) override;
            private:
                std::vector<int> StatisticIntensity(cv::Mat*, std::vector<int>) const;
                float EvalIntensityStandardDeviation(std::vector<int>, int) const;
                bool DetectIntensityStd(cv::Mat*) const;
        };  
    }
}