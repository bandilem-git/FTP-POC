#include "controllogger.h"

ControlLogger::ControlLogger(std::string target) : BaseLogger(target, ""){//Base Logger constructor constructs and checks if it exists
    this->target = target;
    this->generateLog(STARTUP, target, "CONTROL CONNECTION STARTING UP");
}

void ControlLogger::onEvent(Log& log){
    //log fault assume all data in log is correct
    //write at the end
    generateLog(log.getSTATUS(),target, log.getDescription());
}

ControlLogger::~ControlLogger(){
    generateLog(SHUTDOWN, target, "CONTROL CONNECTION IS SHUTTING DOWN");

}