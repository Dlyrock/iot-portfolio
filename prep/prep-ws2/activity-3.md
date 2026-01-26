# Activity 2: A Simple UDP Socket

[<--back](./prep-worksheet2.md)

In this activity you will enhance the basic operation of the client-server application. You need to achieve two tasks:

1) Enable the server to reply to the client.

You might recall in activity 2, we did not bind an address to the client. We need to do that similar to what we did with the server:

```cpp
    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    inet_pton(AF_INET, clientIp, &clientAddr.sin_addr);
    clientAddr.sin_port = htons(8085); // Let the OS choose a random port
    client.bind(clientAddr);
```

The final part is to receive data from the socket, exactly as we did with the server, only that we now will use the client socket endpoint. This part is left to you. 


2) Provide support for multiple clients.

As implemented in activity 2, the server only handles one request of one client, how can you make the server to listen indefinitely for incoming client messages?

Once your server is listening you will need to use ``ctl + c`` to shutdown your server.

[<--back](./prep-worksheet2.md)