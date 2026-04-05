#ifndef CTRLLOGGER_H
#define CTRLLOGGER_H

#include "baselogger.h"

/**
 * @file controllogger.h
 * @class ControlLogger
 * @brief Logger for the control connection — writes control channel events to a log file
 */
class ControlLogger : public BaseLogger{
    private:

        /// @brief target log filename
        std::string target;

    public:

        /**
        * @brief Constructs ControlLogger and writes a startup entry to the log
        * @param target the log filename to write to
        */
        ControlLogger(std::string target);

        /**
        * @brief handles an incoming log event and writes it to the control log file
        * @param log the event to log
        */
        void onEvent(Log& log);
        /**
        * @brief destructor — writes a shutdown entry to the log
        */
        ~ControlLogger();
};

#endif