#ifndef DATALOGGER_H
#define DATALOGGER_H
#include "baselogger.h"

/**
 * @file datalogger.h
 * @class DataLogger
 * @brief Logger for the data connection — writes file transfer events to a log file
 */

class DataLogger: public BaseLogger{
    private:
        /// @brief target log filename
        std::string target;
    public:
        /**
        * @brief Constructs DataLogger and writes a startup entry to the log
        * @param target the log filename to write to
        */
        DataLogger(std::string target);

        /**
        * @brief handles an incoming log event and writes it to the data log file
        * @param log the event to log
        */
        void onEvent(Log& log);

        /**
        * @brief destructor — writes a shutdown entry to the log
        */
        ~DataLogger();
};

#endif