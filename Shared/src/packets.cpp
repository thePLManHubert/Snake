#include "packets.h"

namespace Datagram {

	TypeOfPacket checkType(void * packet) {
		return *(Datagram::TypeOfPacket*)packet;
	}

}