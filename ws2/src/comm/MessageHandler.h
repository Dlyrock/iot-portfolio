/**
 * @file MessageHandler.h
 * @brief Handles incoming messages from eBike clients.
 * Module: UFCFVK-15-2 Internet of Things
 */
#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <arpa/inet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include "sim/socket.h"
#include "sim/in.h"

class MessageHandler {
public:
    explicit MessageHandler(Poco::JSON::Array::Ptr& ebikes) : ebikes_(ebikes) {}

    void handleMessage(const std::string& msg, const std::string& clientIp, 
                       Poco::JSON::Array::Ptr& ebikes, sim::socket* sock = nullptr) {
        try {
            Poco::JSON::Parser parser;
            auto result = parser.parse(msg);
            auto json = result.extract<Poco::JSON::Object::Ptr>();

            std::string type = json->getValue<std::string>("type");

            if (type == "DATA") {
                int ebikeId = json->getValue<int>("ebike_id");
                double lat = json->getValue<double>("lat");
                double lon = json->getValue<double>("lon");
                std::string status = json->getValue<std::string>("status");
                std::string timestamp = json->getValue<std::string>("timestamp");

                std::cout << "[MHANDLER] DATA from " << ebikeId
                          << " at " << timestamp
                          << ": lat=" << lat
                          << ", lon=" << lon
                          << " (" << status << ")" << std::endl;

                Poco::JSON::Object::Ptr feature = new Poco::JSON::Object();
                feature->set("type", std::string("Feature"));

                Poco::JSON::Object::Ptr geometry = new Poco::JSON::Object();
                geometry->set("type", std::string("Point"));
                Poco::JSON::Array::Ptr coords = new Poco::JSON::Array();
                coords->add(lon);
                coords->add(lat);
                geometry->set("coordinates", coords);
                feature->set("geometry", geometry);

                Poco::JSON::Object::Ptr properties = new Poco::JSON::Object();
                properties->set("id", ebikeId);
                properties->set("timestamp", timestamp);
                properties->set("status", status);
                feature->set("properties", properties);

                bool found = false;
                for (size_t i = 0; i < ebikes->size(); i++) {
                    auto existing = ebikes->getObject(i);
                    if (existing && existing->getObject("properties") &&
                        existing->getObject("properties")->getValue<int>("id") == ebikeId) {
                        ebikes->set(i, feature);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    ebikes->add(feature);
                }

            } else if (type == "JOIN") {
                int ebikeId = json->getValue<int>("ebike_id");
                std::cout << "[MHANDLER] JOIN from ebike " << ebikeId << std::endl;
                // Store ebike IP
                ebikeIps_[ebikeId] = clientIp;

            } else if (type == "COMMAND") {
                std::string action = json->getValue<std::string>("action");
                std::cout << "[MHANDLER] COMMAND from management client: action=" << action << std::endl;

                // Forward command to each ebike
                if (sock) {
                    auto ids = json->getArray("ebike_ids");
                    for (size_t i = 0; i < ids->size(); i++) {
                        int ebikeId = ids->get(i).convert<int>();
                        if (ebikeIps_.count(ebikeId)) {
                            std::string ebikeIp = ebikeIps_[ebikeId];
                            struct sockaddr_in ebikeAddr;
                            memset(&ebikeAddr, 0, sizeof(ebikeAddr));
                            ebikeAddr.sin_family = AF_INET;
                            inet_pton(AF_INET, ebikeIp.c_str(), &ebikeAddr.sin_addr);
                            ebikeAddr.sin_port = htons(8087);
                            std::ostringstream cmd;
                            cmd << "{\"type\":\"COMMAND\",\"action\":\"" << action << "\","
                                << "\"ebike_id\":" << ebikeId << "}";
                            sock->sendto(cmd.str().c_str(), cmd.str().size(), 0, ebikeAddr);
                        }
                    }
                }

            } else if (type == "COMMACK") {
                int ebikeId = json->getValue<int>("ebike_id");
                std::cout << "[MHANDLER] COMMACK from ebike " << ebikeId << ": success" << std::endl;

            } else if (type == "SETUP") {
                int interval = json->getValue<int>("data_interval");
                std::cout << "[MHANDLER] SETUP: interval updated to " << interval << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "[MHANDLER] Error parsing message: " << e.what() << std::endl;
        }
    }

    void sendResponse(sim::socket* sock, const std::string& response, struct sockaddr_in addr) {
        sock->sendto(response.c_str(), response.size(), 0, addr);
    }

private:
    Poco::JSON::Array::Ptr& ebikes_;
    std::map<int, std::string> ebikeIps_;
};