#include "json_extension.hpp"

const std::string json::LoadJsonKey(const std::string key)
{
    std::string func_name = "LoadJsonKey";
    if (const_cast<json*>(this)->contains(key))
    {
        return this->operator[](key);
    }
    else
    {
      std::cout << key << std::endl;
      std::cout << (this->contains(key)) << std::endl;
      std::string error_msg = "key is not included.("+key+").";
      throw std::invalid_argument(func_name+" "+ error_msg);
    }
}