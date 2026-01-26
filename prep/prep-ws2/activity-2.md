# Activity 2: A Simple UDP Socket

[<--back](./prep-worksheet2.md)

In this activity you will create a simple Client Server application that uses UDP Sockets.

In our application we will have two processes (programs) running separately. For illustration the following sequence show how the application should work. You need to compile and build the ``client`` and ``server`` before this would work. 

*HERE IGNORE WARNINGS you get on building as these are due to the emulated network classes, these won't affect the development of the project*

* The server starts up and listens for request from clients

```bash

./server

Server running and waiting for messages...

```

* A Client connects and sends a message to the server.

```bash

./client 192.168.1.12
Message to server: Hello, Server!

```
Note the client uses its IP as argument for running. This IP looks real but is emulated, however it is expected you use a valid IP as in the format presented.


* The server processes the message

```bash

Handling message from 192.168.1.12:8085 - Hello, Server!

```

1) Creation of the Server:

The server has two files:  [Server.cpp](./src/Server.cpp) and [MessageHandler.h](./src/MessageHandler.h).

The Server.cpp has the following contents:

The include section:

```cpp
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "sim/socket.h" // Include the wrapper class header
#include <csignal>
#include "sim/in.h"
#include "MessageHandler.h" 

```

The includes are standard files needed for working with strings and network address. Note we are also including our emulated Network Interface (sim/in.h), Socket (sim/socket.h) and the MessageHandler.h.

```cpp

sim::socket* serverSocket = nullptr; // Global pointer to the server socket
```

We create a reference called ``serverSocket`` that will enable us to keep track of the socket and destroy it once we finish, at the moment is pointing to ``null`` but we will use it with our socket.

```cpp
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

```

This signalHandler method enables us to gracefully shutdown our server and delete all the resources we might have used. It is important to do this as we don't to have our server hanging up consuming resources if not shut down properly.

This method is the first called inside the main:


```cpp

      // Register the signal handler for SIGINT
        std::signal(SIGINT, signalHandler);

```

As we are working with a emulated network we need to indicate what IP address our server is going to use, for doing that we do: 

```cpp

 // Set server IP address
sim::set_ipaddr("192.168.1.1");

```

And you can define your own IP as long as it has a valid format. You can have a look to how IPv4 are formatted [here](https://en.wikipedia.org/wiki/IPv4) although this address or a similar will be enough.

You need to copy and paste the following blocks, if not present, in the **Server.cpp** file.

```cpp
   // Create the server socket
       serverSocket = new sim::socket(AF_INET, SOCK_DGRAM, 0);

      // Server binds to a "UDP-like" address
      struct sockaddr_in serverAddr;
      memset(&serverAddr, 0, sizeof(serverAddr));
      serverAddr.sin_family = AF_INET;
      inet_pton(AF_INET, "192.168.1.1", &serverAddr.sin_addr); // Bind to localhost
      serverAddr.sin_port = htons(8080); // Port 8080
        serverSocket->bind(serverAddr);

```

This block indicates what type of socket we are creating, remember the definition of the socket constructor in activity 1?. Then we set the address and port in which our socket will be open, the same address that we use for our server and the port should be in the [permitted range](https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers).

That's it, we have our socket ready to start listening for some incoming requests from clients.

To enable or socket to listen, we nee to use the method ``recvfrom`` as follows:


```cpp
ssize_t received = serverSocket->recvfrom(buffer, sizeof(buffer), 0, clientAddr);

```

We have a buffer where the client is pushing the datagrams and we are pulling them from there. We need to check who is sending the messages and we need to control when the message is finished (Null-terminate).

```cpp

if (received > 0) {
      buffer[received] = '\0'; // Null-terminate the received string

      // Print the received message and client address
      char clientIp[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
      uint16_t clientPort = ntohs(clientAddr.sin_port);
               
      MessageHandler _messageHandler = MessageHandler();
      const char* response = _messageHandler.handleMessage(buffer,clientIp,clientPort);

}

```

Note  ``inet_ntop`` and ``ntohs`` functions that convert IP and port numbers respectively from the byte representations. 

The last part is the creation of ``MessageHandler`` that will process the message for us. We could have done this in the server but it is more elegant to have a separate class for that and we can update it without compromising the fundamental server operation.

So we have the method for handling messages and we pass what we collect from the buffer, with some identification of the sender:

```cpp
const char* response = _messageHandler.handleMessage(buffer,clientIp,clientPort);
```

In the ``handleMessage`` method, you just need to print out the message received together with the sender information:

```cpp
std::cout << "Handling message from " << clientIp << ":" << clientPort << " - " << message << std::endl;

return "";

```

For the time being return an empty string, we will change that in the next activity.



1) Creation of the Client

The Client has only one file:  [Client.cpp](./src/Client.cpp).

```cpp

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include "sim/socket.h" // Include the wrapper class header
#include "sim/in.h"

```

Similarly to the server we include our headers and classes.

The following code will enable us to capture the IP passed as argument when running the program.

```cpp
if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <client_ip>" << std::endl;
            return 1;
        }

```

We set our emulated IP address based on the argument processed:

```cpp

// Set client IP address
        const char* clientIp = argv[1];
        sim::set_ipaddr(clientIp);

```

Again we create the socket and bind it with the server address so we can push the message required:

```cpp
// Create the client socket
        sim::socket client(AF_INET, SOCK_DGRAM, 0);

        // Define the server address
        struct sockaddr_in serverAddr;
        memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        inet_pton(AF_INET, "192.168.1.1", &serverAddr.sin_addr); // Server is on localhost
        serverAddr.sin_port = htons(8080); // Port 8080
```

Socket creation is quite similar to the server one.

Now we are ready to send our message, so we will use the ``sendto`` socket method, we need to pass the message and server address.
Finally, we control that message has been sent successfully, remember UDP does not provide guarantees so it is up to the programmer to check:


```cpp
const char* message = "Hello, Server!";
        ssize_t sent = client.sendto(message, strlen(message), 0, serverAddr);
        if (sent > 0) {
            std::cout << "Message to server: " << message << std::endl;
        }
```


Once you have completed copying and pasting, the initial warnings should have disappeared and the server and client are ready to work.

Run the server, then the client in separate terminals. 


[<--back](./prep-worksheet2.md)