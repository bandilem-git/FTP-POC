#ifndef CONTROLSERVER_H
#define CONTROLSERVER_H

#include <sys/socket.h>
#include <sstream>
#include <fstream>

#include <filesystem>
#include <sys/stat.h>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "COLORS.cpp"
#include <thread>


/*
Server class: emulating (CONTROL SERVER)FILE TRANSFER PROTOCOL in C++17
*/

enum accountType{
    ANONYMOUS,
    USER,
    ADMIN
};

class ControlServer{
    private:

    struct stat sb;
    int ConnectionServerSocket = socket(AF_INET, SOCK_STREAM, 0);//IPv4 protocol, SOCK_STREAM = TCP connection]
    std::string getListOfFiles();
    int getNumFiles();
    bool fileExists(std::string x);
    std::vector<std::string> existingFiles;
    void update();
    accountType accType;

    public:
    
    std::mutex mtx;
    ControlServer(int port);
    std::string getavailableCommands();
    void start();
    std::string getListOfFilesUnsafe();//threaded operations

};

#endif