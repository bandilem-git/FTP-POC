#ifndef CTRLLOGGER_H
#define CTRLLOGGER_H

#include "baselogger.h"


class ControlLogger : public BaseLogger{
    private:
        std::string target;
    public:
        ControlLogger(std::string target);
        void onEvent(Log log);
};

#endif