#ifndef DISPATCHERLOAD
#define DISPATCHERLOAD
#include <iostream>
#include "DITAlgorithms.hpp"

namespace DITCameraTool{
    class AlgorithmDispatch{
        public:
            AlgorithmDispatch(Config, std::string);
            bool executeAlgorithm();
            DITCameraTool::Config getDITConfig();
            DITCameraTool::Config DITConfig;
        private:
            DITCameraTool::Algorithm::AlgorithmBase *algorithm;
            void createAlgorithm(std::string);
    };
}
#endif