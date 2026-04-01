#ifndef BASELOGGER_H
#define BASELOGGER_H

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

enum SEVERITY{
    LOW,
    MED,
    HIGH
};



class BaseLogger{
    private:
    std::string targetFile;//stores the file that we are logging into
    SEVERITY severity; //
    std::string description;
    std::mutex mutualExclusion;
    
    public:
    virtual void logSuccess(std::string x) = 0;

};

#endif