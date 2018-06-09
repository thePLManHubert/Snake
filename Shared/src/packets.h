#pragma once
#include <SFML/Network.hpp>

namespace Datagram {

	typedef unsigned short FunctionID;
	typedef unsigned short ServerPort;
	typedef int PlayerID;

	enum TypeOfPacket { RequestPacket, ReplyPacket, DataPacket };


	struct Request {

		TypeOfPacket type = RequestPacket;
		PlayerID playerID = -1;
		
	};

	struct Reply {

		TypeOfPacket type = ReplyPacket;
		PlayerID playerID;
#ifdef DEBUG
		char testInfo[50] = "Testowa odpowiedz z serwera";
#endif
	};

	struct Data {

		TypeOfPacket type = DataPacket;
		PlayerID playerID;
		char data[80];

	};

}
