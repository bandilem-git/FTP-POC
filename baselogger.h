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
#include "COLORS.cpp"


class BaseLogger{
    private:
        std::string targetFile; //stores the file that we are logging into
        std::string desc; //description, no need to format alr done with log 
        // std::mutex loggerLock; // each class has its own lock
        //TODO: add subscribe

    public:
        BaseLogger(std::string target, std::string desc);
        virtual void onEvent(Log log) = 0;
        void generateLog(STATUS status, std::string target, std::string desc);
        std::string getDesc();
        virtual ~BaseLogger();

};

#endif