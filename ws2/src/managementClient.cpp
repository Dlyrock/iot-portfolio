/**
 * @file managementClient.cpp
 * @brief Management client for sending commands to eBike gateway.
 * Module: UFCFVK-15-2 Internet of Things
 */
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>

#include "sim/socket.h"
#include "sim/in.h"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <client_ip> <server_ip> <json_file>\n";
        return 1;
    }

    std::string clientIp = argv[1];
    std::string serverIp = argv[2];
    std::string jsonFile = argv[3];

    // Read JSON file
    std::ifstream file(jsonFile);
    if (!file.is_open()) {
        std::cerr << "[MCLIENT] Failed to open JSON file: " << jsonFile << std::endl;
        return 1;
    }
    std::string message((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();

    std::cout << "[MCLIENT] Message: " << message << std::endl;

    // Setup socket
    sim::set_ipaddr(clientIp.c_str());
    sim::socket clientSocket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, clientIp.c_str(), &clientAddr.sin_addr);
    clientAddr.sin_port = htons(8086);
    clientSocket.bind(clientAddr);

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);
    serverAddr.sin_port = htons(8085);

    // Send message
    clientSocket.sendto(message.c_str(), message.size(), 0, serverAddr);

    // Receive ACK
    char buffer[256];
    struct sockaddr_in fromAddr;
    memset(&fromAddr, 0, sizeof(fromAddr));
    ssize_t received = clientSocket.recvfrom(buffer, sizeof(buffer) - 1, 0, fromAddr);
    if (received > 0) {
        buffer[received] = '\0';
        std::cout << "[MCLIENT] Operation completed successfully" << std::endl;
    }

    return 0;
}