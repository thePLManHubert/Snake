#include "Server.h"
#include <iostream>

int main(void) {

	Server server(5000, 5050);
	server.start();
	
	std::cin.get();

	server.shutdown();
}