#include "controllogger.h"

ControlLogger::ControlLogger(std::string target) : BaseLogger(target, ""){//Base Logger constructor constructs and checks if it exists
    this->target = target;
    this->generateLog(STARTUP, target, "CONTROL CHANNEL STARTING UP\n");

}

void ControlLogger::onEvent(Log log){
    //log fault assume all data in log is correct
    //write at the end
    generateLog(log.getSTATUS(),target, log.getDescription());
}