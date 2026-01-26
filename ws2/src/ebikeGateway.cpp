#include "util/MiscUtils.h"
#include "web/WebServer.h"
#include <iostream>

/***TODO: complete code as per assignment specification***/
namespace ebikeConstants {
    const std::string CONFIG_PATH = "config/server-config.yaml";
}

int main() {
    // DO NOT MODIFY THIS LINE. Creates a reference to a Poco JSON array to store the ebike objects.
    Poco::JSON::Array::Ptr ebikes = new Poco::JSON::Array();

    //** TODO Adjust & add code as per assignment specification ***/

    try {
        
        // read webport from yaml file. DO NOT MODIFY THIS LINE, but need to update config/server-config.yaml with your allocated port as per specification.
        int port = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "webserver", "port"));

        // Create instances of the server class
        WebServer webServer(ebikes);

        // Start the server 
        webServer.start(port);
    
        return 0;
    } catch (const Poco::Exception& ex) {
        std::cerr << "Server error (Poco): " << ex.displayText() << std::endl;
        return 1;
    }
    return 0;
}



