#ifndef DATASERVER_H
#define DATASERVER_H
#include "controlserver.h"
#include <queue>
#include <utility> //has pair
#include <vector>
#include <err.h>
#include <arpa/inet.h>
#include <filesystem>
#include <regex>
/*
SERVER GOALS:
copying files from server a to client b
*/ 
class DataServer{
    private:
        struct stat sb; //for file reading
        int port = 8081;
        int numConnections;
        std::vector<int> connections; 
        int ConnectionServerSocket;
        void DOWNLOAD(int socket,const char* file);
        void UPLOAD(int socket,const char* file);
        int countExistingfiles(std::string p);
    public:
        DataServer();//opens it up for connection
        void start();
 
};

#endif