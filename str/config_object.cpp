#include "config_object.hpp"

DITCameraTool::Config::~Config()
{
    m_global_config.clear();
    m_algorithm_config.clear();
}

DITCameraTool::Config::Config()
{
    json m_global_config;
    json m_algorithm_config;
}

DITCameraTool::Config::Config(json globalConfig, json algorithmConfig)
{
    m_global_config = globalConfig;
    m_algorithm_config = algorithmConfig;
}

json DITCameraTool::Config::GetAlgorithmConf() const
{
    return m_algorithm_config;
};

json DITCameraTool::Config::GetGlobalConf() const
{
    return m_global_config;
<<<<<<< HEAD
};

=======
};
>>>>>>> 5300e8e04935a22b9a1375aee249e1db6b96a1b0
