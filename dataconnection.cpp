#include "dataconnection.h"

//every time theres a connection add them to a queue of people that 
DataServer::DataServer() : BaseServer(DATAPORT){}
 
void DataServer::start(){
    while (true){
        try{
        int tp = (accept(ConnectionServerSocket, nullptr, nullptr));
        if(tp < 0){
            close(ConnectionServerSocket);
            throw std::runtime_error("error while accepting connection");
        }
        numConnections++;
        this->notify(DATA, Log(LOG, "Client connected on data port: " + std::to_string(tp)));
        
        std::thread t ([this](int socket){//thread that initiates file transfer
        this->notify(DATA, Log(LOG, "Thread started for data socket: " + std::to_string(socket)));
        try    //get the name of the file that you want to clone
        {   char buffer[1024] = {0};
            int bytes = recv(socket, buffer, sizeof(buffer), 0);
            if(bytes <= 0){
                this->notify(DATA, Log(ERROR, "Failed to receive header"));
                close(socket);
                return;
            }

            std::string toClone(buffer, bytes);
            //convert char [] to str
            this->notify(DATA, Log(LOG, std::to_string(socket) + " RAW HEADER: " + toClone.substr(0,toClone.length()-1)));
            size_t colonPos = toClone.find(":");
            if (colonPos == std::string::npos) {
                std::cout << "Invalid header received: " << toClone << std::endl;
                close(socket);
                return; 
            }

            std::string type = toClone.substr(0, colonPos);
            std::string fileEx = toClone.substr(colonPos + 1);

            this->notify(DATA, Log(LOG, std::to_string(socket) + " TYPE: " + type + " FILE: " + fileEx));

            if(type == "DOWNLOAD"){
                this->notify(DATA, Log(LOG,std::to_string(socket) + " DOWNLOAD requested: " + fileEx));
                this->notify(DATA, Log(LOG,"Opening file: files/" + fileEx));
                DOWNLOAD(socket,fileEx.c_str());
            }
            else if(type == "UPLOAD"){
                this->notify(DATA, Log(LOG,std::to_string(socket) + " UPLOAD requested: " + fileEx));
                this->notify(DATA, Log(LOG,"Opening file: files/" + fileEx));   
                UPLOAD(socket,fileEx.c_str());
            }

            this->notify(DATA, Log(LOG, "Thread ended for data socket: " + std::to_string(socket)));

        }
            catch(std::exception& x){
                std::cout << x.what() << std::endl;
            }


        }, tp);

        
        t.detach();

        }
        catch(const std::exception& x){
            std::cout << x.what();
        }

    }
}

void DataServer::DOWNLOAD(int socket,const char* file){  
    std::string toClone(file);
    
    std::cout << "Downloading file: " << toClone<< std::endl; 

    if(toClone.find(".")!= std::string::npos){
        //remove '\n' or '\r\n'
        std::string terminatingChars = toClone.substr(toClone.length()-2,2);
        if(terminatingChars == "\r\n"){
            toClone = toClone.substr(0,toClone.find("\r\n"));
        }
        else{
            toClone = toClone.substr(0,toClone.find("\n"));
        }
                    
        //open file
        std::ifstream toDownload("files/"+toClone, std::ios::binary);// read in as binary files

        if(!toDownload.is_open()){
            close(socket);
            throw std::runtime_error("DATA SERVER: could not open file for transfer");
        }
        
        //seek to the end 
        toDownload.seekg(0, std::ios::end);

        //record file size
        int64_t fileSize = toDownload.tellg();
        this->notify(DATA, Log(LOG, "File size: " + std::to_string(fileSize) + " bytes"));

        //seek to beginnning again
        toDownload.seekg(0, std::ios::beg);

        //send the size of the file
        if(write(socket, (const char*)& fileSize , sizeof(fileSize))< 0){
            close(socket);//g count = numchars extracted
            throw std::runtime_error("DATASERVER: cannot send file size");
        }

        char chunk[4096];
        int64_t sent = 0;
        while(toDownload.read(chunk, sizeof(chunk))){
            int64_t s = send(socket,chunk,toDownload.gcount(),0);
            if(s < 0){
                close(socket);//g count = numchars extracted
                throw std::runtime_error("DATASERVER: cannot send file bytes");
            }
            sent+=s;
            this->notify(DATA, Log(DOWNLOADING,"Progress: " + std::to_string(sent) + "/" + std::to_string(fileSize)));
        }

        // handle the last partial chunk after the loop exits
        if(send(socket, chunk, toDownload.gcount(), 0) < 0){
            this->notify(DATA, Log(ERROR," COULD NOT SEND LAST CHUNK"));
            close(socket);//g count = numchars extracted
            throw std::runtime_error("DATASERVER: cannot send file");
        };

        this->notify(DATA, Log(LOG,"Download complete: " + toClone));
        toDownload.close();
        close(socket);
    }           
}

