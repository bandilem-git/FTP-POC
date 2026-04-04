#ifndef DATASERVER_H
#define DATASERVER_H
#include "baseconnection.h"
#define DATAPORT 8081
/*
SERVER GOALS:
copying files from server a to client b
*/ 
class DataConnection: public BaseConnection{
    private:
        int numConnections;
        void DOWNLOAD(int socket,const char* file);
        void UPLOAD(int socket,const char* file);
        std::string fileRenameProcess(std::string p);
    public:
        DataConnection();//opens it up for connection
        void start();
 
};

#endif