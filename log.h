#ifndef LOG_H
#define LOG_H

#include <chrono>
#include <iostream>
#include <string>
#include <vector> 
#include <time.h>
#include <ctime>
/**
 * @file log.h
 * @brief Log entry class and STATUS enum for the logging system
 * @author Bandile Mnyandu
 */

 /**
 * @enum STATUS
 * @brief Severity and type levels for log entries
 */

enum STATUS{
    ERROR,
    LOG,
    TEST,
    STARTUP,
    SHUTDOWN,
    DOWNLOADING,
    UPLOADING
};

/**
* @class Log
* @brief Represents a single log entry with a status, timestamp and description
*/

class Log{ 
    private:
        /// @brief severity level of this log entry
        STATUS status; // log status either ERROR LOG TEST

        /// @brief time point when this log entry was created
        std::chrono::steady_clock::time_point currTimePoint = std::chrono::steady_clock::now();// time point

        /// @brief one line description of the event being logged
        std::string desc;// one line description of action

        /// @brief converts STATUS enum to a readable string
        /// @return string representation of the status
        std::string statusToString();

        /// @brief formats the current date as a string
        /// @return date string
        std::string dateAsString();

        /// @brief formats the current time as a string
        /// @return time string
        std::string timeAsString();

    public:
        /**
        * @brief Constructs a Log entry with a status and description
        * @param status the severity level of this log entry
        * @param desc the message to log
        */
        Log(STATUS status, std::string desc); //sets the relevant variables

        /// @brief returns the status of this log entry
        /// @return STATUS enum value
        STATUS getSTATUS();

        /// @brief returns the timestamp of this log entry
        /// @return steady_clock time point
        std::chrono::steady_clock::time_point getCurrentTimePoint();

        /// @brief returns the description of this log entry
        /// @return description string
        std::string getDescription();

        /**
        * @brief returns the fully formatted log string
        * @return formatted string in the form [STATUS][DD/MM/YY][HH:MM:SS] - "DESCRIPTION"
        */
        std::string getFormattedData(); // example format below
        //[STATUS] [(DD/MM/YY)](HH:M:SS) - "DESCRIPTION"
        //each of them provide (BYTES SENDING FROM A TO B B TO A ETC WHATEVER)
};

#endif
