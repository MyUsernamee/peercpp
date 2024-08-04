#include "peercpp.h"
#include <nlohmann/json.hpp>

using namespace peercpp;

Peer::Peer() {
    this->options = PeerOptions();
    this->configuration = DEFAULT_CONFIGURATION;
}

Peer::Peer(std::string id, PeerOptions options) {
    this->id = id;
    this->options = options;
    this->configuration = DEFAULT_CONFIGURATION;
}

Peer::Peer(PeerOptions options) {
    this->options = options;
    this->configuration = DEFAULT_CONFIGURATION;

    _obtainId();
}

void Peer::_obtainId() {
    std::string url = "http://" + this->options.host + ":" + std::to_string(this->options.port) + "/id";
    if (this->options.secure) {
        url = "https://" + this->options.host + ":" + std::to_string(this->options.port) + "/id";
    }

    if (this->options.token.has_value()) {
        url += "?token=" + this->options.token.value();
    }

    auto response = rtc::http::get(url);
    auto json = nlohmann::json::parse(response);

    this->id = json["id"];
}

void Peer::start() {
    this->signaling_server = std::make_shared<rtc::WebSocket>(this->getSignalingServerUrl());
    this->signaling_server->connect();
}