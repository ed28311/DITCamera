#include "algorithm_dispatcher.hpp"


DITCameraTool::AlgorithmDispatch::AlgorithmDispatch(DITCameraTool::Config config, std::string image_path)
{
    m_config = config;
    m_image_path = image_path;
    m_p_algorithm = NULL;
};

DITCameraTool::AlgorithmDispatch::~AlgorithmDispatch(){
    delete m_p_algorithm;
    m_p_algorithm = NULL;
}

bool DITCameraTool::AlgorithmDispatch::ExecuteAlgorithm(){
    bool result_bool=false;
    m_p_algorithm->LoadImage(m_image_path);
	printf("Load Image completed.");
    result_bool = m_p_algorithm->Execute();
    // delete m_p_algorithm;
    // m_p_algorithm = NULL;
    return result_bool;
}
DITCameraTool::Reporter DITCameraTool::AlgorithmDispatch::GetReporter()
{
    DITCameraTool::Reporter reporter = m_p_algorithm->GetReporter();
    return reporter;
}

void DITCameraTool::AlgorithmDispatch::DispatchAlgorithm()
{
    if (m_p_algorithm != NULL)
    {
        delete m_p_algorithm;
        m_p_algorithm = NULL;
    }
    json algorithm_conf = m_config.GetAlgorithmConf();
    if (m_p_algorithm==NULL)
    {
        if (algorithm_conf.LoadJsonKey("Algorithm") == "LS"){
            m_p_algorithm = new DITCameraTool::Algorithm::Shading(m_config);
        }
        if (algorithm_conf.LoadJsonKey("Algorithm") == "BL"){
            m_p_algorithm = new DITCameraTool::Algorithm::Blemish(m_config);
        }
        if (algorithm_conf.LoadJsonKey("Algorithm") == "FL"){
            m_p_algorithm = new DITCameraTool::Algorithm::Flare(m_config);
        }
    }
    // else
    // {
    //     throw DITCameraTool::Exception::AllocateToNotNullPointer(m_name);
    // };
}


