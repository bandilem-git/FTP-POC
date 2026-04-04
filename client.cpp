#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <filesystem>
#include <sys/endian.h>
#include "COLORS.h"
//TUE MAR 31 22:07 onward goal : ACC MGMT USER and ANON support 
//data connection
#define DATAPORT 8081


//remove all white spaces in the word so [a b c  ]becomes [abc]
std::string to_trim(std::string x){
    int firstNonBlankPos = x.find_first_not_of(" \t\r\n");//position of first non blank element
    return x.substr(firstNonBlankPos,x.find_last_not_of(" \t\r\n") - firstNonBlankPos + 1);
}

void downloadThroughDataConnection(int port, const char* file){ 
    //client socket
    int clientSocket = socket(AF_INET,SOCK_STREAM,0);

    //if you can't connect then throw error
    if(clientSocket < 0 ){
        throw std::runtime_error("CLIENT: client could not connect");
    }

    //server address sockaddress input 
    sockaddr_in serverAddress;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_family = AF_INET;
    if(inet_pton(AF_INET, "127.0.0.1",&serverAddress.sin_addr)<0){
        close(clientSocket);
        throw std::runtime_error("CLIENT: ERROR WHILE CONNECTING TO DATA CONNECTION");
    } //on localhost note: inetaddr("127.0.0.1 ") depreceated damn

    if(connect(clientSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0){
        close(clientSocket);
        throw std::runtime_error("CLIENT: Error whiile trying to connect ");
    }

    std::string x(file);
    std::string in = "DOWNLOAD:"+x;

    if(send(clientSocket,in.c_str(), in.size(),0) < 0){
        close(clientSocket);
        throw std::runtime_error("Client -> DATACONNECTION : could not request file through DATACONNECTION");
    };

    int64_t fileSize;
    if(recv(clientSocket,&fileSize,sizeof(int64_t),0) < 0){
        close(clientSocket);
        throw std::runtime_error("DATACONNECTION -> CLIENT ERROR: ");
    }
    std::cout << fileSize << " bytes" << std::endl;

    std::ofstream newFile(file,std::ios::binary);

    int64_t totalReceived = 0;
    while(totalReceived < fileSize){
        
        char buffer[1024];
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer),0);
        if(bytesReceived < 0){
            close(clientSocket);
            throw std::runtime_error("CLIENT: could not receive entire file");
        }
        totalReceived+= bytesReceived;
        newFile.write(buffer, bytesReceived);
    }

}

