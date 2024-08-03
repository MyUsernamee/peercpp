#include "peercpp.h"
#include <nlohmann/json.hpp>

using namespace peercpp;

Connection::Connection() {
    this->id = "";
    this->our_id = "";
    this->data_channel = nullptr;
    this->peer_connection = nullptr;
}

Connection::Connection(std::string our_id, std::string id, std::string signaling_server_url) {
    this->id = id;
    this->our_id = our_id;
    this->signaling_server = std::make_shared<rtc::WebSocket>(signaling_server_url);
    this->data_channel = nullptr;
    this->peer_connection = nullptr;


    _establishConnection();

}

Connection::~Connection() {
    delete this->data_channel;
    delete this->peer_connection;
}

