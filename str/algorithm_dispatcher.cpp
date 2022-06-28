#include "algorithm_dispatcher.hpp"


DITCameraTool::AlgorithmDispatch::AlgorithmDispatch(DITCameraTool::Config config, std::string image_path, DITCameraTool::Reporter &reporter):mr_reporter(reporter){
    m_config = config;
    m_image_path = image_path;
    mp_algorithm = NULL;
};

DITCameraTool::AlgorithmDispatch::~AlgorithmDispatch(){
    delete mp_algorithm;
    mp_algorithm = NULL;
}

bool DITCameraTool::AlgorithmDispatch::ExecuteAlgorithm(){
    bool result_bool=false;
    mp_algorithm->LoadImage(m_image_path);
    result_bool = mp_algorithm->Execute(mr_reporter);
    delete mp_algorithm;
    mp_algorithm = NULL;
    return result_bool;
}

void DITCameraTool::AlgorithmDispatch::DispatchAlgorithm()
{
    json algorithm_conf = m_config.GetAlgorithmConf();
    if (mp_algorithm==NULL)
    {
        if (algorithm_conf.LoadJsonKey("Algorithm") == "LS"){
            mp_algorithm = new DITCameraTool::Algorithm::Shading(m_config);
        }
        if (algorithm_conf.LoadJsonKey("Algorithm") == "BL"){
            mp_algorithm = new DITCameraTool::Algorithm::Blemish(m_config);
        }
        if (algorithm_conf.LoadJsonKey("Algorithm") == "FL"){
            mp_algorithm = new DITCameraTool::Algorithm::Flare(m_config);
        }
    }
    else
    {
        throw DITCameraTool::Exception::AllocateToNotNullPointer(m_name);
    };
}


