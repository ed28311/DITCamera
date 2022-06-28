#ifndef ALGORITHM_DISPATHCER_HPP
#define ALGORITHM_DISPATHCER_HPP
#include <iostream>
#include "algorithm_loading_header.hpp"
<<<<<<< HEAD
#include "exception.hpp"
=======
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0

namespace DITCameraTool{
    class AlgorithmDispatch{
        public:
            AlgorithmDispatch(Config, std::string, DITCameraTool::Reporter&);
            ~AlgorithmDispatch();
<<<<<<< HEAD
            DITCameraTool::Reporter& mr_reporter;
            std::string m_name = "AlgorithmDispatch";
            void DispatchAlgorithm();
            bool ExecuteAlgorithm();
        private:
            std::string m_image_path;
            DITCameraTool::Config m_config;
            DITCameraTool::Algorithm::AlgorithmBase* mp_algorithm;
=======
            bool ExecuteAlgorithm();
            DITCameraTool::Config GetConfig();
            DITCameraTool::Reporter& mr_reporter;
        private:
            std::string m_image_path;
            DITCameraTool::Config m_config;
            DITCameraTool::Algorithm::AlgorithmBase *mp_algorithm;
            void CreateAlgorithm(std::string);
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
    };
}
#endif