#include "json_extension.hpp"

const std::string json::LoadJsonKey(const typename object_t::key_type& key)
{
    std::string func_name = "LoadJsonKey";
    if (const_cast<json*>(this)->contains(key))
    {
        return this->operator[](key);
    }
    else
    {
        throw DITCameraTool::Exception::KeyIsNotIncluded(func_name, "key("+key+") is not included.");
    }
}