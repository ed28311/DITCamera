#include "algorithm_base.hpp"

namespace DITCameraTool{
    namespace Algorithm{
        class Shading: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                Shading();
                Shading(const DITCameraTool::Config);
                virtual bool Execute() const override;
                virtual void LoadFigure(std::string) override;
            private:
                int FetchAvgPixel(int, int, int, int) const;
                json CollectCornerAvgIntensity(int avgAreaLB, int avgAreaLT, int avgAreaRB, int avgAreaRT) const;
                void AppendCornerAvgIntensity(json &jsonAvgAreaCorner,std::string name, int val) const;
                bool DetectCenterIntensity(int) const;
                bool DetectCornerShading(int, json) const;
                bool DetectCornerDiff(json) const;
        };
    }
}
