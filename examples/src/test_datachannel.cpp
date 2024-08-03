

#include <rtc/rtc.hpp>
#include <iostream>

#include <nlohmann/json.hpp>

std::unordered_map<std::string, std::shared_ptr<rtc::PeerConnection>> connections;

rtc::PeerConnection 

int main (int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <peer_id> <connection_id>" << std::endl;
        return 1;
    }

    std::string id = argv[1];
    std::cout << "Peer id: " << id << std::endl;

    // Signaling
    auto ws = std::make_shared<rtc::WebSocket>("ws://localhost:8080"); // Shared pointer because this can be acessed by multiple threads
    // This will be converted to a weak pointer when passed to the lambda

    // Used for waiting for connections
    std::promise<void> promise;
    auto future = promise.get_future();

    ws->onOpen([&promise]() {
        std::cout << "Connected to signaling server!" << std::endl;
        promise.set_value();
    });

    ws->onError([](std::string error) {
        std::cout << "Error: " << error << std::endl;
    });

    ws->onMessage([](auto data) {
        
        if (!std::holds_alternative<std::string>(data)) {
            return;
        }

        nlohmann::json message = nlohmann::json::parse(std::get<std::string>(data));

        auto it = message.find("id");
        if (it == message.end()) {
            return;
        }

        std::string id = it->get<std::string>();

        it = message.find("type");
        if (it == message.end()) {
            return;
        }

        std::string type = it->get<std::string>();

        std::shared_ptr<rtc::PeerConnection> pc;
        if (auto jt = connections.find(id); jt != connections.end()) {
            pc = jt->second; // Connection already exists
        }
        else if (type == "offer") {
            pc = std::make_shared<rtc::PeerConnection>();
            connections[id] = pc;
        }
        else {
            return;
        }

    }); 


}