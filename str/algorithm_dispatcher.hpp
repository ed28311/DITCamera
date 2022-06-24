#ifndef ALGORITHM_DISPATHCER_HPP
#define ALGORITHM_DISPATHCER_HPP
#include <iostream>
#include "algorithm_loading_header.hpp"

namespace DITCameraTool{
    class AlgorithmDispatch{
        public:
            AlgorithmDispatch(Config, std::string, DITCameraTool::Reporter&);
            ~AlgorithmDispatch();
            bool ExecuteAlgorithm();
            DITCameraTool::Config GetConfig();
            DITCameraTool::Reporter& mr_reporter;
        private:
            std::string m_image_path;
            DITCameraTool::Config m_config;
            DITCameraTool::Algorithm::AlgorithmBase *mp_algorithm;
            void CreateAlgorithm(std::string);
    };
}
#endif