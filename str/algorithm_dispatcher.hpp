#ifndef ALGORITHM_DISPATHCER_HPP
#define ALGORITHM_DISPATHCER_HPP
#include <iostream>
#include "algorithm_loading_header.hpp"
#include "exception.hpp"

namespace DITCameraTool{
    class AlgorithmDispatch{
        public:
            AlgorithmDispatch(Config, std::string);
            ~AlgorithmDispatch();
            void DispatchAlgorithm();
            bool ExecuteAlgorithm();
            DITCameraTool::Reporter GetReporter();
        private:
            std::string m_name = "AlgorithmDispatch";
            std::string m_image_path;
            DITCameraTool::Config m_config;
            DITCameraTool::Algorithm::AlgorithmBase* m_p_algorithm;
    };
}
#endif