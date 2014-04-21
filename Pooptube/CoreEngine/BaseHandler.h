
#pragma once

#include "CircularBuffer.h"
#include "PacketHeader.h"

namespace pooptube {
	class BaseHandler {
	public:
		BaseHandler() {}
		virtual ~BaseHandler() {}

		virtual void HandlingPacket(short packetType, CircularBuffer* circularBuffer, PacketHeader* header) = 0;
	};
}