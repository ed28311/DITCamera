#include "algorithm_dispatcher.hpp"


<<<<<<< HEAD
DITCameraTool::AlgorithmDispatch::AlgorithmDispatch(DITCameraTool::Config config, std::string image_path, DITCameraTool::Reporter &reporter):mr_reporter(reporter){
    m_config = config;
    m_image_path = image_path;
=======
DITCameraTool::AlgorithmDispatch::AlgorithmDispatch(DITCameraTool::Config m_config, std::string imagePath, DITCameraTool::Reporter &reporter):mr_reporter(reporter){
    m_config = m_config;
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
    mp_algorithm = NULL;
};

DITCameraTool::AlgorithmDispatch::~AlgorithmDispatch(){
    delete mp_algorithm;
    mp_algorithm = NULL;
}

bool DITCameraTool::AlgorithmDispatch::ExecuteAlgorithm(){
    bool result_bool=false;
<<<<<<< HEAD
    mp_algorithm->LoadImage(m_image_path);
=======
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
    result_bool = mp_algorithm->Execute(mr_reporter);
    delete mp_algorithm;
    mp_algorithm = NULL;
    return result_bool;
}

<<<<<<< HEAD
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


=======
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
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
