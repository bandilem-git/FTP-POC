#ifndef CONTROLSERVER_H
#define CONTROLSERVER_H

#include "baseserver.h"


/*
Server class: emulating (CONTROL SERVER)FILE TRANSFER PROTOCOL in C++17
*/

class ControlServer : public BaseServer{
    private:

    struct stat sb;
    std::string getListOfFiles();
    int getNumFiles();
    bool fileExists(std::string x);
    std::vector<std::string> existingFiles;
    void update();

    public:
    
    std::mutex mtx;
    ControlServer();
    std::string getavailableCommands();
    void start();
    std::string getListOfFilesUnsafe();//threaded operations

};

#endif