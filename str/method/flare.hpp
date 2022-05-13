#ifndef DITALG_LOADED
#include "../DITAlgorithm.hpp"
#define DITALG_LOADED
#endif


class flare: public DITAlgorithm{
    public:
        flare(Config, std::string);
        ~flare();
        bool execute();
};
