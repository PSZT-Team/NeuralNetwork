#include "Network.h"

Network::Network (int argc, char* argv[]) : mInterface (argc, argv) {

}

void Network::run () {
    mInterface.printParams();
    mInterface.printHelp ();
    mInterface.printUsage ();

    std::cin.get ();
}