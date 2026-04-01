#include "dataserver.h"

int main(){
    //ensure the files directory always exists
    std::filesystem::path path = "files/";

    //check to see if it exists
    bool exists = std::filesystem::exists(path);
    //check if path is even a directory 
    bool isDirectory = std::filesystem::is_directory(path);

    if(!exists){
        std::filesystem::create_directory(path);
    }
    else if(!isDirectory){
        std::filesystem::create_directory(path);

    }

    std::thread dataServerThread([]()
    {    
        DataServer dataserver;
        dataserver.start();
    });
    
    std::thread connectionServerThread([](){
        ControlServer controlserver(8080);
        controlserver.start();

    });
    dataServerThread.join();

    connectionServerThread.join();

    return 0;
}