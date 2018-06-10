#pragma once
#include <SFML/Network.hpp>

namespace Datagram {

	typedef unsigned short FunctionID;
	typedef unsigned short ServerPort;
	typedef int PlayerID;

	enum TypeOfPacket { RequestPacket, ReplyPacket, QueuePacket, DataPacket, DCPacket };


	struct Request {

		TypeOfPacket type = RequestPacket;
		PlayerID playerID = -1;
		
	};

	struct Reply {

		TypeOfPacket type = ReplyPacket;
		PlayerID playerID;
		unsigned short newServerPort;
#ifdef DEBUG
		char testInfo[50] = "Testowa odpowiedz z serwera";
#endif
	};

	struct Queue {

		TypeOfPacket type = QueuePacket;
		PlayerID playerID;
		char data[50];

	};

	struct Data {

		TypeOfPacket type = DataPacket;
		PlayerID playerID;
		char data[80];

	};

	struct DC {

		TypeOfPacket type = DCPacket;
		PlayerID playerID;

	};

	TypeOfPacket checkType(void * packet);
}
