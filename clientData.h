#include <iostream>
#include <mutex>
#include <future>
#include <unistd.h>
#include <arpa/inet.h>

class TradingData {
public:
    static TradingData& getInstance(int initialMoney = 1000) {
        static TradingData instance(initialMoney);
        return instance;
    }

    int getInitialMoney() const {
        return initialMoney;
    }

    std::atomic<int> flag;
    double initialMoney;

private:
    TradingData(int initialMoney) : initialMoney(initialMoney), flag(0) {}
};

class TradingSystem {
public:
    static TradingSystem& getInstance(TradingData& tradingData, int strategySocket) {
        static TradingSystem instance(tradingData, strategySocket);
        return instance;
    }

    void executeTrade(int tradeSignal, std::promise<void>& promise, int type, double tradePrice) ;

private:
    TradingSystem(TradingData& tradingData, int strategySocket) : tradingData(tradingData),strategySocket(strategySocket) {}
    TradingData& tradingData;
    int strategySocket;
    int quantity;
};

class Strategy {
public:
    static Strategy& getInstance(int strategyId, int initialMoney, int strategySocket) {
        static Strategy instance(strategyId, initialMoney, strategySocket);
        return instance;
    }

    void run(int strategyId, int type, double tradePrice); 

private:
    Strategy(int strategyId, int initialMoney, int strategySocket) 
    : strategyId(strategyId), tradingData(TradingData::getInstance(initialMoney)), tradingSystem(TradingSystem::getInstance(tradingData, strategySocket)) {}

    int strategyId;
    TradingData& tradingData;
    TradingSystem& tradingSystem;
};

void TradingSystem::executeTrade(int tradeSignal, std::promise<void>& promise, int type, double tradePrice) {
    
    
    std::string tradePriceString = std::to_string(tradePrice);
    
    if(type == -1){
        
        std::string quantityString = std::to_string(quantity);
        std::string myOrder = "SELL AAPL " + tradePriceString + " " + quantityString;
        send(strategySocket, myOrder.c_str(), sizeof(myOrder), 0);
        
        char buffer[1024];
        recv(strategySocket, buffer, sizeof(buffer), 0);
        buffer[sizeof(buffer)] = '\0';
        // std::cout<<std::endl<<buffer<<std::endl;
        
        tradingData.initialMoney += (quantity*tradePrice);
        // std::cout << tradeSignal << " : Square_Off : Successful  :  " << (quantity*tradePrice) << std::endl;
        std::cout << tradeSignal << " : Square_Off : Successful  :  " << tradingData.initialMoney << std::endl;
        std::cout<<"\n-----------------------------------------------------------\n\n";
    }
    else{
        
        // if (tradingData.getInitialMoney() < (quantity*tradePrice)) {
            // std::cout << tradeSignal << " : Enter : Failed  :  " << (quantity*tradePrice) << std::endl;
        // }
        quantity = (tradingData.initialMoney/3)/tradePrice;
        std::string quantityString = std::to_string(quantity);
        std::string myOrder = "BUY AAPL " + tradePriceString + " " + quantityString;
        send(strategySocket, myOrder.c_str(), sizeof(myOrder), 0);
        char buffer[1024];
        recv(strategySocket, buffer, sizeof(buffer), 0);
        buffer[sizeof(buffer)] = '\0';
        // std::cout<<std::endl<<buffer<<std::endl;
        
        tradingData.initialMoney -= (quantity*tradePrice);
        // std::cout << tradeSignal << " : Enter : Successful  :  " << (quantity*tradePrice) << std::endl;
        std::cout << tradeSignal << " : BUY : Successful  :  " << tradingData.initialMoney << std::endl;
        
    }
    promise.set_value();
}

void Strategy::run(int strategyId, int type, double tradePrice) {
    // type: 1(openTrade : BUY), -1(squareOff : SELL)
    int expected = 0;
    std::promise<void> promise;

    while (!(tradingData.flag.compare_exchange_strong(expected, strategyId, std::memory_order_acq_rel))) {}
    tradingSystem.executeTrade(strategyId, promise, type, tradePrice);

    promise.get_future().wait();
    tradingData.flag.store(0);
}
