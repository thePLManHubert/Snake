#pragma once
#include <SFML/Network.hpp>

namespace Datagram {

	typedef unsigned short FunctionID;
	typedef unsigned short ServerPort;
	typedef int PlayerID;

	enum TypeOfPacket { 
		RequestPacket, 
		ReplyPacket, 
		StartPacket, 
		QuitPacket, 
		QueuePacket, 
		DataPacket, 
		DCPacket, 
		SyncPacket, 
		DirPacket
	};
	enum Direction { UP, DOWN, LEFT, RIGHT, STOP, FREEZE };

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

	struct Start {

		TypeOfPacket type = StartPacket;
		int id;
		int id1;
		int id2;
		unsigned short position1;
		unsigned short position2;
		unsigned short fruit;
		unsigned short gamePort;

	};

	struct Quit {

		TypeOfPacket type = QuitPacket;

	};

	struct Queue {

		TypeOfPacket type = QueuePacket;
		PlayerID playerID;
		char data[50];

	};

	struct Data {

		TypeOfPacket type = DataPacket;
		PlayerID playerID;
		Direction direction;
		unsigned short fruit;
		unsigned short score;
		unsigned short position[20];

	};

	struct DC {

		TypeOfPacket type = DCPacket;
		PlayerID playerID;

	};

	struct Sync {

		TypeOfPacket type = SyncPacket;

	};

	struct Dir {

		TypeOfPacket type = DirPacket;
		PlayerID playerID;
		Direction direction;

	};

	TypeOfPacket checkType(void * packet);
}
