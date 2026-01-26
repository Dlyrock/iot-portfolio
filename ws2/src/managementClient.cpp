#include <string>
#include <iostream>

/***TODO: complete code as per assignment specification***/

int main(int argc, char* argv[]) {
    if (argc  < 3) {
        std::cerr << "Usage: " << argv[0] << " <socket_client_ip> <socket_server_ip> <json_file> " << std::endl;
        return 1;
    }

    //***IMPORTANT: DO NOT CHANGE THE ORDER, QUANTITY OR TYPE OF THE COMMAND LINE ARGUMENTS*/

    // Get the client IP, eBike ID, CSV file, and number of ports from the command line arguments
    std::string clientIp = argv[1]; // Client socket IP address        
    std::string serverIp = argv[2]; // Server socket IP address
    std::string jsonFile = argv[3]; // JSON file path, normally in the data directory of the project (See examples). JSON file contains the management message to send to the server.

    //TODO: complete code as per assignment specification

    std::cout << "Client IP: " << clientIp << std::endl;
    std::cout << "Server IP: " << serverIp << std::endl;
    std::cout << "JSON message file: " << jsonFile << std::endl;

}