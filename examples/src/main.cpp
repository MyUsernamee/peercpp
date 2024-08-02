
#include <peercpp.h>

int main (int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <peer_id>" << std::endl;
        return 1;
    }

    Peer peer(argv[1]);
    

}