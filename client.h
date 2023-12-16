// #################################################################

//          --------       client.cpp      ---------                //

// #################################################################


#pragma once

#include "stockClass.h"
#include "allStrategy.h"

#ifndef LINE_H
 #define LINE_H
#include "SocketCreation.h"
#endif


class HFTClient {
    
public:

    // Constructor
    HFTClient() {}

    // Run method to start the client logic
    void start(); 
    
    // Recieving Tick data from server
    void receiveLiveTickData(tickClass& ticker);
};

void HFTClient::start() {
    // Create tickClass object
    tickClass ticker;

    // Create Strategy object and provide live/tickData with synchronization
    allStrategy allStrategyObj(ticker);

    // Create a thread to update liveTickData every 100 milliseconds using TCP connection
    std::thread updateThread(&HFTClient::receiveLiveTickData, this, std::ref(ticker));
    
    // Create a thread to run the strategy
    std::thread strategyThread(&allStrategy::runStrategy, &allStrategyObj);

    // Wait for threads to finish (this example runs indefinitely, so you may need to use some termination condition)
    updateThread.join();
    strategyThread.join();
}

void HFTClient::receiveLiveTickData(tickClass& ticker) {
    createSocket socketObj;
    int clientSocket = socketObj.createSocketforHFTclient(8080);

    if (clientSocket == -1) {
        std::cerr << "Client:  Error connecting to the server : receiveLiveTickData" << std::endl;
        return;
    }
    
    double receivedData;
    while (recv(clientSocket, &receivedData, sizeof(receivedData), 0) > 0) {

        // Update liveTickData
        ticker.setLiveTickData(receivedData);

    }
}


