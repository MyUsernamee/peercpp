#include "peercpp.h"
#include <nlohmann/json.hpp>

using namespace peercpp;

Peer::Peer() {

    Peer("");

}

Peer::Peer(std::string id) : id(id) {

    Peer(id, "ws://localhost:8080");

}

Peer::Peer(std::string id, std::string signaling_server_url) : id(id), signaling_server_url(signaling_server_url) {

    
    signaling_server = std::make_shared<rtc::WebSocket>(signaling_server_url);

    std::promise<void> promise;
    auto future = promise.get_future();

    signaling_server->onOpen([&]() {

        #ifdef DEBUG
        std::cout << "Connected to signaling server!" << std::endl;
        #endif

        _register();
        promise.set_value(); // We connected, no need to wait anymore

    });

    signaling_server->onError([&](std::string error) {

        #ifdef DEBUG
        std::cout << "Error: " << error << std::endl;
        #endif

        _handleSignalingError(error);

    });

    signaling_server->onMessage([&](auto data) {

        // Make the data a string if it can be
        if (!std::holds_alternative<std::string>(data))
            return;

        std::string message = std::get<std::string>(data);
        nlohmann::json json_message = nlohmann::json::parse(message);


        _handleSignalingMessage(json_message);


    });

    future.wait();

}

void peercpp::Peer::connect(std::string id)
{

    _establishConnection(id);

}

void peercpp::Peer::_sendSignalingMessage(nlohmann::json message)
{

    message["from"] = id;
    signaling_server->send(message.dump());

}


void peercpp::Peer::_sendSignalingMessage(std::string id, nlohmann::json message)
{

    message["from"] = this->id;
    message["to"] = id;
    signaling_server->send(message.dump());

}

void peercpp::Peer::_establishConnection(std::string id)
{

    if (connections.find(id) != connections.end()) {

        return; // Connection already exists

    }

    _sendOffer(id); // Send a offer to the peer and _handleSignalingMessage will handle the rest

}

void peercpp::Peer::_handleSignalingMessage(nlohmann::json message)
{

    if (message.find("to") == message.end() || message.find("from") == message.end()) {

        _handleSignalingError("Invalid message format");
        return;

    }

    if (message["to"] != id) {

        return; // Not for us

    }

    if (message.find("type") == message.end()) {

        _handleSignalingError("Invalid message format");
        return;

    }

    if (message["type"] == "offer") {

        // We got an offer, we need to send an answer
        // We need to create a connection object and send an answer
        Connection connection = Connection(message["from"]);
        connections[message["from"]] = connection;

        connection.onData([&](std::string data) {

            // We got data from the connection
            // We need to send it to the user
            if (on_data) {

                on_data(connection, data);

            }

        });

        connection.onOpen([&]() {

            // Connection is open
            // We need to send a answer
            // We need to send a answer
            nlohmann::json answer;
            answer["type"] = "answer";
            answer["sdp"] = "answer";
            _sendSignalingMessage(answer);

            if (on_connect) {

                on_connect(connection);

            }

        });

        connection.onClose([&]() {

            // Connection is closed
            // We need to remove the connection
            connections.erase(connection.getId());

            if (on_disconnect) {

                on_disconnect(connection);

            }

        });

    } else if (message["type"] == "answer") {

        // We got an answer
        // We need to set the answer
        if (connections.find(message["from"]) == connections.end()) {

            _handleSignalingError("Invalid connection");
            return;

        }

        connections[message["from"]].setAnswer(message["sdp"]);

    } else if (message["type"] == "candidate") {

        // We got a candidate
        // We need to add the candidate
        if (connections.find(message["from"]) == connections.end()) {

            _handleSignalingError("Invalid connection");
            return;

        }

        connections[message["from"]].addCandidate(message["candidate"]);

    } else {

        _handleSignalingError("Invalid message type");

    }

}