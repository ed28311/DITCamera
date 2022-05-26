#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif

namespace DITCameraTool{
    namespace Algorithm{
        class Shading: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                Shading();
                Shading(DITCameraTool::Config, std::string);
                virtual cv::Mat loadImage() const override;
                virtual bool execute() const override;
            private:
                int _fetchAvgPixel(int, int, int, int) const;
                bool _detectCentre(int) const;
                bool _detectCornerShading(int, std::vector<int>) const;
                bool _detectCornerDiff(std::vector<int>) const;
        };
    }
}
