#ifndef LOG_H
#define LOG_H

#include <chrono>
#include <iostream>
#include <string>
#include <vector> 
#include <time.h>
#include <ctime>

enum STATUS{
    ERROR,
    LOG,
    TEST,
    STARTUP,
    SHUTDOWN,
    DOWNLOADING
};

class Log{ 
    private:
        STATUS status; // log status either ERROR LOG TEST
        std::chrono::steady_clock::time_point currTimePoint = std::chrono::steady_clock::now();// time point
        std::string desc;// one line description of action
        std::string statusToString();
        std::string dateAsString();
        std::string timeAsString();
        time_t parseDateTime(const char* datetimeString, const char* format);
        std::string DateTime(time_t time, const char* format);

    public:
        Log(STATUS status, std::string desc); //sets the relevant variables
        STATUS getSTATUS();
        std::chrono::steady_clock::time_point getCurrentTimePoint();
        std::string getDescription();
        std::string getFormattedData(); // example format below
        //[STATUS] [(DD/MM/YY)](HH:M:SS) - "DESCRIPTION"
        //each of them provide (BYTES SENDING FROM A TO B B TO A ETC WHATEVER)
};

#endif