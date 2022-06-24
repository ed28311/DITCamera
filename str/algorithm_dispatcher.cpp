#include "algorithm_dispatcher.hpp"


DITCameraTool::AlgorithmDispatch::AlgorithmDispatch(DITCameraTool::Config m_config, std::string imagePath, DITCameraTool::Reporter &reporter):mr_reporter(reporter){
    m_config = m_config;
    mp_algorithm = NULL;
};

DITCameraTool::AlgorithmDispatch::~AlgorithmDispatch(){
    delete mp_algorithm;
    mp_algorithm = NULL;
}

bool DITCameraTool::AlgorithmDispatch::ExecuteAlgorithm(){
    bool result_bool=false;
    result_bool = mp_algorithm->Execute(mr_reporter);
    delete mp_algorithm;
    mp_algorithm = NULL;
    return result_bool;
}

void DITCameraTool::AlgorithmDispatch::CreateAlgorithm(std::string imagePath){
    m_config = GetConfig();
    json algorithm_conf = m_config.GetAlgorithmConf();
    if (algorithm_conf["Algorithm"] == "LS"){
        mp_algorithm = new DITCameraTool::Algorithm::Shading(m_config);
    }
    if (algorithm_conf["Algorithm"] == "BL"){
        mp_algorithm = new DITCameraTool::Algorithm::Blemish(m_config);
    }
    if (algorithm_conf["Algorithm"] == "FL"){
        mp_algorithm = new DITCameraTool::Algorithm::Flare(m_config);
    }
    // mp_algorithm->LoadImage(imagePath);
};

DITCameraTool::Config DITCameraTool::AlgorithmDispatch::GetConfig(){
    return m_config;
};
