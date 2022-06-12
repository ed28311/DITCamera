#ifndef PRINTVARIABLE_LOAD
#include <iostream>
#include <vector>
#include "../DITConfig.hpp"
#define PRINTVARIABLE_LOAD


namespace DITCameraTool {
    class Display{
        public:
            Display();
            Display(std::string);
            std::string debugMode;
            void cprintf(std::string);
            void printVariable(std::string, std::string);
            void printVariable(std::string, int);
            void printVariable(std::string, float);
            void printVairable(std::string, std::vector<std::string>);
            void printVariable(std::string, DITCameraTool::Config);
        private:
            std::string _debugPrintFormat(std::string);          
              
    };
}

#define _PrintVariable(variable) DITCameraTool::Display().printVariable((std::string)#variable, (variable))
#endif