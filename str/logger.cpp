#include "logger.hpp"

DITCameraTool::Logger::Logger(){
    logEnable = false; 
    maxCol = 0;
    serialNums = "ED0472-28311";
    _getCurrentTime();
}

DITCameraTool::Logger::Logger(json globalConf){
    std::cout << "LOGGER" <<std::endl;
    maxCol = logCols.size();
    logEnable = _getLogOption();
    serialNums = "ED0472-28311";
    logCols = {"ITEM", "STATUS", "VALUE", "UCL", "LCL", "RESULT", "SPEC_NAME", "DATE_TIME", "OBJ_NAME", "COMMENT", "OTHERS", "IMG"};
    _getCurrentTime();
    fileDir = (std::string)config["ReportDirectory"]+currentDate+"/";
    fileName = "Report-"+serialNums+"-"+currentDatetime;
}

void DITCameraTool::Logger::_getCurrentTime(){
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);
    std::string currentYear = std::to_string(now->tm_year+1900);
    std::string currentMonth = std::to_string(now->tm_mon+1);
    std::string currentDay = std::to_string(now->tm_mday);
    std::string currentHour = std::to_string(now->tm_hour);
    std::string currentMin = std::to_string(now->tm_min);
    std::string currentSec = std::to_string(now->tm_sec);
    currentDate = currentYear+currentMonth+currentDay;
    currentDatetime = currentYear+currentMonth+currentDay+currentHour+currentMin+currentSec;
}

int DITCameraTool::Logger::_addDITLogComponentHeader(){
    std::vector<std::string> reportDeclare(maxCol);
    reportDeclare[0] = "Report_Version:"+(std::string)config["Version"];
    reportDeclare[1] = "Project_Name:"+(std::string)config["Name"];
    this->logComponent.push_front(logCols);
    this->logComponent.push_front(reportDeclare);
    return maxCol;
}

void DITCameraTool::Logger::write(json logElement){
    std::vector<std::string> logVec;
    for(std::string key:logCols){
        logVec.push_back((std::string)logElement[key]);
    }
    this->logComponent.push_back(logVec);
}

std::string DITCameraTool::Logger::_generateCSVString(std::vector<std::string> logVec){
    _addDITLogComponentHeader();
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

bool DITCameraTool::Logger::generate(){
    bool resultBool = false;
    if(~logEnable){
        return resultBool;
    }
    std::ofstream logFile;
    logFile.open(fileDir+fileName);
    for(int i=0; i<logComponent.size(); i++){
        logFile << _generateCSVString(logComponent[i]);
    }
    logFile.close();
    resultBool = true;
    return resultBool;
}


bool DITCameraTool::Logger::_getLogOption(){
    return std::stoi((std::string)config["OutputDebugInfo"]);
}
