#ifndef CONTROLCONNECTION_H
#define ControlConnection_H

#include "baseconnection.h"
#define CONTROLPORT 8080

/*
Server class: emulating (CONTROL SERVER)FILE TRANSFER PROTOCOL in C++17
*/

class ControlConnection : public BaseConnection{
    private:

        std::string getListOfFiles();
        int getNumFiles();
        bool fileExists(std::string x);
        std::vector<std::string> existingFiles;
        void update();

    public:
    
        std::mutex mtx;
        ControlConnection();
        std::string getavailableCommands();
        void start();
        std::string getListOfFilesUnsafe();//threaded operations

};

#endif