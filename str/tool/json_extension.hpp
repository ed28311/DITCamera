#ifndef DITCAMERATOOL_JSON_LOADER
#define DITCAMERATOOL_JSON_LOADER
#include <iostream>
#include <exception>
#include "nlohmann/json.hpp"
#include "../exception.hpp"

class json: public nlohmann::json
{
    public:
        const std::string LoadJsonKey(const std::string key);
};
#endif