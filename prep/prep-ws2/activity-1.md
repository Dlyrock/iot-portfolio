# Activity 1: Explore the sockets operations

[<--back](./prep-worksheet2.md)

In this activity you just need to go through the files provided and understand how these work. You will use these files in the next activity.

We have seen there are multiple mechanisms for inter-process and network communication.

UDP Sockets are one of those mechanisms, you might remember the main characteristics of UDP sockets:

* **Connectionless**: UDP sockets do not establish connections; each message is sent independently, making communication lightweight and fast.
* **Unreliable:** No guarantees for delivery, order, or integrity; applications must handle reliability if needed.
* **Low Overhead:** Minimal protocol overhead ensures low latency, ideal for real-time applications like gaming and streaming.
* **Message-Oriented**: Data is sent and received as discrete packets (datagrams) with a size limit of 65,535 bytes.
* **Broadcast/Multicast Support**: Enables sending messages to multiple recipients, useful for service discovery and live streaming.

The standard C++ API for sockets is provided by the library `sys/sockets.h` however, we are not going to use that library, and given that we are all working on the csctcloud we will use a different library that will work on a emulated network. Our sockets will have the same API as the standard UDP sockets but will work as UNIX sockets underneath, so we remove issues related to the network and concurrency of many of us working on the same infrastructure.

Our socket API is available in [`sim/sockets.h`](./src/sim/socket.h).

### Description of `socket.h` Header

The `socket` class in `sim/socket.h` is a custom implementation of a UDP-like socket for a emulated network environment. It provides methods for creating, binding, sending, and receiving data using UNIX domain sockets internally.

### Methods Available

1. **Constructor:**
   ```cpp
   socket(int domain, int type, int protocol);
   ```
   Initializes a new socket object. Only supports `AF_INET`, `SOCK_DGRAM`, and protocol `0`.

   - **AF_INET**: Specifies the address family for IPv4.
   - **SOCK_DGRAM**: Specifies the socket type as datagram, which is used for UDP.
   - **protocol 0**: Indicates the default protocol for the specified address family and socket type, which is UDP for `AF_INET` and `SOCK_DGRAM`.


2. **Destructor:**
   
   ```cpp
   ~socket();
   ```
   Cleans up the socket, including unlinking the UNIX socket file path and closing the socket.

3. **bind:**
   ```cpp
   void bind(const struct ::sockaddr_in& addr);
   ```
   Binds the socket to a specified IP address and port.

4. **sendto:**
   ```cpp
   ssize_t sendto(const void* data, size_t size, int flags, const struct ::sockaddr_in& destAddr);
   ```
   Sends data to a specified IP address and port.

5. **recvfrom:**
   ```cpp
   ssize_t recvfrom(void* buffer, size_t size, int flags, struct ::sockaddr_in& srcAddr);
   ```
   Receives data from any source and fills the provided buffer.

### Private Methods

1. **ipPortToPath:**
   ```cpp
   std::string ipPortToPath(const struct ::sockaddr_in& addr);
   ```
   Converts an IP address and port to a UNIX socket file path.

2. **pathToIpPort:**
   ```cpp
   void pathToIpPort(const std::string& path, struct ::sockaddr_in& addr);
   ```
   Converts a UNIX socket file path back to an IP address and port.

3. **get_ipaddr:**
   ```cpp
   std::string get_ipaddr();
   ```
   Retrieves the IP address of the current thread.

### Private Members

- **sockfd:** `int`
  - The file descriptor for the socket.
- **unixPath:** `std::string`
  - The UNIX socket file path.
- **uid:** `std::string`
  - The user ID.
- **ip_map:** `static std::unordered_map<std::string, std::string>`
  - A static map to store IP addresses for each thread.


### Description of `in.h` Header

The `in.h` header file in the `sim` directory provides definitions and functions for handling IPv4 addresses and ports in a emulated network environment. It includes constants, type definitions, and utility functions that mimic the behaviour of standard network programming interfaces.

### Contents of `in.h`

1. **Constants:**
   - `AF_INET`: Specifies the address family for IPv4.
   - `INET_ADDRSTRLEN`: Maximum length of an IPv4 address string (e.g., "255.255.255.255").
   - `SOCK_DGRAM`: Specifies the socket type as datagram, which is used for UDP.

2. **Type Definitions:**
   - `in_port_t`: Type for port numbers.
   - `in_addr_t`: Type for IPv4 addresses.

3. **Structures:**
   - `in_addr`: Structure for an IPv4 address.
     ```cpp
     struct in_addr {
         in_addr_t s_addr; // IPv4 address in network byte order
     };
     ```
   - `sockaddr_in`: Structure for a socket address (IPv4).
     ```cpp
     struct sockaddr_in {
         uint8_t sin_family; // Address family (AF_INET)
         in_port_t sin_port; // Port number in network byte order
         struct in_addr sin_addr; // IPv4 address
         char sin_zero[8]; // Padding to match the size of sockaddr
     };
     ```

4. **emulated NIC Structure:**
   - `emulatedNIC`: Structure to store the IP address of the emulated NIC.
     ```cpp
     struct emulatedNIC {
         char ip[INET_ADDRSTRLEN]; // Store the IP address of the NIC
     };
     ```

5. **Functions:**
   - `set_ipaddr`: Sets the IP address of the emulated NIC.
     ```cpp
     void set_ipaddr(const char* ip);
     ```
   - `get_ipaddr`: Retrieves the IP address of the emulated NIC.
     ```cpp
     const char* get_ipaddr();
     ```
   - `inet_ntop`: emulated function to convert a binary address to a string.
     ```cpp
     const char* inet_ntop(int af, const void* src, char* dst, socklen_t size);
     ```
   - `inet_pton`: emulated function to convert a string address to a binary address.
     ```cpp
     int inet_pton(int af, const char* src, void* dst);
     ```

You might have notice we have other files (Client.cpp, Server.cpp and MessageHandler.h) we will cover these in the next activity.

[<--back](./prep-worksheet2.md)