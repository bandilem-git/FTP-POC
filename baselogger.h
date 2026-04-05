#ifndef BASELOGGER_H
#define BASELOGGER_H

#include "log.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>
#include <filesystem>
#include "COLORS.h"

/**
 * @class BaseLogger
 * @brief parent class of different type of loggers
 * @author Bandile Mnyandu
 */

class BaseLogger{
    private:
        /**
         * @brief target file is filename (with extention) that will have all the logs written to
         */
        std::string targetFile; //stores the file that we are logging into

        /**
         * @brief description that is going to be logged
         */
        std::string desc; //description, no need to format already done with log 

        /**
         * @brief mutually exclusive lock variable
         */
        std::mutex loggerLock;        
        
    public:
/**
         * @brief Constructs BaseLogger and initialises log file
         * @param target filename of the log file
         * @param desc description of the log
         */
        BaseLogger(std::string target, std::string desc);

        /**
         * @brief pure virtual event handler — must be implemented by derived classes
         * @param log the log entry to process
         */
        virtual void onEvent(Log& log) = 0;
        
        /**
         * @brief writes a log entry to the target file
         * @param status the severity level of the log
         * @param target the log file to write to
         * @param desc the message to log
         */
        void generateLog(STATUS status, std::string target, std::string desc);

        /**
         * @brief returns the description of this logger
         * @return logger description string
         */
        std::string getDesc();

        /**
         * @brief virtual destructor
         */
        virtual ~BaseLogger();
};

#endif