#ifndef DISPATCHERLOAD
#define DISPATCHERLOAD
#include <iostream>
#include "DITAlgorithms.hpp"

class algorithmDispatch{
    public:
        algorithmDispatch(Config, std::string);
        bool executeAlgorithm();
        Config getDITConfig();
        Config DITConfig;
    private:
        DITAlgorithm *algorithm;
        void createAlgorithm(std::string);
};
#endif