void uploadThroughDataConnection (int port, const char* file){
    //client socket
    int clientSocket = socket(AF_INET,SOCK_STREAM,0);

    //if you can't connect then throw error
    if(clientSocket < 0 ){
        throw std::runtime_error("CLIENT: client could not connect");
    }

    //server address sockaddress input 
    sockaddr_in serverAddress;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_family = AF_INET;

    if(inet_pton(AF_INET, "127.0.0.1",&serverAddress.sin_addr)<0){
        close(clientSocket);
        throw std::runtime_error("CLIENT: ERROR WHILE CONNECTING TO DATA CONNECTION");
    } //on localhost

    if(connect(clientSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0){
        close(clientSocket);
        throw std::runtime_error("CLIENT: Error whiile trying to connect ");
    }


    std::string fileName = std::string(file);
    if(fileName.find("\\/")!= std::string::npos){
        fileName = fileName.substr(fileName.find_last_of("/")+1);
    }

    std::string upload = "UPLOAD:" + fileName;

    //debug line
    std::cout << upload << std::endl;

    char header[128];
    for(int i = 0; i < sizeof(header) / sizeof(char); i++){
        if(i < upload.length())
        header[i] = upload[i];
        else 
        header[i] = 0;
    }

    //send the request UPLOAD to DATA connection
    if(send(clientSocket, header, sizeof(header), 0) < 0){
        close(clientSocket);
        throw std::runtime_error("Client: could not send the size of the file");
    }   

    std::filesystem::path path = std::string(file);
    
    int64_t sizeOfFile = 0;

    try{
        sizeOfFile = std::filesystem::file_size(path);
    }
    catch(const std::filesystem::filesystem_error& e){
        throw std::runtime_error("File size error: " + std::string(e.what()));
    }

    if(send(clientSocket, &sizeOfFile, sizeof(int64_t),0) < 0){
        close(clientSocket);
        throw std::runtime_error("CLIENT: cannot send size file to DATA CONTROL");
    }

    std::ifstream fileToUpload(path, std::ios::binary);

    if(!fileToUpload.is_open()){
        close(clientSocket);
        throw std::runtime_error("CLIENT: Could not open file to upload, closing connection");
    }

    char chunk[4096];
    while (fileToUpload.read(chunk, sizeof(chunk)) || fileToUpload.gcount() > 0) {
        ssize_t bytesRead = fileToUpload.gcount();
        // ONLY send what was actually read from the disk
        ssize_t bytesSent = send(clientSocket, chunk, bytesRead, 0);
        if (bytesSent < 0) {
            fileToUpload.close();
            close(clientSocket);
            throw std::runtime_error("CLIENT: Send failed during transfer");
        }
        
        // Safety: If send sent less than we asked, TCP buffer is weird
        if (bytesSent < bytesRead) {
            std::cerr << to_yellow("Warning: partial send occurred") << std::endl;
        }
    }

    std::cout << to_green("FILE TRANSFER COMPLETE")<< std::endl;
    close(clientSocket);
    fileToUpload.close();


}

void downloadhandle(int clientSocket){
            std::cin.clear();
            //receive list of files you can download, type beat
            char buffer[1024] = {0};
            if(recv(clientSocket,buffer,sizeof(buffer),0) < 0){
                close(clientSocket);
                throw std::runtime_error("CLIENT: Could not receive message from Elsewhere");
            }
            //show client the list 
            
            if(std::string(buffer) == "FAIL\n"){
                std::cout << "NO FILES AVAILABLE FOR DOWNLOAD" << std::endl;
                return;
            }
            
            std::cout << buffer;
            //choose a file
            std::string chosenFile;
            std::getline(std::cin,chosenFile);

            // should this be attached? maybe maybe not
            chosenFile+="\n";// add it apparently??
            //send the chosen file  and
            if(send(clientSocket,chosenFile.c_str(),chosenFile.size(),0)< 0){
                close(clientSocket);
                throw std::runtime_error("file could not be sent");
            }

            //buffer_1 gonna have the port and file
            char buffer_1[1024] = {0};
            if (recv(clientSocket, buffer_1,sizeof(buffer_1),0) < 0){
                close(clientSocket);
                throw std::runtime_error("could not receivee port and file");
            }

            
            //separate them into parts  { port , file.extension } 
            std::vector<std::string> parts;
            std::string toProc(buffer_1);

            //split by : 
            parts.push_back(toProc.substr(0,toProc.find(":")));
            parts.push_back(toProc.substr(toProc.find(":") + 1));

            if(parts[0] == std::to_string(DATAPORT)){
                //debugging lines on client side

                downloadThroughDataConnection(DATAPORT,parts[1].c_str());

            }
            else{
                std::cout << parts[0]<<std::endl;
            }
}

void uploadhandle(int clientSocket){
    // glocbal stuct stat sb
    struct stat sb;
    //decide on which file should be upload
    std::string fileDirectory;

    //let user decide on the directory (should allow for copy and paste)
    std::cout << "Enter a directory: " << std::endl;
    std::getline(std::cin,fileDirectory);
            
    //checking that directory exists    
    //filedirectory
    std::filesystem::path p = fileDirectory;

    //edge case: "/" was not added
    if(fileDirectory[fileDirectory.length()-1] !='/'){
        fileDirectory+="/";
        }
            
    //using filesystem (c++17 feature)
    if(!std::filesystem::exists(p)){
        std::cout << "FILE PATH: " << to_red(fileDirectory) << " DOES NOT EXIST" << std::endl;
        return;
    }

    //tell client that it was valid
    std::cout << to_green(fileDirectory + " is a valid directory" +"\n");

    //files in the directory as a vector 
    std::vector<std::string> f_s;

    //reading all files in the directory
    try{
        for(const auto& e : std::filesystem::directory_iterator(fileDirectory)){
            std::filesystem::path out = e.path();
            std::string outfilename_str = out.string();
            const char* path = outfilename_str.c_str();

            if (stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR)){
                std::string p(path);
                f_s.push_back(p.substr(fileDirectory.length()));
                }
            }
        }
        catch(const std::exception& e){
            std::cerr << "Directory error: " << e.what() << std::endl;
        }

        //list the avail files
        std::cout << "\nAvailable files: "<<std::endl;
        std::string visFiles = "";

        //if there are no files in the directory then start over
        if(f_s.empty()){
            std::cout << "No files are available"<< std::endl;
            std::cout << to_yellow("Cancelling upload operation") << std::endl;
            return;
        }
        
        //for printing purposes
        for(std::string& x : f_s){
            visFiles+=x+"\n";
        }
        //print what is available
        std::cout << visFiles;
            
        //selecting file to upload
        std::cout << "Select file to upload: "<< std::endl;

        //chosen file
        std::string choice;
        std::getline(std::cin,choice);

        //removes the spaces... pray that they don't have spaces, i'll account for spaces later
        choice = to_trim(choice);
            
        //check if the file being selected exists
        bool found = false;

        for(const std::string& file : f_s){
            if(file == choice){
                found = true;
                break;
            }
        }

        //if not found start over 
        if(!found){
            std::cout << to_red("FILE DOES NOT EXIST")<< std::endl;
            std::cout << to_yellow("CANCELLING OPERATION")<< std::endl;
            return;
        }

        char buffer[1024] ={0};
        if(recv(clientSocket, buffer, sizeof(buffer),0) < 0){
            close(clientSocket);
            throw std::runtime_error("CLIENT: could not receive acceptance");
        }
            
        std::string ackMSG(buffer);

        //debugging line
        // std::cout << "acknowledgement message: " + ackMSG << std::endl;

        if(ackMSG.substr(0,2)!= "OK"){
            std::cout << to_red("CLIENT ERROR: non OK msg from Connection Control");
            return;
        }

        std::string dir_path = fileDirectory + choice;
        std::filesystem::path path = dir_path;
        int64_t size = 0;

        try{
            size = std::filesystem::file_size(path);
        }
        catch(const std::filesystem::filesystem_error& e){
            std::cerr << "File size error: " << e.what() << std::endl;
            close(clientSocket);
            return;
        }
        uploadThroughDataConnection(DATAPORT, dir_path.c_str());
}

