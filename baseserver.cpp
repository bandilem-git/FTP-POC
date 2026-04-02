#include "baseserver.h" 


void BaseServer::BindAndListen(int port){
    // std::printf("Atempting to listen to PORT: %d",port); 
    //SocketConnectionServer
    int clientSocket;


    //ConnectionServer Address
    //sockadde is the data type that stores network information
    sockaddr_in ConnectionServerAddress;
        ConnectionServerAddress.sin_family = AF_INET;
        ConnectionServerAddress.sin_port = htons(port);// port to Network byte order 
        ConnectionServerAddress.sin_addr.s_addr = INADDR_ANY;// accept any ip address


    //binding
    if(bind(
        ConnectionServerSocket,
        (struct sockaddr*) &ConnectionServerAddress,
        sizeof(ConnectionServerAddress)
    ) < 0){
        close(ConnectionServerSocket);
        throw std::runtime_error("error while binding for incoming clients");
    }

    //listening for incoming conns
    if(listen(ConnectionServerSocket, 5) < 0){
        close(ConnectionServerSocket);
        throw std::runtime_error("error while listening for incoming clients");
    };
}