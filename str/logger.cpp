#include "logger.hpp"

DITCameraTool::Logger::Logger(){
    logEnable = false; 
    maxCol = 0;
    serialNums = "ED0472-28311";
    currentDatetime = _getCurrentDate();
}

DITCameraTool::Logger::Logger(json globalConf){
    config = globalConf;
    logEnable = _getLogOption();
    serialNums = "ED0472-28311";
    currentDatetime = _getCurrentDate();
    maxCol = _addDITLogComponentHeader();
    fileDir = (std::string)config["ReportDirectory"]+currentDatetime+"/";
    fileName = "Report-"+serialNums+"-"+currentDatetime;
}

std::string DITCameraTool::Logger::_getCurrentDate(){
    std::time_t t = std::time(0);
    std::tm *now = std::localtime(&t);
    std::string currentYear = std::to_string(now->tm_year+1900);
    std::string currentMonth = std::to_string(now->tm_mon+1);
    std::string currentDay = std::to_string(now->tm_mday);
    std::string currentDatetime = currentYear+currentMonth+currentDay;
    return currentDatetime;
}

int DITCameraTool::Logger::_addDITLogComponentHeader(){
    logCols = {"ITEM", "STATUS", "VALUE", "UCL", "LCL", "RESULT", "SPEC_NAME", "DATE_TIME", "OBJ_NAME", "COMMENT", "OTHERS", "IMG"};
    maxCol = logCols.size();
    std::vector<std::string> reportDeclare(maxCol);
    reportDeclare[0] = "Report_Version:"+(std::string)config["Version"];
    reportDeclare[1] = "Project_Name:"+(std::string)config["Name"];
    this->logComponent.push_back(reportDeclare);
    this->logComponent.push_back(logCols);
    return maxCol;
}

void DITCameraTool::Logger::write(json logElement){
    std::vector<std::string> logVec;
    for(std::string key:logCols){
        logVec.push_back((std::string)logElement[key]);
    }
    this->logComponent.push_back(logVec);
}

std::string _generateCSVString(std::vector<std::string> logVec){
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
