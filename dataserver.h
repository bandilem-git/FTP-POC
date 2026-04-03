#ifndef DATASERVER_H
#define DATASERVER_H
#include "baseserver.h"
#define DATAPORT 8081
/*
SERVER GOALS:
copying files from server a to client b
*/ 
class DataServer: public BaseServer{
    private:
        int numConnections;
        void DOWNLOAD(int socket,const char* file);
        void UPLOAD(int socket,const char* file);
        int countExistingfiles(std::string p);
    public:
        DataServer();//opens it up for connection
        void start();
 
};

#endif