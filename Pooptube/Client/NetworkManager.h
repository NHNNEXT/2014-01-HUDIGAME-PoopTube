
#include "PacketType.h"
#include "NetworkSystem.h"

#pragma once

namespace pooptube
{

	class LoginResultHandler : public BaseHandler
	{
	public:
		virtual void HandlingPacket(short packetType, CircularBuffer* circularBuffer, PacketHeader* header, int recvLen)
		{
			// 실제 받은 패킷의 크기(recvLen)와 패킷에 적힌 크기(mSize)가 다를 경우
			// 실제 받은 패킷의 크기만큼 circularBuffer에서 뽑아낸다.
			// or
			// 패킷에 적힌 크기(mSize)와 packetType에서 요구하는 크기(sizeof(LoginResult))가 다를 경우
			// 실제 받은 패킷의 크기만큼 circularBuffer에서 뽑아낸다.

			if (header->mSize != recvLen || recvLen != sizeof(LoginResult))
			{
				char* garbage = new char[recvLen];
				circularBuffer->Read((char*)&garbage, recvLen);
				delete[] garbage;

				return;
			}

			// 로그인 결과 처리
			LoginResult packet;
			circularBuffer->Read((char*)&packet, packet.mSize);

			printf("Player's ID : [%d] Login Success \n", packet.mPlayerId);
			printf("Player's Name : [%s] \n", packet.mName);
			printf("Player's X, Y, Z : [%.2lf, %.2lf, %.2lf] \n", packet.mPosX, packet.mPosY, packet.mPosZ);
		}
	};

	class NetworkManager
	{
	public:
		NetworkManager();
		~NetworkManager();

		void Init();
		void Destroy();

		void Connect();
		void Disconnect();

		void HandleLoginResult(LoginResult& inPacket);

		void RegisterPacketHandler();

	private:
		int mMyPlayerId;
	};
}