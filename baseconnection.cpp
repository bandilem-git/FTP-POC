#include "baseconnection.h" 

void BaseConnection::subscribe(CONNECTIONS type, BaseLogger* observer){
    observers[type].push_back(observer);
}

BaseConnection::BaseConnection(int port){
    BindAndListen(port);
}

void BaseConnection::BindAndListen(int port){
    std::printf("Atempting to listen to PORT: %d",port); 

    //ConnectionServer Address
    //sockadde is the data type that stores network information
    sockaddr_in ConnectionServerAddress;
        ConnectionServerAddress.sin_family = AF_INET;
        ConnectionServerAddress.sin_port = htons(port);// port to Network byte order 
        ConnectionServerAddress.sin_addr.s_addr = INADDR_ANY;// accept any ip address


    //binding
    std::cout << to_yellow("ATTEMPTING TO BIND TO PORT: ") << port << std::endl;
    
    if(bind(
        ConnectionServerSocket,
        (struct sockaddr*) &ConnectionServerAddress,
        sizeof(ConnectionServerAddress)
    ) < 0){
        std::cout << to_red("COULD NOT LISTEN ON PORT: ") << port<< std::endl;
        close(ConnectionServerSocket);
        throw std::runtime_error("error while binding for incoming clients");
    }

    std::cout << to_green("BINDING to port ") + to_white(std::to_string(port)) +to_green(" COMPLETE") << std::endl;

    std::cout << to_yellow("ATTEMPTING TO LISTEN ON PORT: ") << port << std::endl;

    //listening for incoming conns
    if(listen(ConnectionServerSocket, 5) < 0){
        std::cout << to_red("COULD NOT LISTEN ON PORT: ")<< port << std::endl;
        close(ConnectionServerSocket);
        throw std::runtime_error("error while listening for incoming clients");
    };
    std::cout << to_green("NOW LISTENING on port ") + to_white(std::to_string(port)) +to_green(" COMPLETE") << std::endl;

}

void BaseConnection::notify(CONNECTIONS connection, Log log){
    for(BaseLogger* x: observers[connection]){
        x->onEvent(log);
    }
}

BaseConnection::~BaseConnection(){}