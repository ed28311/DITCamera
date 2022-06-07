#include <iostream>
#include <fstream>
#include <ctime>
#include "DITConfig.hpp"

namespace DITCameraTool{
    class Logger{
        public:
            Logger();
            Logger(json globalConf);
            void write(json);
            bool generate();
            int maxCol;
            std::string serialNums;
            std::string fileDir;
            std::string fileName;
            std::vector<std::vector<std::string>> logComponent;
            std::vector<std::string> logCols;
            std::string currentDatetime;
            json config;
            
        private:
            bool logEnable;
            bool _getLogOption();
            int _addDITLogComponentHeader();
            std::string _generateCSVString(std::vector<std::string>);
            std::string _getCurrentDate();
    };
}