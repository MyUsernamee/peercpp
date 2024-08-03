#include <rtc/rtc.hpp>

#include <functional>

#pragma once

const rtc::Configuration DEFAULT_CONFIGURATION = {
    .iceServers = {
        rtc::IceServer("stun:stun.l.google.com:19302")
    }
};

namespace peercpp {

    class Connection {

        public:

            Connection();
            ~Connection();
            Connection(std::string our_id, std::string id, std::string signaling_server_url);

            std::string getToId();
            std::string getFromId();
            rtc::DataChannel* getDataChannel();

            void onData(std::function<void(std::string)> callback);
            void onOpen(std::function<void()> callback);
            void onClose(std::function<void()> callback);

            void send(char* data, unsigned int length);

        private:

            std::vector<std::function<void(std::string)>> on_data;
            std::vector<std::function<void()>> on_open;
            std::vector<std::function<void()>> on_close;

            std::string id;
            std::string our_id;
            std::shared_ptr<rtc::WebSocket> signaling_server;
            rtc::DataChannel* data_channel;
            rtc::PeerConnection* peer_connection;

            std::weak_ptr<rtc::WebSocket> signaling_server;

            std::function<void(std::string)> on_data;
            std::function<void()> on_open;
            std::function<void()> on_close;

            void _establishConnection();

    };

    class Peer {

        public:

            Peer();
            ~Peer();

            Peer(std::string id);
            Peer(std::string id, std::string signaling_server_url);

            rtc::Configuration& getConfiguration();
            std::unordered_map<std::string, Connection>& getConnections();

            std::string getId();
            std::string getSignalingServerUrl();

            void connect(std::string id);

            void onConnect(std::function<void(Connection&)> callback);
            void onDisconnect(std::function<void(Connection&)> callback);
            void onData(std::function<void(Connection&, std::string)> callback);

            void broadcast(char* data, unsigned int length);
            void send(std::string id, char* data, unsigned int length);

            void listen();

        private:
            
            rtc::Configuration configuration;
            std::unordered_map<std::string, Connection> connections;

            std::shared_ptr<rtc::WebSocket> signaling_server;

            std::function<void(Connection&)> on_connect;
            std::function<void(Connection&)> on_disconnect;
            std::function<void(Connection&, std::string)> on_data;

            std::string id;
            std::string signaling_server_url;

            void _sendSignalingMessage(nlohmann::json message); // Send message to signaling server ( with no to id )
            void _sendSignalingMessage(std::string id, nlohmann::json message); // Send message to peer through the signaling server
            // The above is used for signaling messages, not data messages
            void _handleSignalingMessage(nlohmann::json message); // Handle signaling message
            void _handleSignalingError(std::string error); // Handle signaling error

            void _register(); // Register with signaling server

            void _establishConnection(std::string id); // Establish connection with peer

            void _sendOffer(std::string id); // Send offer to peer

    };

}