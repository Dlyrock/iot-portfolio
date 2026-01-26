#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H


#include <iostream>
#include <sstream>


#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "sim/socket.h"

class MessageHandler {
    public:

    const char* handleMessage(const char* message, const char* clientIp, uint16_t clientPort) {
        // Handle the message (existing implementation)
        std::cout << "Handling message from " << clientIp << ":" << clientPort << " - " << message << std::endl;

        return "OK";
    }

    void sendResponse(sim::socket* serverSocket, const char* response, const struct sockaddr_in& clientAddr) {
        ssize_t sent = serverSocket->sendto(response, strlen(response), 0, clientAddr);

        // Print the response sent to the client and clientAddr
        char clientIp[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
        uint16_t clientPort = ntohs(clientAddr.sin_port);

        std::cout << "Server: " << response << " sent to client: " << clientIp << ":" << clientPort << std::endl;

        if (sent > 0) {
            std::cout << "Response sent to client: " << response << std::endl;
        }
    }
};

#endif 