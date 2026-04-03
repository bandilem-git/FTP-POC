#ifndef BASESERVER_H
#define BASESERVER_H

//system
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>


//streams
#include <sstream>
#include <fstream>

//C based include
#include <cstdlib>

//files
#include <filesystem>

//NET
#include <netinet/in.h>
#include <arpa/inet.h>

//data structures
#include <queue>
#include <utility> //has pair
#include <vector>

//c++ incldes
#include <thread>
#include <err.h>

//includes 
#include "COLORS.cpp"
#include "baselogger.h"

enum CONNECTIONS{
    CONTROL,
    DATA
};

class BaseServer{
    private:
        std::vector<BaseLogger*> observers;
        void BindAndListen(int port);
    public:
    //shared 
        struct stat sb;
        int ConnectionServerSocket = socket(AF_INET, SOCK_STREAM, 0);//IPv4 protocol, SOCK_STREAM = TCP connection]
        void subscribe(BaseLogger* observer);
        void notify(CONNECTIONS connection, Log l);
        BaseServer();
        virtual void start() = 0;

};

#endif