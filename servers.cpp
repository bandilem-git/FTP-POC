#include "dataserver.h"
#include "controlserver.h"
#include "controllogger.h"
#include "datalogger.h"
#include <thread>

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
        std::filesystem::remove(path);
        std::filesystem::create_directory(path);

    }

    //startup loggers
    ControlLogger ctrlLOGS("controlconnection");
    DataLogger dataLOGS("dataconnection");

    DataServer dataserver;
    ControlServer controlserver;

    dataserver.subscribe(DATA, &dataLOGS);
    controlserver.subscribe(CONTROL, &ctrlLOGS);

    std::thread dataServerThread([&]()
    {    
        dataserver.start();
    });
    
    std::thread connectionServerThread([&](){
        controlserver.start();

    });
    dataServerThread.detach();

    connectionServerThread.detach();
    while(true) std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}