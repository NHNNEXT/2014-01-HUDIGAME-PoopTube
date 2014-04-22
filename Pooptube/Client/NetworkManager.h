
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
			// ���� ���� ��Ŷ�� ũ��(recvLen)�� ��Ŷ�� ���� ũ��(mSize)�� �ٸ� ���
			// ���� ���� ��Ŷ�� ũ�⸸ŭ circularBuffer���� �̾Ƴ���.
			// or
			// ��Ŷ�� ���� ũ��(mSize)�� packetType���� �䱸�ϴ� ũ��(sizeof(LoginResult))�� �ٸ� ���
			// ���� ���� ��Ŷ�� ũ�⸸ŭ circularBuffer���� �̾Ƴ���.

			if (header->mSize != recvLen || recvLen != sizeof(LoginResult))
			{
				char* garbage = new char[recvLen];
				circularBuffer->Read((char*)&garbage, recvLen);
				delete[] garbage;

				return;
			}

			// �α��� ��� ó��
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