#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include "sim/in.h"

namespace sim {

class socket {
public:
    // Constructor matching the standard UDP socket constructor
    socket(int domain, int type, int protocol);
    ~socket();

    // Bind the socket to an IP and port (standard UDP API)
    void bind(const struct ::sockaddr_in& addr);

    // Send data to a specific IP and port (standard UDP API)
    ssize_t sendto(const void* data, size_t size, int flags, const struct ::sockaddr_in& destAddr);

    // Receive data from any source (standard UDP API)
    ssize_t recvfrom(void* buffer, size_t size, int flags, struct ::sockaddr_in& srcAddr);

private:
    int sockfd;
    std::string unixPath;
    std::string uid;

    // Convert IP and port from sockaddr_in to a UNIX socket file path
    std::string ipPortToPath(const struct ::sockaddr_in& addr);

    // Convert a UNIX socket file path back to sockaddr_in
    void pathToIpPort(const std::string& path, struct ::sockaddr_in& addr);

    // Get the IP address of the current thread
    std::string get_ipaddr();

    // Map to store IP addresses for each thread
    static std::unordered_map<std::string, std::string> ip_map;
};

}