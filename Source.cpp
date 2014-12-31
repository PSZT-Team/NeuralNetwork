#include <iostream>
#include "Network.h"

int main (int argc, char *argv[]) {

    Network network (argc, argv);
    network.run ();

    std::cout << "\nProgram terminated. Press Enter to continue...";
    std::cin.get ();

	return 0;
}