#include "log.h"

Log::Log(STATUS status, std::string desc){
    //set time to now
    this->currTimePoint = std::chrono::steady_clock::now(); // record time

    this->status = status; // record status 

    this->desc = desc; // record description
}

STATUS Log::getSTATUS(){
    return status;
}

std::chrono::steady_clock::time_point Log::getCurrentTimePoint(){
    return this->currTimePoint;
}

std::string Log::getDescription(){
    return this->desc;
}

std::string Log::getFormattedData(){
    //[STATUS] [(DD/MM/YY)](HH:M:SS) -  "DESCRIPTION"
    std::string statusComponent = "["+statusToString()+"]";
    std::string dateComponent = "["+dateAsString()+"]";
    std::string timeComponent = "["+timeAsString()+"]";
    return statusComponent + dateComponent + timeComponent + " - \"" +desc+ "\" ";
}

std::string Log::dateAsString(){//DD/MM/YY
    time_t now  = time(nullptr);

    struct tm* localTime = localtime(&now);
    int year = localTime->tm_year +1900;
    std::string yearStr = std::to_string(year).substr(2);
    
    int month = localTime->tm_mon+1; // 0 - 11

    int day = localTime->tm_mday; // 1- 31

    std::string toRet = std::to_string(day);
    toRet+="/";
    toRet+=std::to_string(month);
    toRet+="/";
    toRet+=yearStr;

    return toRet;
}
std::string Log::timeAsString(){
    time_t now = time(nullptr);
    struct tm* localTime = localtime(&now);
    int hr = localTime->tm_hour;
    int min = localTime->tm_min;
    int sec = localTime->tm_sec;

    return std::to_string(hr)+":"+std::to_string(min)+":"+std::to_string(sec);
}

std::string Log::statusToString(){
    if(this->status == ERROR){
        return "ERROR";
    }
    else if(this->status == TEST){
        return "TEST";
    }
    else if(this->status == STARTUP){
        return "STARTUP";
    }
    else if(this->status == SHUTDOWN){
        return "SHUTDOWN";
    }
    else{
        return "LOG";
    }
}
