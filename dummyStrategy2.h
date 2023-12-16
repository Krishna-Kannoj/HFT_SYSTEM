#pragma once

#include <iostream>
#include "stockClass.h"
// #include "clientData.h"

class dummyStrategy2{
private:
    tickClass& tickData_;
public:

    dummyStrategy2(tickClass& tickData) : tickData_(tickData) {}
    
    void run(){
        
        int clientSocket;
        Strategy& strategy = Strategy::getInstance(2, 10, clientSocket);
            
        // double lastTickData = -1;
        // while (true) {
        //     double currentTickData = tickData_.getLiveTickData();

        //     // Check if the data has been updated since the last iteration
        //     if (currentTickData != lastTickData) {
        //         // std::cout << "Strategy is running with liveTickData: " << currentTickData << std::endl;
        //         strategy.run();
        //         // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //         // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //         // Update the lastTickData
        //         lastTickData = currentTickData;
        //     }
            
        //     // Non-blocking delay or other processing can be added here
        //     // For simplicity, let's use a busy-wait approach
        //     std::this_thread::yield();
        // }
  }  
};