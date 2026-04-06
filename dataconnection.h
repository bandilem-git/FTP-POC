#ifndef DATASERVER_H
#define DATASERVER_H
#include "baseconnection.h"
#define DATAPORT 8081
/**
* @file dataconnection.h
* @brief Data connection server — handles file uploads and downloads on port 8081
* @author Bandile Mnyandu
*/

/**
* @class DataConnection
* @brief Handles binary file transfer over the data channel
* 
* Accepts incoming connections on port 8081 and spawns a thread per client.
* Each thread determines whether the request is an upload or download and handles accordingly.
*/
class DataConnection: public BaseConnection{
    private:
        /// @brief tracks number of active connections
        int numConnections;
        /**
        * @brief sends a file to the client
        * @param socket the client socket to send to
        * @param file the filename to download from the files/ directory
        */
        void DOWNLOAD(int socket,const char* file);
        /**
        * @brief receives a file from the client and saves it to files/
        * @param socket the client socket to receive from
        * @param file the filename to save as
        */
        void UPLOAD(int socket,const char* file);
        /**
        * @brief handles filename conflicts by appending _1, _2 etc
        * @param p the original filename
        * @return a unique filename that does not conflict with existing files
        */

        
        std::string fileRenameProcess(std::string p);
    public:
        /**
         * @brief Constructs DataConnection and binds to port 8081
         */
        DataConnection();//opens it up for connection
        /**
         * @brief starts the data connection accept loop and spawns a thread per client
         */
        void start();
 
};

#endif
