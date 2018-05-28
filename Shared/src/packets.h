#pragma once
#include <SFML/Network.hpp>

namespace Datagram {

	typedef unsigned short FunctionID;
	typedef unsigned short ServerPort;
	typedef short PlayerID;

	enum TypeOfPacket { RequestPacket, ReplyPacket, DataPacket };


	struct Request {

		TypeOfPacket type = RequestPacket;
		PlayerID playerID = -1;

	};

	struct Reply {

		TypeOfPacket type = ReplyPacket;
		PlayerID playerID;

	};

	struct Data {

		TypeOfPacket type = DataPacket;
		PlayerID playerID;

	};

	struct State {

		TypeOfPacket type = DataPacket;
		PlayerID playerID;

	};

}
