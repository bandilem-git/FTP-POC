#ifndef BASECONNECTION_H
#define BASECONNECTION_H
/**
 * @file baseconnection.h
 * @author Bandile Mnyandu 
 * 
 */
//system
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>


//streams
#include <sstream>
#include <fstream>

//C based include
#include <cstdlib>
#include <cstring>
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
#include "COLORS.h"
#include "baselogger.h"

/**
 * @enum CONNECTIONS
 * @brief Identifies connection type between CONTROL and DATA
 * 
 */

enum CONNECTIONS{
    CONTROL,
    DATA
};

/**
 * @class BaseConnection
 * @brief Parent class of ControlConnection and DataConnection.
 * 
 * Base class that all derived connection classes will publically inherit from
 * class has methods to handle Binding and listening of ports 
 * 8081 -> DATA PORT (simulating port 21)
 * 8080 -> CONNECTION PORT (simulating port 20)
 * 
 */
class BaseConnection{

    private:
        /**
         *  
         * @brief map of observers
         */

        std::map<CONNECTIONS,std::vector<BaseLogger*>> observers;
        
        /**
         * @brief Binds the port and listens to the port
         * @param port integer value of the port that should be bound and listened on
         */

        void BindAndListen(int port);

    public:

        /**
         * 
         * @brief adds observer an observer to the map of observers
         * @param type relevant CONNECTIONS enum value 
         * @param observer the relevant observer to notify depending on the type
         */

        void subscribe(CONNECTIONS type ,BaseLogger* observer);

        /**
         * 
         * @brief sends the appropriate subscriber(s) a log of the event
         * @param connection the relevant connection that should log event
         * @param log the event being logged
         */

        void notify(CONNECTIONS connection, Log l);

        /**
         * 
         * @brief Constructs BaseConnection and binds ports to allow for access
         * @param port port that is about to be binded and listened to
         */

        BaseConnection(int port);
        
        /**
         * @brief pure virtual function that all derived classes must implement
         */
        virtual void start() = 0;

        /**
         * 
         * @brief virtual destructor 
         */
        virtual ~BaseConnection();

    protected:
        //shared 

        /**
         * @brief sb variable used for file traversal
         * @note file traversal using sb comes from https://www.geeksforgeeks.org/cpp/cpp-program-to-get-the-list-of-files-in-a-directory/
         */

        struct stat sb;

        /**
         * @brief establishes a TCP link using IPv4 protocol
         */
        int ConnectionServerSocket = socket(AF_INET, SOCK_STREAM, 0);//IPv4 protocol, SOCK_STREAM = TCP connection]
};

#endif