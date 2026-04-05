#ifndef CONTROLCONNECTION_H
#define CONTROLCONNECTION_H

#include "baseconnection.h"
#define CONTROLPORT 8080

/*
Server class: emulating (CONTROL SERVER)FILE TRANSFER PROTOCOL in C++17
*/

class ControlConnection : public BaseConnection{
    private:
        /**
         * @brief function that gets a list of available files in the 'files folder'
         * @returns newline sparated list of files
         */
        std::string getListOfFiles();

        /**
         * @brief number of files int the 'files folder'
         * @returns gets the number of files in the 'files folder'
         */
        int getNumFiles();

        /**
         * @brief checks if the file exists 
         * @returns true / false
         */
        bool fileExists(std::string x);

        /**
         * @brief list of already existing files
         * @returns A vector of file names 
         */
        std::vector<std::string> existingFiles;
        /**
         * @brief updates the existingFiles variable
         */
        void update();

    public:
        /**
         * @brief for easier access to lock
         */
        std::mutex mtx;

        /**
        * @brief Constructs ControlConnection and binds to port 8080
        */
        ControlConnection();

        /**
        * @brief reads available commands from FTPCMDS.txt
        * @return newline separated list of available commands
        */
        std::string getavailableCommands();

        /**
        * @brief starts the control connection accept loop and spawns a thread per client
        */
        void start();

        /**
        * @brief gets list of files without acquiring the mutex lock
        * @note only call this when mtx is already held
        * @return newline separated list of files
        */
        std::string getListOfFilesUnsafe();//threaded operations

};

#endif