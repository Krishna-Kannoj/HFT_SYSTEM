// // // #################################################################

// // //          --------       strategy.h      ---------             //

// // // #################################################################


#pragma once

#include <iostream>
#include "stockClass.h"
#include "dummyStrategy1.h"
#include "dummyStrategy2.h"

#ifndef LINE_H
 #define LINE_H
#include "SocketCreation.h"
#endif


class allStrategy {
public:
    // Constructor
    allStrategy(tickClass& tickData) : tickData_(tickData) {
        // Create a socket for the strategy
        createSocket socketObj;
        strategySocket_ = socketObj.createSocketforHFTclient(8081);

        if (strategySocket_ == -1) {
            std::cerr << "Client: Error connecting to the server" << std::endl;
            // You may want to handle this error in a better way, such as throwing an exception.
        }
    }

    // Function to run different strategies
    void runStrategy();

    // Destructor to close the strategy socket
    ~allStrategy() {
        close(strategySocket_);
    }

private:
    tickClass& tickData_;
    int strategySocket_;
};

void allStrategy::runStrategy() {
    Strategy& strategy = Strategy::getInstance(1, 10000, strategySocket_);
    int id_1 = 1;
    dummyStrategy1 obj1(tickData_, strategySocket_, id_1, strategy);

    std::thread strategyThread1(&dummyStrategy1::run, &obj1);
    strategyThread1.join();
}



