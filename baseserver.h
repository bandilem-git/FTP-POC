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
#include <map>

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
        std::map<CONNECTIONS,std::vector<BaseLogger*>> observers;
        void BindAndListen(int port);
    public:
        void subscribe(CONNECTIONS type ,BaseLogger* observer);
        void notify(CONNECTIONS connection, Log l);
        BaseServer(int port);
        virtual void start() = 0;
    protected:
        //shared 
        struct stat sb;
        int ConnectionServerSocket = socket(AF_INET, SOCK_STREAM, 0);//IPv4 protocol, SOCK_STREAM = TCP connection]
};

#endif