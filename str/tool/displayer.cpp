#include "displayer.hpp"

DITCameraTool::Display::Display(){
    debugMode = _debugPrintFormat("INFO");
}

DITCameraTool::Display::Display(std::string debugString){
    debugMode = _debugPrintFormat(debugString);
}


std::string DITCameraTool::Display::_debugPrintFormat(std::string debugString){
    if (debugString==(std::string)("INFO")){
        return "\x1b[;33;1m";
    }
    else if(debugString==(std::string)("WARNING")){
        return  "\x1b[;31;1m";
    }
    else{
        throw std::invalid_argument("Wrong debugMode format.");
    }
}
void DITCameraTool::Display::printVariable(std::string variableName, std::string variable) {
    std::cout <<  debugMode <<variableName<< "\033[0m" << " = " << variable << std::endl;
}

void DITCameraTool::Display::printVariable(std::string variableName, int variable) {
    std::cout <<  debugMode <<variableName<< "\033[0m" << " = " << variable << std::endl;
}

void DITCameraTool::Display::printVariable(std::string variableName, float variable) {
    std::cout <<  debugMode <<variableName<< "\033[0m" << " = " << variable << std::endl;
}

void cprintf(std::string printString){

};
