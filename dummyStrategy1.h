#pragma once

#include <iostream>
#include "stockClass.h"
#include "clientData.h"

class dummyStrategy1{
private:
    tickClass& tickData_;
    int clientSocket;
    int id_1;
    Strategy& strategy;
    int squareOff = -1;
    int openTrade = 1;
public:

    dummyStrategy1(tickClass& tickData, int clientSocket, int id_1, Strategy& strategy) 
    : tickData_(tickData), clientSocket(clientSocket), id_1(id_1), strategy(strategy) {}
    
    void run();
    
    // Execute trade
    void movingAverageCrossOverStart();
    
    // Square-Off current trade
    void watchMarket(double tradePrice, const char* type, double &price) ;
 
};

void dummyStrategy1::run(){
            
    double lastTickData = -1;
    while (true) {
        double currentTickData = tickData_.getLiveTickData();

        // Check if the data has been updated since the last iteration
        if (currentTickData != lastTickData) {
            
            movingAverageCrossOverStart();

            // Update the lastTickData
            lastTickData = currentTickData;
        }
        
        // Non-blocking delay or other processing can be added here
        // For simplicity, let's use a busy-wait approach
        std::this_thread::yield();
    }
}
    

void dummyStrategy1::movingAverageCrossOverStart(){
        
    if (tickData_.liveTickDataIndex >= tickData_.slowMaWindow) {
        
        // Implement the MACD strategy here...
        // For simplicity, a basic MACD crossover strategy is shown

        double fastMA = tickData_.movingAverage[tickData_.liveTickDataIndex].first;
        double slowMA = tickData_.movingAverage[tickData_.liveTickDataIndex].second;

        if (fastMA > slowMA) {
            // Generate BUY order
            double tradePrice = tickData_.getLiveTickData();
            strategy.run(id_1, openTrade, tradePrice);
            
            // std::cout << std::endl<< "Position: BUY  ||  Price: " << tradePrice << std::endl;
            
            // Simulate watching the market for target or stop loss
            watchMarket(tradePrice, "SELL", tradePrice);
        } 
        // else {
        //     // Generate SELL order
        //     double tradePrice = price;
            
        //     std::cout << std::endl<< "Take Position: SELL order at price:  " << tradePrice << std::endl;

        //     // Simulate watching the market for target or stop loss
        //     watchMarket(tradePrice, "SELL", price);
        // }
    }
}


void dummyStrategy1::watchMarket(double tradePrice, const char* type, double &price) {
        
    // Simulate watching the market for target or stop loss
    double targetPrice = tradePrice * 0.99;  // 1% target
    double stopLossPrice = tradePrice * 1.01; // 1% stop loss
    
    // std::cout<<std::endl<<"targetPrice:  "<<targetPrice<<"  ||  stopLossPrice:  "<< stopLossPrice<<std::endl;
    while (tickData_.liveTickData > targetPrice && tickData_.liveTickData < stopLossPrice) {
        // std::cout<<tickData_.getLiveTickData()<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (tickData_.liveTickData <= targetPrice) {
        // Generate SELL order for hitting the target
        
        double tradePrice = tickData_.liveTickData;
        strategy.run(id_1, squareOff, tradePrice);
        // std::cout << std::endl<< "Square Off:  SELL  ||  target price:  " << tradePrice << std::endl;
        // std::cout<<"\n-----------------------------------------------------------\n\n";
    } else {
        // Generate SELL order for hitting the stop loss
        
        double tradePrice = tickData_.liveTickData;
        strategy.run(id_1, squareOff, tradePrice);
        // std::cout << std::endl<< "Square Off:  SELL  ||  stop loss price:  " << tradePrice << std::endl;
        // std::cout<<"\n-----------------------------------------------------------\n\n";
    }

}

