// #################################################################
//                 --------   main.cpp   ---------                //
// #################################################################


#include <chrono> 
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

#include "client.h"
#include "server.h"  

class Application {
public:
    Application() = default;

    void run() {
        // Start the server thread
        std::thread server(&Application::serverThread, this);

        // Give some time for the server to start before starting the client
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Start the client thread
        std::thread client(&Application::clientThread, this);

        // Join the threads to wait for them to finish
        client.join();
        server.join();
    }

private:
    void clientThread() {
        // Create a socket
        HFTClient client1;
        client1.start();
    }

    void serverThread() {
        // Server handling single client
        TCPServer tcpServer;
        tcpServer.start();
    }
};

int main() {
    // Create an Application instance and run it
    Application app;
    app.run();

    return 0;
}
