#ifndef CTRLLOGGER_H
#define CTRLLOGGER_H

#include "baselogger.h"

class ControlLogger : public BaseLogger{
    private:
        std::string targetFile = "logs/control.txt";
        
    public:
        ControlLogger();
};

#endif