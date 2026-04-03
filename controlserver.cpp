#include "controlserver.h"
ControlServer::ControlServer() : BaseServer(CONTROLPORT){}

void ControlServer::start(){
    while(true){
        try{
        //accepting client connection
        int clientSocket = accept(ConnectionServerSocket, nullptr, nullptr);
        std::thread t([this](int socket) {
            try{
                while(true){               
                //receive Data from Client
                char buffer[1024] = {0};
                int bytes = recv(socket, buffer, sizeof(buffer), 0);

                if(bytes <= 0){
                    close(socket);
                    break;
                }

                //convert char[] to string  
                std::string msg(buffer); 
                    
                //q to quit
                if(msg == "q\n"|| msg == "q\r\n"){
                    std::cout << "Disconnecting the client"<< std::endl;
                    break;
                }
                //up to upload
                
                if(msg == "up\n"|| msg == "up\r\n"){

                    char resp[3]= "OK";

                    if(send(socket,resp ,sizeof(resp),0) < 0){
                        throw std::runtime_error("CONTROL SERVER: COULD NOT ACCEPT FILE");
                    };

                }
                
                //list files to download
                if(msg == "ls\n" || msg == "ls\r\n"){
                    std::string files;
                    {// lock and do what you have to 
                        std::lock_guard<std::mutex> lck(mtx);
                        update();
                        files = getListOfFilesUnsafe();

                    }

                    send(socket, files.c_str(), files.size(), 0);


                }

                // files to download
                if(msg == "down\n"|| msg == "down\r\n"){
                    //inform client about available files
                    std::string templateString;
                    {
                        std::lock_guard<std::mutex> lck(mtx);
                        update();
                        templateString = "\nAvailable Files:\n";
                        templateString += getListOfFilesUnsafe();
                    }                    
                    templateString+="\n";
                    templateString+="enter name of the file that you want (case sensitive)\n";

                    //send data to client
                    send(socket, templateString.c_str(), templateString.size(), 0);

                    //receive file to download
                    char fileToDownload[1024] = {0};
                    int bytes = recv(socket, fileToDownload, sizeof(fileToDownload), 0);

                    if(bytes <= 0){
                        close(socket);
                        break;
                    }
                    //convert data received from char arr to string
                    std::string tryDownlaod(fileToDownload);

                    //extract until \n
                    tryDownlaod = tryDownlaod.substr(0, tryDownlaod.find('\n'));

                    //find file 
                    if(!fileExists(tryDownlaod)){
                        //send the error message to the client
                        std::string errResp = to_red(tryDownlaod + " is a file that does not exist. \n PLEASE TRY AGAIN:");
                        send(socket, errResp.c_str(), errResp.size(), 0);

                        //tell ConnectionServer that a file that doesn't exist is attempting to be downloaded
                        std::cout << tryDownlaod << " does not exist on the Connection Server\n";
                            
                        //TODO: add error logging
                        continue;
                    }
                    
                    std::string INITIATE = "8081:"+ tryDownlaod;

                    if(send(socket, INITIATE.c_str(), INITIATE.size(), 0) < 0){
                        close(socket);
                        throw std::runtime_error("CONNECTION SERVER: COULD NOT SEND REQUIREMENTS");
                    }
                }

                //list all available commands in FTPCMDS.txt
                if(msg == "cmds\n"||msg == "cmds\r\n"){
                    std::string blankInput = "=================================\n";
                    blankInput +="|\t";
                    blankInput += "LIST OF COMMANDS\t|";
                    blankInput += "\r\n=================================\r\n";
                    blankInput+="\n";
                    blankInput+= getavailableCommands();
                        
                    if(send(socket, blankInput.c_str(), blankInput.size(), 0) < 0){
                        close(socket);
                        throw std::runtime_error("CONNECTION SERVER: COULD NOT SEND REQUIREMENTS");
                    }
                }
            }
            close(socket);
            }
            catch(std::exception& e){
                std::cout << e.what() << std::endl;
            }
        }, clientSocket);
        
        t.detach();

        }
        catch(const std::exception& x){
            std::cout << x.what();
        }
    }
}

bool ControlServer::fileExists(std::string x){
    std::lock_guard<std::mutex> lck(mtx);
    std::cout << "COMPARING INITIATED" << std::endl;

    for(const std::string& y : existingFiles){
        if(y == x){
            std::cout << "FILE HAS BEEN FOUND HENCE TRANSFER IS INITIATING " << std::endl;
            return true;
        }
    }
    std::cout << "FILE CANNOT BE FOUND HENCE NO TRANSFER" << std::endl;
    return false;
}

std::string ControlServer::getavailableCommands(){
    std::lock_guard<std::mutex> lck(mtx);
    const char*  toRead = "FTPCMDS.txt";
    std::ifstream fileToRead(toRead);

    std::string temp;
    std::string toRet = "";
    while(getline(fileToRead, temp)){
        toRet+=temp;
        toRet+= "\n";
    }

    return toRet;

}

std::string ControlServer::getListOfFiles(){   
    std::lock_guard<std::mutex> lck(mtx);
    std::string finalListOfFiles = "";

    for(const std::string& x: existingFiles){
        finalListOfFiles += x + "\n";
    }

    return finalListOfFiles;
}

void ControlServer::update(){
    existingFiles.clear();

    std::string fileDirectory = "files/";

    try{
        for(const auto& e : std::filesystem::directory_iterator(fileDirectory)){
            std::filesystem::path out = e.path();
            std::string outfilename_str = out.string();
            const char* path = outfilename_str.c_str();

            if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)){
                std::string p(path);
                existingFiles.push_back(p.substr(6));
            }
        }
    }
    catch(const std::exception& e){
        std::cerr << "Directory error: " << e.what() << std::endl;
    }
}

int ControlServer::getNumFiles(){
    std::lock_guard<std::mutex> lck(mtx);    
    return existingFiles.size();
}

std::string ControlServer::getListOfFilesUnsafe(){   
    std::string finalListOfFiles = "";

    for(const std::string& x: existingFiles){
        finalListOfFiles += x + "\n";
    }

    return finalListOfFiles;
}