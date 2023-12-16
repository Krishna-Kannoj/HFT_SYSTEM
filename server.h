

// #################################################################
//                --------   server.cpp   ---------               //
// #################################################################


#include <sys/socket.h>
#include <netinet/in.h>
#include "StockSimulation.h"

#ifndef LINE_H
 #define LINE_H
#include "SocketCreation.h"
#endif


class TCPServer {
public:

    void start();
    
    void sendIntValues();
    
    void processOrders();
        
};

void TCPServer::start() {

    std::thread clientThread1(&TCPServer::sendIntValues, this);
    std::thread clientThread2(&TCPServer::processOrders, this);
    
    clientThread1.join();
    clientThread2.join();

}

void TCPServer::sendIntValues() {
    
    // Create a socket
    createSocket socketObjserver1;
    int serverSocket = socketObjserver1.createSocketforHFTserver(8080);

    if(serverSocket == -1){
        return ;
    }
    sockaddr_in clientAddress{};
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);

    if (clientSocket == -1) {
        std::cerr << "Server:  Error accepting connection : sendTickData" << std::endl;
        close(serverSocket);
        return ;
    }
    
    while (true) {
        
        // Set simulation Parameters for geometric Brownian motion
        // StockSimulationParameters              (double initialPrice, double volatility, double drift, double dt)
        StockSimulationParameters simulationParams(100.0,               0.02,              0.0001,       1.0 / 252.0);
    
        // Create simulator with the parameters
        StockSimulator simulator(simulationParams);
        
        // Number of data points
        int numPoints = 1000;

        // Seed the random  number generator
        std::srand(std::time(0));
        
        // Sending simulated stock price to client
        for (int i = 0; i < numPoints; ++i) {
            
            double live_data;
            live_data = simulator.simulate(i);
            send(clientSocket, &live_data, sizeof(live_data), 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
    }
    // Close the sockets
    close(clientSocket);
    close(serverSocket);
}

void TCPServer::processOrders() {
        
        // Create a socket
        createSocket socketObjserver2;
        int serverSocket = socketObjserver2.createSocketforHFTserver(8081);

        if(serverSocket == -1){
            return ;
        }
        sockaddr_in clientAddress{};
        socklen_t clientAddressSize = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);

        if (clientSocket == -1) {
            std::cerr << "Server:  Error accepting connection : processOrders" << std::endl;
            close(serverSocket);
            return ;
        }
        
        
        // Receiving order from client
        // formate: "stock_name type(buy/sell) price quantity"
        char buffer[1024];
        while(recv(clientSocket, buffer, sizeof(buffer), 0) > 0){
            buffer[sizeof(buffer)] = '\0';
            
            // For now send success message to every order
            std::string orderMessage("Successfull");
            
            int messageLength = orderMessage.length();
            // Send the string data
            send(clientSocket, orderMessage.c_str(), messageLength, 0);
            
        }
        close(clientSocket);
        close(serverSocket);
    }









