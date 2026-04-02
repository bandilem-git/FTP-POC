#ifndef DATALOGGER_H
#define DATALOGGER_H
#include "baselogger.h"

class DataLogger: public BaseLogger{
    private:
        std::string target;
    public:
        DataLogger(std::string target);
        void onEvent(Log log);
};

#endif