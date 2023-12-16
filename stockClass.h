#pragma once

#include <iostream>
#include <atomic>
#include <vector>
#include <numeric>

class tickClass {
public:
    // Constructor
    tickClass() : liveTickDataIndex(-1) {}

    void setLiveTickData(double data); 

    // Function to get the current liveTickData
    double getLiveTickData();
    
    int getLiveTickDataIndex();


    double liveTickData; // price
    std::atomic<bool> isFree{true};
    // std::mutex dataMutex;  // Mutex for data access
    
    int liveTickDataIndex;
    std::vector<double> receivedData;
    std::vector<std::pair<double, double>> movingAverage;
    int fastMaWindow = 5;
    int slowMaWindow = 10;
    
    void calculateMovingAverage();

    double calculateMA(int windowSize);
};

void tickClass::setLiveTickData(double data) {
    
    // std::lock_guard<std::mutex> lock(dataMutex);
    bool T = true, F = false;
    while (!(isFree.compare_exchange_weak(T, F, std::memory_order_acq_rel))) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    liveTickDataIndex++;
    liveTickData = data;
    receivedData.push_back(data);
    calculateMovingAverage();
    isFree.store(true, std::memory_order_relaxed);
}

double tickClass::getLiveTickData() {
    
    // std::lock_guard<std::mutex> lock(dataMutex);
    bool T = true, F = false;
    while (!(isFree.compare_exchange_weak(T, F, std::memory_order_acq_rel))) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    double currentTickData = liveTickData;
    isFree.store(true, std::memory_order_relaxed);
    return currentTickData;
}

int tickClass::getLiveTickDataIndex() {
    
    // std::lock_guard<std::mutex> lock(dataMutex);
    bool T = true, F = false;
    while (!(isFree.compare_exchange_weak(T, F, std::memory_order_acq_rel))) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    int currentLiveTickDataIndex = liveTickDataIndex;
    isFree.store(true, std::memory_order_relaxed);
    return currentLiveTickDataIndex;
}

void tickClass::calculateMovingAverage() {
    if (receivedData.size() >= slowMaWindow) {
        double fastMa = calculateMA(fastMaWindow);
        double slowMa = calculateMA(slowMaWindow);
        movingAverage.push_back(std::make_pair(fastMa, slowMa));
    }
}

double tickClass::calculateMA(int windowSize) {
    int dataSize = receivedData.size();
    int startIndex = (dataSize > windowSize) ? (dataSize - windowSize) : 0;
    std::vector<double> windowData(receivedData.begin() + startIndex, receivedData.end());
    return std::accumulate(windowData.begin(), windowData.end(), 0.0) / windowSize;
}

