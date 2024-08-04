#include <rtc/rtc.hpp>

#include <functional>

#pragma once

const rtc::Configuration DEFAULT_CONFIGURATION = {
    .iceServers = {
        rtc::IceServer("stun:stun.l.google.com:19302")
    }
};

namespace peercpp {

    struct PeerOptions {

        std::string host = "0.peerjs.com";
        int port = 443;
        std::string path = "";
        std::string key = "peerjs"; 
        std::optional<std::string> token = std::nullopt;
        bool secure = false;
        double pingInterval = 5000;

    };

    class Peer {

        public:

            Peer();
            ~Peer();

            Peer(PeerOptions options=PeerOptions());
            Peer(std::string id, PeerOptions options = PeerOptions());

            rtc::Configuration& getConfiguration();

            std::string getId();
            std::string getSignalingServerUrl();

            void start();

        private:
            
            rtc::Configuration configuration;
            std::shared_ptr<rtc::WebSocket> signaling_server;

            PeerOptions options;
            std::string id;

            void _obtainId();

    };

}