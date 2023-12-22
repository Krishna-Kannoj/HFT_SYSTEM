# HFT System 

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Dependencies](#dependencies)
- [File](#Files_and_Components)
- [Contributing](#contributing)
- [License](#license)

## Introduction
This High-Frequency Trading (HFT) system project aims to create a robust trading infrastructure utilizing multi-threading and network communication. The system involves three main components: the server, client, and various trading strategies.

## Dependencies
 - This project uses C++11 standard.
 - Ensure a compatible C++ compiler (e.g., g++) is installed.

## Files_and_Components
main.cpp
Entry point of the program.
Creates two threads: one for the client and the other for the server.

server.h
Manages the server functionality.
Creates two threads:
Thread 1: Establishes a TCP connection with the client, generates LiveTickData using StockSimulation.h, and sends the data to the client.
Thread 2: Establishes another TCP connection with the client to receive orders.

client.h
Manages the client functionality.
Creates two threads:
Thread 1: Establishes a TCP connection with the server, receives LiveTickData, and stores it using StockClass.h.
Thread 2: Creates objects from allStrategy.h to run various trading strategies.

StockSimulation.h
Simulates stock price using Brownian motion.

allStrategy.h
Creates objects of client data from clientData.h.
Creates a separate thread for each strategy.

StockClass.h
Contains the TickClass class.
Stores LiveTickData and provides functions to access and calculate indicators.

clientData.h
Holds client money.
Provides functions to access client money and execute trades.

dummyStrategy.h
Implements the MACD strategy to execute trades.

## Dependencies
C++11 or later
Standard Template Library (STL)

## Contributing
- Feel free to contribute to this project by forking it and creating a pull request with any improvements or features.

## License
 - This project is licensed under the MIT License.
