#include "controlserver.h"
ControlServer::ControlServer() : BaseServer(CONTROLPORT){}

void ControlServer::start(){
    while(true){
        try{
        //accepting client connection
        int clientSocket = accept(ConnectionServerSocket, nullptr, nullptr);
        if(clientSocket > 0 ){
            this->notify(CONTROL, Log(LOG, std::to_string(clientSocket) + ": client has connected "));
        }
        else{
            this->notify(CONTROL, Log(ERROR, std::to_string(clientSocket) + ": client could not conect"));
        }
        std::thread t([this](int socket) {
            this->notify(CONTROL, Log(LOG, "Thread started for socket: " + std::to_string(socket)));
            try{
                while(true){               
                //receive Data from Client
                char buffer[1024] = {0};
                int bytes = recv(socket, buffer, sizeof(buffer), 0);

                if(bytes == 0){
                    this->notify(CONTROL, Log(LOG, std::to_string(socket) + " disconnected gracefully"));
                    close(socket);
                    break;
                }
                else if(bytes < 0){
                    this->notify(CONTROL, Log(ERROR,std::to_string(socket) + " recv error: " + strerror(errno)));
                    close(socket);
                    break;
                }

                
                //convert char[] to string  
                std::string msg(buffer); 
                this->notify(CONTROL, Log(LOG, std::to_string(socket) + ": sent a message: " + msg.substr(0,msg.length()-1)));

                //q to quit
                if(msg == "q\n"|| msg == "q\r\n"){
                    this->notify(CONTROL, Log(LOG, std::to_string(socket) + " is Disconecting"));
                    std::cout << "Disconnecting the client"<< std::endl;
                    break;
                }
                //up to upload
                
                else if(msg == "up\n"|| msg == "up\r\n"){

                    char resp[3]= "OK";
                    this->notify(CONTROL, Log(LOG,std::string(resp) + " response sent to client: " + std::to_string(socket)));

                    if(send(socket,resp ,sizeof(resp),0) < 0){
                        this->notify(CONTROL, Log(ERROR, "COULD NOT SEND OK RESPONSE TO CLIENT: " + std::to_string(socket)));
                        throw std::runtime_error("CONTROL SERVER: COULD NOT ACCEPT FILE");
                    };

                }
                
                //list files to download
                else if(msg == "ls\n" || msg == "ls\r\n"){
                    std::string files;
                    {// lock and do what you have to 
                        std::lock_guard<std::mutex> lck(mtx);
                        this->notify(CONTROL, Log(LOG, "LOCK ACQUIRED for file list"));
                        update();
                        files = getListOfFilesUnsafe();

                    }
                    this->notify(CONTROL, Log(LOG,std::to_string(socket) + " - CLIENT REQUESTED LIST OF COMMANDS"));
                    if(send(socket, files.c_str(), files.size(), 0) < 0){
                        this->notify(CONTROL, Log(ERROR,"COULD NOT SEND THE LIST TO THE CLIENT"));
                        close(socket);
                        throw std::runtime_error("COULD NOT SEND THE LIST TO THE CLIENT");
                    }


                }

                // files to download
                else if(msg == "down\n"|| msg == "down\r\n"){
                    //inform client about available files
                    this->notify(CONTROL, Log(LOG,std::to_string(socket) + " - PHASE 1: CLIENT REQUESTED FILE"));
                    std::string templateString;
                    {
                        std::lock_guard<std::mutex> lck(mtx);
                        this->notify(CONTROL, Log(LOG, "LOCK ACQUIRED for file list"));
                        update();
                        templateString = "\nAvailable Files:\n";
                        templateString += getListOfFilesUnsafe();
                    }                    
                    templateString+="\n";
                    templateString+="enter name of the file that you want (case sensitive)\n";

                    this->notify(CONTROL, Log(LOG,std::to_string(socket) + " - PHASE 1: CLIENT REQUESTED FILE"));

                    //send data to client
                    if(send(socket, templateString.c_str(), templateString.size(), 0) < 0){
                        this->notify(CONTROL, Log(ERROR,std::to_string(socket) + " - PHASE 1 FAILED"));
                        this->notify(CONTROL, Log(ERROR,std::to_string(socket) + " - PHASE 1: CLIENT DID NOT RECEIVE LIST OF FILES "));

                        close(socket);
                        throw std::runtime_error("CONTROL CONNECTION: COULD NOT SEND LIST OF FILES TO CLIENT");
                    }

                    //receive file to download
                    char fileToDownload[1024] = {0};
                    int bytes = recv(socket, fileToDownload, sizeof(fileToDownload), 0);

                    std::string ftd (fileToDownload);
                    this->notify(CONTROL, Log(LOG,std::to_string(socket) + " - PHASE 1 SUCCESS"));
                    this->notify(CONTROL, Log(LOG,std::to_string(socket) + " - PHASE 2: CLIENT REQUESTED FILE \"" + ftd.substr(0,ftd.length()-1) +"\"" ));

                    if(bytes == 0){
                        this->notify(CONTROL, Log(LOG, std::to_string(socket) + " disconnected gracefully"));
                        close(socket);
                        break;
                    }
                    else if(bytes < 0){
                        this->notify(CONTROL, Log(ERROR,std::to_string(socket) + " recv error: " + strerror(errno)));
                        close(socket);
                        break;
                    }
                    
                    
                    //convert data received from char arr to string
                    std::string tryDownlaod(fileToDownload);

                    //extract until \n
                    tryDownlaod = tryDownlaod.substr(0, tryDownlaod.find('\n'));

                    //find file 
                    if(!fileExists(tryDownlaod)){
                        this->notify(CONTROL, Log(ERROR,std::to_string(socket) + " - PHASE 2 FAILED"));
                        this->notify(CONTROL, Log(ERROR, tryDownlaod + "DOES NOT EXIST"));

                        //send the error message to the client
                        std::string errResp = to_red(tryDownlaod + " is a file that does not exist. \n PLEASE TRY AGAIN:");
                        send(socket, errResp.c_str(), errResp.size(), 0);

                        //tell ConnectionServer that a file that doesn't exist is attempting to be downloaded
                        std::cout << tryDownlaod << " does not exist on the Connection Server\n";
                        continue;
                    }
                    
                    this->notify(CONTROL, Log(LOG, std::to_string(socket) + " - PHASE 2 SUCCESS"));


                    std::string INITIATE = "8081:"+ tryDownlaod;// not necessary

                    this->notify(CONTROL, Log(LOG, std::to_string(socket) + " - PHASE 3: FILE TRANSFERRING"));

                    if(send(socket, INITIATE.c_str(), INITIATE.size(), 0) < 0){
                        close(socket);
                        this->notify(CONTROL, Log(ERROR, std::to_string(socket) + " - PHASE 3: FAILED"));

                        throw std::runtime_error("CONNECTION SERVER: COULD NOT SEND REQUIREMENTS");
                    }
                    this->notify(CONTROL, Log(LOG, std::to_string(socket) + " - PHASE 3: SUCCESS"));

                }

                //list all available commands in FTPCMDS.txt
                else if(msg == "cmds\n"||msg == "cmds\r\n"){
                    this->notify(CONTROL, Log(LOG, std::to_string(socket) + " - CLIENT REQUESTED AVAILABLE COMMANDS"));

                    std::string blankInput = "=================================\n";
                    blankInput +="|\t";
                    blankInput += "LIST OF COMMANDS\t|";
                    blankInput += "\r\n=================================\r\n";
                    blankInput+="\n";
                    blankInput+= getavailableCommands();
                        
                    if(send(socket, blankInput.c_str(), blankInput.size(), 0) < 0){
                        this->notify(CONTROL, Log(ERROR, std::to_string(socket) + " - COULD NOT SEND LIST AVAILABLE COMMANDS TO CLIENT"));
                        close(socket);
                        throw std::runtime_error("CONNECTION SERVER: COULD NOT SEND REQUIREMENTS");
                    }
                }
                else {
                    this->notify(CONTROL, Log(ERROR, std::to_string(socket) + " sent UNKNOWN command: " + msg));
                }

            }
            this->notify(CONTROL, Log(LOG, "Thread ended for socket: " + std::to_string(socket)));
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
    this->notify(CONTROL, Log(LOG, "LOCK ACQUIRED for file list"));
    this->notify(CONTROL, Log(LOG,"FILE SEARCH INITIATED"));
    std::cout << "COMPARING INITIATED" << std::endl;

    for(const std::string& y : existingFiles){
        if(y == x){
            this->notify(CONTROL, Log(LOG,"FILE FOUND"));
            std::cout << "FILE HAS BEEN FOUND HENCE TRANSFER IS INITIATING " << std::endl;
            return true;
        }
    }
    this->notify(CONTROL, Log(LOG,"FILE WAS NOT FOUND"));

    std::cout << "FILE CANNOT BE FOUND HENCE NO TRANSFER" << std::endl;
    return false;
}

std::string ControlServer::getavailableCommands(){
    std::lock_guard<std::mutex> lck(mtx);
    this->notify(CONTROL, Log(LOG, "LOCK ACQUIRED for file list"));
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
    this->notify(CONTROL, Log(LOG, "LOCK ACQUIRED for file list"));
    std::string finalListOfFiles = "";

    for(const std::string& x: existingFiles){
        finalListOfFiles += x + "\n";
    }

    return finalListOfFiles;
}

void ControlServer::update(){
     this->notify(CONTROL, Log(LOG, "Refreshing file list"));
    existingFiles.clear();

    std::string fileDirectory = "files/";

    try{
        for(const auto& e : std::filesystem::directory_iterator(fileDirectory)){
            std::filesystem::path out = e.path();
            std::string outfilename_str = out.string();
            const char* path = outfilename_str.c_str();

            if (stat(path, &sb) != 0) {
                this->notify(DATA, Log(ERROR, "Stat failed for " + std::string(path) + ": " + strerror(errno)));
                continue;
            }
            if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)){
                
                std::string p(path);
                existingFiles.push_back(p.substr(6));
            }
        }
    }
    catch(const std::exception& e){
        std::cerr << "Directory error: " << e.what() << std::endl;
    }
        this->notify(CONTROL, Log(LOG, "Files found: " + std::to_string(existingFiles.size())));
}

int ControlServer::getNumFiles(){
    std::lock_guard<std::mutex> lck(mtx);
    this->notify(CONTROL, Log(LOG, "LOCK ACQUIRED for Number of Files list"));    
    return existingFiles.size();
}

std::string ControlServer::getListOfFilesUnsafe(){   
    std::string finalListOfFiles = "";

    for(const std::string& x: existingFiles){
        finalListOfFiles += x + "\n";
    }

    return finalListOfFiles;
}