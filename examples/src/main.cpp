
#include <peercpp.h>

using namespace peercpp;

int main (int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <peer_id> <connection_id>" << std::endl;
        return 1;
    }

    Peer peer(argv[1]);

    if (argc == 3) {
        peer.connect(argv[2]);
    }

    peer.onConnect([](Connection conn) {
        std::cout << "Connected to peer: " << conn.getId() << std::endl;

        conn.onData([](std::string data) {
            std::cout << "Received data: " << data << std::endl;
        });

        conn.onOpen([&conn]() {
            std::cout << "Connection opened!" << std::endl;
            conn.send("Hello, world!");
        });

    });

    peer.onDisconnection([](Connection conn) {
        std::cout << "Disconnected from peer: " << conn.getId() << std::endl;
    });


}