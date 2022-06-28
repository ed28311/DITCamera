#include "algorithm_base.hpp"

namespace DITCameraTool{
    namespace Algorithm{
        class Shading: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                Shading();
                Shading(DITCameraTool::Config);
                virtual bool Execute(DITCameraTool::Reporter&) const override;
                virtual void LoadImage(std::string) override;
            private:
                int _FetchAvgPixel(int, int, int, int) const;
                json _CollectCornerAvgIntensity(int avgAreaLB, int avgAreaLT, int avgAreaRB, int avgAreaRT) const;
                void _AppendCornerAvgIntensity(json &jsonAvgAreaCorner,std::string name, int val) const;
                bool _DetectCenterIntensity(int, DITCameraTool::Reporter&) const;
                bool _DetectCornerShading(int, json, DITCameraTool::Reporter&) const;
                bool _DetectCornerDiff(json, DITCameraTool::Reporter&) const;
        };
    }
}
