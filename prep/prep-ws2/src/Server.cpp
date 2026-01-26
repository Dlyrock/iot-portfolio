#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "sim/socket.h" // Include the wrapper class header
#include <csignal>
#include "sim/in.h"
#include "MessageHandler.h" // Include the MessageHandler header


sim::socket* serverSocket = nullptr; // Global pointer to the server socket

// Signal handler function
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nGracefully shutting down..." << std::endl;
        
        if (serverSocket) {
            delete serverSocket;
            serverSocket = nullptr;
        }
        
        std::exit(0); // Exit the program
    }
}



int main() {
    try {

        // Register the signal handler for SIGINT
        std::signal(SIGINT, signalHandler);

        // Set server IP address
        sim::set_ipaddr("192.168.1.1");

       // Create the server socket
       serverSocket = new sim::socket(AF_INET, SOCK_DGRAM, 0);

        // Server binds to a "UDP-like" address
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        inet_pton(AF_INET, "192.168.1.1", &serverAddr.sin_addr); // Bind to localhost
        serverAddr.sin_port = htons(8080); // Port 8080
        serverSocket->bind(serverAddr);

        std::cout << "Server running and waiting for messages..." << std::endl;

        //wait 5 seconds before stating to receive messages
        std::this_thread::sleep_for(std::chrono::seconds(5));

        while (true) {
            // Buffer to receive data
            char buffer[256];
            struct sockaddr_in clientAddr;

            // Receive data from the client
            ssize_t received = serverSocket->recvfrom(buffer, sizeof(buffer), 0, clientAddr);
            if (received > 0) {
                buffer[received] = '\0'; // Null-terminate the received string

                // Print the received message and client address
                char clientIp[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
                uint16_t clientPort = ntohs(clientAddr.sin_port);
               
                MessageHandler _messageHandler = MessageHandler();
                const char* response = _messageHandler.handleMessage(buffer,clientIp,clientPort);

                _messageHandler.sendResponse(serverSocket, response, clientAddr);

            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Server: " << e.what() << std::endl;
    }

    return 0;
}