/**
 * @file SocketServer.h
 * @brief UDP Socket server for receiving eBike data.
 * Module: UFCFVK-15-2 Internet of Things
 */
#pragma once
#include <string>
#include <iostream>
#include <cstring>
#include <thread>
#include <atomic>
#include <arpa/inet.h>
#include "sim/socket.h"
#include "sim/in.h"
#include "comm/MessageHandler.h"

class SocketServer {
public:
    SocketServer(const std::string& ip, int port, int mgmtPort, Poco::JSON::Array::Ptr& ebikes)
        : ip_(ip), port_(port), mgmtPort_(mgmtPort), ebikes_(ebikes), running_(false) {}

    void start() {
        running_ = true;
        thread_ = std::thread(&SocketServer::run, this);
        thread_.detach();
        mgmtThread_ = std::thread(&SocketServer::runMgmt, this);
        mgmtThread_.detach();
    }

    void stop() { running_ = false; }

private:
    void run() {
        try {
            sim::set_ipaddr(ip_.c_str());
            sim::socket serverSocket(AF_INET, SOCK_DGRAM, 0);

            struct sockaddr_in serverAddr;
            memset(&serverAddr, 0, sizeof(serverAddr));
            serverAddr.sin_family = AF_INET;
            inet_pton(AF_INET, ip_.c_str(), &serverAddr.sin_addr);
            serverAddr.sin_port = htons(port_);
            serverSocket.bind(serverAddr);

            std::cout << "[SOCKETS] " << ip_ << " Socket server started on:" << port_
                      << ". Management port: " << mgmtPort_ << std::endl;

            MessageHandler handler(ebikes_);

            while (running_) {
                char buffer[1024];
                struct sockaddr_in clientAddr;
                memset(&clientAddr, 0, sizeof(clientAddr));
                ssize_t received = serverSocket.recvfrom(buffer, sizeof(buffer) - 1, 0, clientAddr);
                if (received > 0) {
                    buffer[received] = '\0';
                    char clientIp[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
                    handler.handleMessage(std::string(buffer), std::string(clientIp), ebikes_);
                    std::string ack = "{\"status\":\"ok\"}";
                    serverSocket.sendto(ack.c_str(), ack.size(), 0, clientAddr);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[SOCKETS] Error: " << e.what() << std::endl;
        }
    }

    void runMgmt() {
        try {
            sim::socket mgmtSocket(AF_INET, SOCK_DGRAM, 0);

            struct sockaddr_in mgmtAddr;
            memset(&mgmtAddr, 0, sizeof(mgmtAddr));
            mgmtAddr.sin_family = AF_INET;
            inet_pton(AF_INET, ip_.c_str(), &mgmtAddr.sin_addr);
            mgmtAddr.sin_port = htons(mgmtPort_);
            mgmtSocket.bind(mgmtAddr);

            MessageHandler handler(ebikes_);

            while (running_) {
                char buffer[1024];
                struct sockaddr_in clientAddr;
                memset(&clientAddr, 0, sizeof(clientAddr));
                ssize_t received = mgmtSocket.recvfrom(buffer, sizeof(buffer) - 1, 0, clientAddr);
                if (received > 0) {
                    buffer[received] = '\0';
                    char clientIp[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIp, sizeof(clientIp));
                    handler.handleMessage(std::string(buffer), std::string(clientIp), ebikes_);
                    std::string ack = "{\"status\":\"ok\"}";
                    mgmtSocket.sendto(ack.c_str(), ack.size(), 0, clientAddr);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[SOCKETS] Mgmt error: " << e.what() << std::endl;
        }
    }

    std::string ip_;
    int port_;
    int mgmtPort_;
    Poco::JSON::Array::Ptr& ebikes_;
    std::atomic<bool> running_;
    std::thread thread_;
    std::thread mgmtThread_;
};