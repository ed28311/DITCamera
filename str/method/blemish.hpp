#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


class blemish: public DITAlgorithm{
    public:
        blemish(Config, std::string);
        ~blemish();
        bool execute() ;
};
