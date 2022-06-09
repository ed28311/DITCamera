#include "logger.hpp"

DITCameraTool::Logger::Logger(){
    logEnable = false; 
    maxCol = 0;
    serialNums = "ED0472-28311";
    _getCurrentTime();
    fileName = "Report-"+serialNums+"-"+currentDatetime;
}

DITCameraTool::Logger::Logger(json globalConf){
    config = globalConf;
    logCols = {"ITEM", "STATUS", "VALUE", "UCL", "LCL", "RESULT", "SPEC_NAME", "DATE_TIME", "OBJ_NAME", "COMMENT", "OTHERS", "IMG"};
    maxCol = logCols.size();
    logEnable = _getLogOption();
    std::cout << logEnable<< std::endl;
    serialNums = "ED0472-28311";
    _getCurrentTime();
    fileDir = (std::string)config["ReportDirectory"]+"/"+currentDate+"/";
    fileName = "Report-"+serialNums+"-"+currentDatetime;
}

void DITCameraTool::Logger::_getCurrentTime(){
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);
    std::string currentYear = std::to_string(now->tm_year+1900);
    std::string currentMonth = std::to_string(now->tm_mon+1);
    if(currentMonth.size()==1) currentMonth="0"+currentMonth;
    std::string currentDay = std::to_string(now->tm_mday);
    if(currentDay.size()==1) currentDay="0"+currentDay;
    std::string currentHour = std::to_string(now->tm_hour);
    std::string currentMin = std::to_string(now->tm_min);
    std::string currentSec = std::to_string(now->tm_sec);
    currentDate = currentYear+currentMonth+currentDay;
    currentDatetime = currentYear+currentMonth+currentDay+currentHour+currentMin+currentSec;
}

int DITCameraTool::Logger::_addDITLogComponentHeader(){
    std::vector<std::string> reportDeclare(maxCol);
    _PrintVariable((int)logCols.size());
    reportDeclare[0] = "Report_Version:"+(std::string)config["Version"];
    reportDeclare[1] = "Project_Name:"+(std::string)config["Name"];
    _PrintVariable(reportDeclare[1]); 
    this->logComponent.push_front(logCols);
    this->logComponent.push_front(reportDeclare);
    return maxCol;
}

void DITCameraTool::Logger::writeBack(json logElement){
    std::vector<std::string> logVec;
    for(std::string key:logCols){
        logVec.push_back((std::string)logElement[key]);
    }
    this->logComponent.push_back(logVec);
}
void DITCameraTool::Logger::writeFront(json logElement){
    std::vector<std::string> logVec;
    for(std::string key:logCols){
        logVec.push_back((std::string)logElement[key]);
    }
    this->logComponent.push_front(logVec);
}

std::string DITCameraTool::Logger::_generateCSVString(std::vector<std::string> logVec){
    std::string outputString = "";
    for (int i=0; i<logVec.size(); i++){
        outputString+=(logVec[i]+",");
    }
    if (outputString.size()>0){
        outputString.pop_back();
        outputString += "\n";
    }
    return outputString;
}

bool DITCameraTool::Logger::generateCSV(){
    _addDITLogComponentHeader();
    bool resultBool = false;
    if(!logEnable){
        return resultBool;
    }
    _PrintVariable((int)logComponent.size());
    std::ofstream logFile;
    _checkDirRoot(fileDir);
    std::string filePath = fileDir+fileName+".csv";
    _PrintVariable(filePath);
    logFile.open(filePath);
    for(int i=0; i<logComponent.size(); i++){
        logFile << _generateCSVString(logComponent[i]);
    }
    logFile.close();
    resultBool = true;
    return resultBool;
}

void DITCameraTool::Logger::_checkDirRoot(std::string fileDir){
    std::filesystem::create_directories(fileDir);
};
bool DITCameraTool::Logger::_getLogOption(){
    return std::stoi((std::string)config["OutputDebugInfo"]);
}

void DITCameraTool::Logger::mergeLogger(DITCameraTool::Logger mergeLogger){
    logEnable = (logEnable||mergeLogger.logEnable);
    for(std::string item:mergeLogger.logCols){
        if(std::find(logCols.begin(), logCols.end(), item) == logCols.end()){
            logCols.push_back(item);
        }
    }
    maxCol = (maxCol>mergeLogger.maxCol)?(maxCol):(mergeLogger.maxCol);
    fileDir = (fileDir.size()==0)?(mergeLogger.fileDir):(fileDir);
    fileName = (fileName.size()==0)?(mergeLogger.fileName):(fileName);
    for(auto& item:mergeLogger.config.items()){
        auto result = config.find(item.key());
        if (result == config.end()){
            config[item.key()] = item.value();
        }
    }
    logComponent.insert(logComponent.end(), mergeLogger.logComponent.begin(), mergeLogger.logComponent.end());
}