void DataServer::UPLOAD(int socket,const char* file){
    this->notify(DATA, Log(UPLOADING," UPLOADING INITIATED"));
    //const char* to std::string
    std::string fn = file;
    //find backslash
    std::size_t lastSlash = fn.find_last_of("/\\");
    //extract the file name
    std::string fileName = (lastSlash == std::string::npos) ? fn : fn.substr(lastSlash + 1);
    // size of the file to upload
    int64_t sizeOfUpFile;
    // store the size in the size of upload file var (sizeOfUpFile)
    if(recv(socket, &sizeOfUpFile, sizeof(int64_t),MSG_WAITALL) < 0){ // WAITALL ensures you get all 8 bytes
        close(socket);
        this->notify(DATA,Log(ERROR,"COULD NOT GET THE SIZE OF THE FILE"));
        throw std::runtime_error("DATASERVER: cannot get size of file from CLIENT");
    }
    this->notify(DATA,Log(LOG, "RECEIVED THE SIZE OF THE FILE: " + std::to_string(sizeOfUpFile)));

    int i = countExistingfiles(fileName); // if any files have the same name 

    if(i > 0){
        int dotPos = fileName.find(".");
        std::string fileNoExt = fileName.substr(0, dotPos);
        std::string Extension = fileName.substr(dotPos);
        fileNoExt += std::to_string(i)+=Extension;
        fileName = fileNoExt;
    };
    this->notify(DATA, Log(LOG,"FILE NAME PROCESSED"));
    //upload every file to "files/file.fileExtension" name 
    std::ofstream toUpload("files/"+fileName, std::ios::binary);
    //check if it was possible to create
    if(!toUpload.is_open()){
        this->notify(DATA, Log(ERROR,"FILE COULD NOT BE PROCESSED: "+ fileName));

        close(socket);
        throw std::runtime_error("DATASERVER: could not open file : files/" + fileName);  
    }
    //chunks
    char chunk[4096] ={0};
    int64_t totalReceived = 0;
    this->notify(DATA,Log(UPLOADING, "UPLOAD PROCESS BEGINNING"));

    while (totalReceived < sizeOfUpFile) {
            int64_t remaining = sizeOfUpFile - totalReceived;
            size_t toRead = (remaining < (int64_t)sizeof(chunk)) ? (size_t)remaining : sizeof(chunk);

            int bytesRec = recv(socket, chunk, toRead, 0);
            
            if (bytesRec < 0) {
                this->notify(DATA, Log(ERROR, "Recv error during transfer: " + std::string(strerror(errno))));
                break; // Exit loop to allow cleanup
            } 
            if (bytesRec == 0) {
                this->notify(DATA, Log(ERROR, "Client closed connection prematurely at " + std::to_string(totalReceived) + " bytes"));
                break;
            }

            toUpload.write(chunk, bytesRec);
            totalReceived += bytesRec;
        }

        if (totalReceived == sizeOfUpFile) {
            this->notify(DATA, Log(LOG, "UPLOAD COMPLETE: " + fileName));
        } else {
            this->notify(DATA, Log(ERROR, "UPLOAD INCOMPLETE: Saved " + std::to_string(totalReceived) + "/" + std::to_string(sizeOfUpFile)));
        }


    close(socket);
    toUpload.close();
}

int DataServer::countExistingfiles(std::string p){
    this->notify(DATA, Log(LOG,"Checking existing files for: " + p));

    int count = 0;
    if(std::filesystem::exists(p)){
        count++;
    }
    std::vector<std::string> f;
    try{
        for(const auto& e : std::filesystem::directory_iterator("files/")){
            std::filesystem::path out = e.path();
            std::string outfilename_str = out.string();
            const char* path = outfilename_str.c_str();

            if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)){
                std::string p(path);
                f.push_back(p.substr(6,p.length()));
            }
        }
    }
    catch(const std::exception& e){
        std::cerr << "Directory error: " << e.what() << std::endl;
    }
    
    //temp solution
    for(std::string& x: f){
        if(x[0] == p[0]){
            //if files length is less than new file its obviuously not the same file
            if(x.length() < p.length()){
                continue;
            }
            //iterate through each char for the same len as p since at this poit x len >= p len
            for(int i = 0; i < p.length();i++){
                if(p[i]!= x[i]){//x = file1 p =  file
                    continue;
                }
            }
            count++;
        }
    }
    this->notify(DATA, Log(LOG,"Existing file count: " + std::to_string(count)));
    return count;

}