# UFCFVK-15-2 Internet of Things - Worksheet 2 Journal

## 2026-03-10

Today I started working on Activity 2.1 by understanding how UDP socket communication works in the simulated network environment. I studied the prep worksheet 2 examples (Server.cpp and Client.cpp) to understand how to use the sim::socket API. The main challenge was understanding that sim::socket uses ::sockaddr_in internally, not sim::sockaddr_in, which caused several compilation errors. I solved this by using standard arpa/inet.h types throughout and only using sim:: namespace for the socket object and set_ipaddr function. I also implemented the MessageHandler class to parse incoming JSON messages and convert them to GeoJSON format for the web server.

*Reflection: I learned to read the API headers carefully before writing code. Initially I assumed sim::sockaddr_in would work everywhere, but checking the socket.h header showed the actual parameter types expected.*

## 2026-03-17

Today I completed Activity 2.1 by connecting the eBikeClient to the eBikeGateway via UDP sockets. The client now sends JOIN and DATA messages in JSON format, and the gateway parses them and updates the shared eBikes list. I encountered issues with the GeoJSON object structure — the web server expected a specific format with type "Feature", geometry and properties fields. I referenced the assignment specification carefully to get the correct structure. I also added acknowledgement messages from the server back to the client. Resources used: Poco JSON documentation, assignment specification.

## 2026-03-24

Today I completed Activity 2.2 by adding a management port (8085) to the gateway and implementing the managementClient. The gateway now runs two socket servers concurrently — one for eBike data on port 8080 and one for management commands on port 8085. I used std::thread to run both servers simultaneously. The main challenge was ensuring thread safety when accessing the shared ebikes list. I also implemented COMMAND and SETUP message handling in the MessageHandler. Testing confirmed that lock commands and setup messages are correctly received and processed by the gateway.