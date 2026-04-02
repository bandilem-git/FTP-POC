#include "datalogger.h"

DataLogger::DataLogger(std::string target) : BaseLogger(target, ""){//Base Logger constructor constructs and checks if it exists
    this->target = target;
    //check logs/control.txt exists 
    std::string folderPath = "logs/";
    std::string fileName = target;

    //directory composing of folderPath and fileName
    std::string dir = folderPath + fileName;

    //go to the lowest point and begin writing by saying logging start up through log
    //control.txt

    std::ofstream controlFile(dir,std::ios::app);//set mode to append
    //if the file doesn't open, throw a runtime error
    if(!controlFile.is_open()){
        throw std::runtime_error("could not open control.txt for error logging");
        controlFile.close();
    }

    controlFile.close();
    this->generateLog(STARTUP, target, "CONTROL CHANNEL IN USE");

}

void DataLogger::onEvent(Log log){
    //log fault assume all data in log is correct
    //write at the end
    generateLog(log.getSTATUS(),target, log.getDescription());
}