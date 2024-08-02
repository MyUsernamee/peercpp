#include <rtc/rtc.hpp>

class Peer {

    public:
        Peer();
        ~Peer();

        Peer(char* id);
        Peer(char* id, char* signaling_server_url);

        void createOffer();
        void createAnswer();
        void setRemoteDescription(char* sdp);
        void setLocalDescription(char* sdp);
        void addIceCandidate(char* candidate);
        void send(char* message);
        void close();

        rtc::DataChannel& getDataChannel();
        rtc::PeerConnection& getPeerConnection();
        rtc::Configuration& getConfiguration();

    private:
        
        rtc::Configuration configuration;
        rtc::PeerConnection* peer_connection;
        rtc::DataChannel* data_channel;


};