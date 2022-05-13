#include <iostream>
#include "DITAlgorithms.hpp"

class algorithmDispatch{
    public:
        algorithmDispatch(Config, std::string);
        ~algorithmDispatch();
        bool executeAlgorithm();
        Config getDITConfig();
        Config DITConfig;
    private:
        DITAlgorithm *algorithm;
        void createAlgorithm(std::string);
};