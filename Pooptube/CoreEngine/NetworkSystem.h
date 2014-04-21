
#pragma once

#include "Config.h"
#include "PacketHeader.h"
#include "CircularBuffer.h"
#include "BaseHandler.h"
#include <map>

namespace pooptube {
	class NetworkSystem : public Singleton<NetworkSystem> {
	public:
		NetworkSystem();
		~NetworkSystem();

		bool Init();
		void Destroy();

		bool Connect(const char* serverIP, int port);
		void Disconnect();

		void SetPacketHandler(short packetType, BaseHandler* handler);

		void Write(const char* data, size_t size);
		void Read();

	private:
		void ProcessPacket();

	private:
		SOCKET		mSocket;
		SOCKADDR_IN	mServerAddr;

		char*		mServerIP;
		int			mPort;

		std::map<short, BaseHandler*> mPacketHandler;

		CircularBuffer	mRecvBuffer;
		CircularBuffer	mSendBuffer;
	};
}