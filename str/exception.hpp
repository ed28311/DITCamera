#ifndef DITCAMERATOOL_EXCEPTION_HPP
#define DITCAMERATOOL_EXCEPTION_HPP
#include <iostream>
namespace DITCameraTool
{
    namespace Exception
    {
        class AllocateToNotNullPointer: public std::exception
        {
            std::string _msg;
            std::string description = "allocate to not null pointer.";
            public:
                AllocateToNotNullPointer():_msg(description){}
                AllocateToNotNullPointer(const std::string &function_name):_msg(function_name+" "+description){}
                virtual const char* what() const noexcept override
                {
                    return _msg.c_str();
                }
        };
        class DivideByZero: public std::exception
        {
            std::string _msg;
            std::string description = "divide by zero.";
            public:
                DivideByZero():_msg(description){}
                DivideByZero(const std::string &function_name):_msg(function_name+" "+description){}
                DivideByZero(const std::string &function_name, const std::string &additional_description):_msg(function_name+" "+additional_description){}
                virtual const char* what() const noexcept override
                {
                    return _msg.c_str();
                }
        };
        class KeyIsNotIncluded: public std::exception
        {
            std::string _msg;
            std::string description = " key is not included.";
            public:
                KeyIsNotIncluded():_msg(description){}
                KeyIsNotIncluded(const std::string &function_name):_msg(function_name+" "+description){}
                KeyIsNotIncluded(const std::string &function_name, const std::string &additional_description):_msg(function_name+" "+additional_description){}
                virtual const char* what() const noexcept override
                {
                    return _msg.c_str();
                }
        };
    }
}
#endif