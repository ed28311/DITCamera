#include "config_loader.hpp"

DITCameraTool::ConfigLoader::ConfigLoader()
{
    json config;
};

DITCameraTool::Config DITCameraTool::ConfigLoader::GetJsonConfig(std::string file_path, std::string algorithm_name, std::vector<std::string> global_config_member)
{
    return DITCameraTool::Config();
};

DITCameraTool::Config DITCameraTool::ConfigLoader::GetSPEConfig(std::string file_path, std::string algorithm_name, std::vector<std::string> global_config_member)
{
    m_global_config_member = global_config_member;
    json global_config, algorithm_config;
    std::string algorithm, config_mode;
    
    std::tuple<std::string, std::string>  mode_pair = _ParseModeArgument(algorithm_name);
    std::tie(algorithm, config_mode) = mode_pair;

    std::tuple<json, json> config_pair = _ParseSPEConfig(file_path, algorithm, config_mode);
    std::tie(global_config, algorithm_config) = config_pair;
    algorithm_config["Algorithm"] = algorithm;
    algorithm_config["ConfigMode"] = config_mode;
    DITCameraTool::Config config = DITCameraTool::Config(global_config, algorithm_config);
    return config;
};

std::tuple<json, json> DITCameraTool::ConfigLoader::_ParseSPEConfig(std::string file_path, std::string algorithm, std::string config_mode)
{
    std::ifstream in_file(file_path);
    std::string read_line;
    json global_config;
    json algorithm_config; 
    std::string inspect = "";
    bool detect_algorithm = false;
    while(std::getline(in_file, read_line))
    {
        if (!std::regex_search(read_line, std::regex(";")))
        {
            if (std::regex_search(read_line, std::regex("\\[.*[\\w]+\\]")))
            {
                std::smatch sm;
                std::regex_search(read_line, sm, std::regex("\\w+"));
                inspect = sm[0];
            }
            if (std::regex_search(read_line, std::regex("[\\w]+[\\ \\=^;]+[\\w\\.]+")))
            {
                if (IsGlobalParam(inspect))
                {
                    std::string key;
                    std::string value;
                    std::tuple<std::string, std::string> key_value_pair = _ParseSPEConfigLine(read_line);
                    std::tie(key, value) = key_value_pair;
                    global_config[key] = value;
                }else{
                    if(inspect==config_mode)
                    {
                        detect_algorithm = true;
                        std::string key;
                        std::string value;
                        std::tuple<std::string, std::string> key_value_pair = _ParseSPEConfigLine(read_line);
                        std::tie(key, value) = key_value_pair;
                        algorithm_config[key] = value;
                    }
                };
            }   
        }
    };  
    if (!detect_algorithm)
    {
        throw std::invalid_argument("Invalid mode. (mode: "+config_mode+").");
    }
    return std::make_tuple(global_config, algorithm_config);
};

std::tuple<std::string, std::string> DITCameraTool::ConfigLoader::_ParseSPEConfigLine(std::string read_line)
{
    /*
    Assumption: 
        1. the read_line only contain a singal equal sign, and space is meanless.
        2. Parameter in config is all integer format.
    */
    std::string param;
    std::string value;
    std::string parse_line = read_line;
    parse_line.erase(std::remove(parse_line.begin(), parse_line.end(), ' '), parse_line.end());
    parse_line.erase(std::remove(parse_line.begin(), parse_line.end(), '\r'), parse_line.end());
    int parseLineSize = parse_line.size();
    int paramEndLoc = parse_line.find_first_of("=");
    int valueBeginLoc = parse_line.find_last_of("=");
    param = parse_line.substr(0, paramEndLoc);
    value = parse_line.substr(valueBeginLoc+1, parseLineSize);
    return std::make_tuple(param, value);
};

bool DITCameraTool::ConfigLoader::IsGlobalParam(std::string inspect)
{
    if(std::find(m_global_config_member.begin(), m_global_config_member.end(), inspect)!=m_global_config_member.end())
    {
        return true;
    }
    else
    {
        return false;
    }
};

std::tuple<std::string, std::string> DITCameraTool::ConfigLoader::_ParseModeArgument(std::string mode) 
{
	std::vector<std::string> mode_pair(2);
	std::regex reg("^-.*[\\w]+\\[.*[\\w]+\\]");
	if (std::regex_match(mode, reg))
    {
		std::smatch sm;
		std::regex matchstring("[\\w]+");
        std::regex_search(mode, sm, matchstring);
		int i = 0;
		while (std::regex_search(mode, sm, matchstring))
        {
			for (std::string x : sm)
            {
				mode_pair[i] = x;
			}
			mode = sm.suffix().str();
			i++;
			if (i > 2)
            {
				throw std::invalid_argument("Invalid arguments format (" + mode + "Wrong mode format.). (format: -mode[modeName] configPath imagePath)");
				break;
			}
		}
	}
	else 
    {
		throw std::invalid_argument("Invalid arguments format (" + mode + "Wrong mode format.). (format: -mode[modeName] configPath imagePath)");
	}
	return std::make_tuple(mode_pair[0], mode_pair[1]);
}