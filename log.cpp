#include "log.h"

Log::Log(STATUS status, std::string desc){
    //set time to now
    this->currTimePoint = std::chrono::steady_clock::now(); // record time

    this->status = status; // record status 

    this->desc = desc; // record description
}