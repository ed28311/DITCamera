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
                virtual bool execute(DITCameraTool::Logger&) const override;
            private:
                int _fetchAvgPixel(int, int, int, int) const;
                void _attachBaseLogInfo(DITCameraTool::Logger&) const;
                void _collectAvgAreaCorner(json &jsonAvgAreaCorner, int avgAreaLB, int avgAreaLT, int avgAreaRB, int avgAreaRT) const;
                void _appendAvgAreaCorner(json &jsonAvgAreaCorner,std::string name, int val) const;
                bool _detectCentre(int, DITCameraTool::Logger&) const;
                bool _detectCornerShading(int, json, DITCameraTool::Logger&) const;
                bool _detectCornerDiff(json, DITCameraTool::Logger&) const;
        };
    }
}
