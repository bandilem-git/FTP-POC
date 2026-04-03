#include "datalogger.h"

DataLogger::DataLogger(std::string target) : BaseLogger(target, ""){//Base Logger constructor constructs and checks if it exists
    this->target = target;
    this->generateLog(STARTUP, target, "DATA CONNECTION STARTING UP");
}

void DataLogger::onEvent(Log& log){
    //log fault assume all data in log is correct
    //write at the end
    generateLog(log.getSTATUS(),target, log.getDescription());
}

DataLogger::~DataLogger(){
    //log shut down
    generateLog(SHUTDOWN, target, "DATA CONNECTION IS SHUTTING DOWN");
}