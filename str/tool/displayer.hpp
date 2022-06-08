#ifndef PRINTVARIABLE_LOAD
#include <iostream>
#define PRINTVARIABLE_LOAD
#define _PrintVariable(variable) DITCameraTool::Display().printVariable(#variable, (variable))

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
        private:
            std::string _debugPrintFormat(std::string);            
    };
}
#endif