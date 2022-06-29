#ifndef ALGORITHM_DISPATHCER_HPP
#define ALGORITHM_DISPATHCER_HPP
#include <iostream>
#include "algorithm_loading_header.hpp"
#include "exception.hpp"

namespace DITCameraTool{
    class AlgorithmDispatch{
        public:
            AlgorithmDispatch(Config, std::string, DITCameraTool::Reporter&);
            ~AlgorithmDispatch();
            DITCameraTool::Reporter& mr_reporter;
            std::string m_name = "AlgorithmDispatch";
            void DispatchAlgorithm();
            bool ExecuteAlgorithm();
        private:
            std::string m_image_path;
            DITCameraTool::Config m_config;
            DITCameraTool::Algorithm::AlgorithmBase* mp_algorithm;
    };
}
#endif