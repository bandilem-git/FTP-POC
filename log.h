#ifndef LOG_H
#define LOG_H

#include <chrono>
#include <iostream>
#include <string>
#include <vector> 

enum STATUS{
    ERROR,
    LOG,
    TEST
};

class Log{ 
    private:
        STATUS status; // log status either ERROR LOG TEST
        std::chrono::steady_clock::time_point currTimePoint = std::chrono::steady_clock::now();// time point
        std::string desc;// one line description of action
    public:
        Log(STATUS status, std::string desc); //sets the relevant variables
        STATUS getSTATUS();
        std::chrono::steady_clock::time_point getCurrentTimePoint();
        std::string getDescription();
        std::string getFormattedData(); // example format below
        //[STATUS] [(DD/MM/YY)](HH:M:SS) - threadID: "DESCRIPTION"
        //each of them provide (BYTES SENDING FROM A TO B B TO A ETC WHATEVER)
};

#endif