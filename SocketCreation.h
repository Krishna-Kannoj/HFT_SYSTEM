// #include "SocketCreation.h"
#include <unistd.h>
#include <arpa/inet.h>
struct createSocket{
    
    int createSocketforHFTserver(int portNo){

        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            std::cerr << "Server:  Error creating socket in server" << std::endl;
            return -1;
        }
    
        // Set up the server address structure
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(portNo);
    
        // Bind the socket to the address and port
        if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            std::cerr << "Server:  Error binding socket in server" << std::endl;
            close(serverSocket);
            return -1;
        }
    
        // Listen for incoming connections
        if (listen(serverSocket, 10) == -1) {
            std::cerr << "Server:  Error listening for connections in server" << std::endl;
            close(serverSocket);
            return -1;
        }

        std::cout << "Server:  Server listening on port 8080..." << std::endl;

        return serverSocket;
    }
    
    int createSocketforHFTclient(int portNo) {

        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Client:  Error creating socket in client" << std::endl;
            return -1;
        }
    
        // Set up the server address structure
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use the server's IP address
        // serverAddress.sin_addr.s_addr = in_addr("127.0.0.1"); // Use the server's IP address
        serverAddress.sin_port = htons(portNo);
    
        // Connect to the server
        if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            std::cerr << "Client:  Error connecting to server in client" << std::endl;
            close(clientSocket);
            return -1;
        }

        std::cout << "Client:  Connected to the server" << std::endl;
        return clientSocket;       
    }
    
};
