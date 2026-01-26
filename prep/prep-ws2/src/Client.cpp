#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "sim/socket.h" // Include the wrapper class header
#include "sim/in.h" // Include the IP address setting function

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <client_ip>" << std::endl;
            return 1;
        }

        // Set client IP address
        const char* clientIp = argv[1];
        sim::set_ipaddr(clientIp);

        // Create the client socket
        sim::socket client(AF_INET, SOCK_DGRAM, 0);

        // Define the server address
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        inet_pton(AF_INET, "192.168.1.1", &serverAddr.sin_addr); // Server is on localhost
        serverAddr.sin_port = htons(8080); // Port 8080

         // Bind the client socket to the client address
        struct sockaddr_in clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        clientAddr.sin_family = AF_INET;
        inet_pton(AF_INET, clientIp, &clientAddr.sin_addr);
        clientAddr.sin_port = htons(8085); // Let the OS choose a random port
        client.bind(clientAddr);

        // Message to send
        const char* message = "Hello, Server!";
        ssize_t sent = client.sendto(message, strlen(message), 0, serverAddr);
        if (sent > 0) {
            std::cout << "Message to server: " << message << std::endl;
        }

        // Buffer to receive response
        char buffer[256];
        struct sockaddr_in fromAddr;
        
        // Receive response from the server
        ssize_t received = client.recvfrom(buffer, sizeof(buffer), 0, fromAddr);

        //wait 5 seconds before closing
        std::this_thread::sleep_for(std::chrono::seconds(5));

        if (received > 0) {
            buffer[received] = '\0'; // Null-terminate the received string

            // Print the received response
            char fromIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &fromAddr.sin_addr, fromIp, sizeof(fromIp));
            uint16_t fromPort = ntohs(fromAddr.sin_port);

            std::cout << "Received response: " << buffer << std::endl;
            std::cout << "From: " << fromIp << ":" << fromPort << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Client: " << e.what() << std::endl;
    }

    return 0;
}