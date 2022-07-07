#include "algorithm_base.hpp"

namespace DITCameraTool{
    namespace Algorithm{
        class BlemishPixel: public DITCameraTool::Algorithm::AlgorithmBase{
            public:
                BlemishPixel();
                BlemishPixel(DITCameraTool::Config);
                virtual bool Execute() const override;
                virtual void LoadFigure(std::string) override;
            private:
                bool _loopPixels(bool, std::vector<std::vector<int>>&) const;
                int _calcAbsVal(int) const;
        };
    }
}