void listhandle(int clientSocket){
    char buffer[1024] = {0};
    if(recv(clientSocket,buffer,sizeof(buffer),0) < 0){
    close(clientSocket);
    throw std::runtime_error("CLIENT: Could not receive message from Elsewhere");
    }
    std::cout << buffer;
}

void cmdhandle(int clientSocket){
    char buffer[1024] ={0};

    if(recv(clientSocket, buffer, sizeof(buffer),0) < 0){
        close(clientSocket);
        throw std::runtime_error("CLIENT: could not fetch commands");
    }
    std::cout << buffer << std::endl;
}

int main(){

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket < 0 ){
        throw std::runtime_error("CLIENT: client could not connect");
    }

    sockaddr_in serverAddress;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_family = AF_INET;

    if(inet_pton(AF_INET, "127.0.0.1",&serverAddress.sin_addr)<0){
        close(clientSocket);
        throw std::runtime_error("CLIENT: ERROR WHILE CONNECTING TO DATA CONNECTION");
    } 
    
    if(connect(clientSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0){
        close(clientSocket);
        throw std::runtime_error("CLIENT: Error whiile trying to connect ");
    }
    std::string in;

    while(true){
        //clear stream
        std::cin.clear();
        //read in whatever is typed in
        std::getline(std::cin,in);
        //remove any spaces
        in = to_trim(in);
        //attach newline symbol
        in+="\n";

        // always send whatever input is attached
        if(send(clientSocket,in.c_str(),in.size(),0) < 0){
            close(clientSocket);
            throw std::runtime_error("CLIENT: Could not send \""+ in + "\" from client to server");
        }
        
        //if download is called 
        if(in == "down\n"){
            downloadhandle(clientSocket);
        }
        
        //if upload is called
        if(in == "up\n"){
            uploadhandle(clientSocket);
        }

        //if list is called
        if(in == "ls\n"){
            listhandle(clientSocket);
        }
        
        //list commands
        if(in == "cmds\n"){
            cmdhandle(clientSocket);
        }
        
        //quitting
        if(in == "q\n" || in == "quit\n"){
            break;
        }

    }

    close(clientSocket);

    return 0;
}