#include "baselogger.h"

BaseLogger::BaseLogger(std::string target, std::string desc){
    this->targetFile = target;
    this->desc = desc;
    
    //check if log exists, if it doesn't create folder and files
    std::filesystem::path folder("logs");

    if(!std::filesystem::exists(folder)){
        try{
            std::filesystem::create_directory(folder);
        }catch(std::exception& e){
            throw std::runtime_error(e.what());
        }
    }
    
    std::filesystem::path filePath = folder / target;

    //open log and check if file couldve been opened
    std::ofstream file(filePath,std::ios::app);

    if(!file.is_open()){
        std::cout << to_red("FAILED TO SET UP: logs/"+ target);
        throw std::runtime_error("Failed to open log file");
    }

    std::cout << to_green( "logs/"+target+" had no issues while initialising\n");
    file.close();
}

void BaseLogger::generateLog(STATUS status, std::string target, std::string desc){
    
    Log toLog(status,desc);
    //add this to the end of target file
    std::string targetDir = "logs/"+target;

    std::ofstream targetFile(targetDir,std::ios::app); // append mode 

    if(!targetFile.is_open()){
        throw std::runtime_error("BASE LOGGER: error while trying to open files" + target);
    }

    try{
        targetFile << toLog.getFormattedData();
    }
    catch( std::exception& e){
        throw std::runtime_error("BASE LOGGER: " + std::string(e.what()));
    }

    targetFile.close();
}

std::string BaseLogger::getDesc(){
    return desc;
}

BaseLogger::~BaseLogger(){
    //doubt that i'll use pointers
}

