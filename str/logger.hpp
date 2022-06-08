#include <iostream>
#include <fstream>
#include <deque>
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
            std::deque<std::vector<std::string>> logComponent;
            std::vector<std::string> logCols;
            std::string currentDatetime;
            std::string currentDate;
            json config;
            bool logEnable;
        private:
            bool _getLogOption();
            int _addDITLogComponentHeader();
            std::string _generateCSVString(std::vector<std::string>);
            void _getCurrentTime();
    };
}