/**
 * @file ebikeGateway.cpp
 * @brief Main gateway application for eBike IoT system.
 * Module: UFCFVK-15-2 Internet of Things
 */
#include <string>
#include <iostream>
#include <csignal>
#include <atomic>

#include "util/MiscUtils.h"
#include "web/WebServer.h"
#include "comm/SocketServer.h"

namespace ebikeConstants {
    const std::string CONFIG_PATH = "config/server-config.yaml";
}

static std::atomic<bool> g_running{true};

void signalHandler(int) {
    g_running = false;
    std::exit(0);
}

int main() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    try {
        std::string serverIp = readConfigValue(ebikeConstants::CONFIG_PATH, "server", "ip");
        int socketPort = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "server", "port"));
        int mgmtPort = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "server", "management_port"));
        int webPort = std::stoi(readConfigValue(ebikeConstants::CONFIG_PATH, "webserver", "port"));

        Poco::JSON::Array::Ptr ebikes = new Poco::JSON::Array();

        SocketServer socketServer(serverIp, socketPort, mgmtPort, ebikes);
        socketServer.start();

        WebServer webServer(ebikes);
        webServer.start(webPort);

        return 0;
    } catch (const Poco::Exception& ex) {
        std::cerr << "Server error (Poco): " << ex.displayText() << std::endl;
        return 1;
    }
}