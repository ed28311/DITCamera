#include <iostream>
#include <fstream>
#include <deque>
#include <ctime>
#include <filesystem>
#include "DITConfig.hpp"
#include "tool/displayer.hpp"

namespace DITCameraTool{
    class Logger{
        public:
            Logger();
            Logger(json globalConf);
            void writeBack(json);
            void writeFront(json);
            bool generateCSV();
            int maxCol;
            json config;
            std::string serialNums;
            std::string fileDir;
            std::string fileName;
            std::deque<std::vector<std::string>> logComponent;
            std::vector<std::string> logCols;
            std::string currentDatetime;
            std::string currentDate;
            bool logEnable;
            void mergeLogger(DITCameraTool::Logger);
        private:
            bool _getLogOption();
            int _addDITLogComponentHeader();
            std::string _generateCSVString(std::vector<std::string>);
            void _getCurrentTime();
            void _checkDirRoot(std::string);
    };
}