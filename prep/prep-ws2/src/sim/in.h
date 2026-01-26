#pragma once

#include <cstring>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <sys/socket.h> // For socklen_t

namespace sim {

// Define constants
#define AF_INET 2 // Address family for IPv4
#define INET_ADDRSTRLEN 16 // Maximum length of an IPv4 address string (e.g., "255.255.255.255")
#define SOCK_DGRAM 2 // Datagram socket type

// Define types
typedef uint16_t in_port_t; // Port type
typedef uint32_t in_addr_t; // IPv4 address type

// Structure for an IPv4 address
struct in_addr {
    in_addr_t s_addr; // IPv4 address in network byte order
};

// Structure for a socket address (IPv4)
struct sockaddr_in {
    uint8_t sin_family; // Address family (AF_INET)
    in_port_t sin_port; // Port number in network byte order
    struct in_addr sin_addr; // IPv4 address
    char sin_zero[8]; // Padding to match the size of sockaddr
};

// Simulated NIC structure
struct SimulatedNIC {
    char ip[INET_ADDRSTRLEN]; // Store the IP address of the NIC
};

// Function to set the IP address of the simulated NIC
void set_ipaddr(const char* ip);

// Function to retrieve the IP address of the simulated NIC
const char* get_ipaddr();

// Simulated inet_ntop function to return the IP of the simulated NIC
const char* inet_ntop(int af, const void* src, char* dst, socklen_t size);

// Simulated inet_pton function to convert an IP string to a binary address
int inet_pton(int af, const char* src, void* dst);

}