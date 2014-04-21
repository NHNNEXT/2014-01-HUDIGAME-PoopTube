
#include "stdafx.h"
#include "NetworkSystem.h"
#include "Application.h"

namespace pooptube {
	NetworkSystem::NetworkSystem()
		: mServerIP(nullptr), mPort(9001),
		mRecvBuffer(CircularBuffer(1024 * 10)),
		mSendBuffer(CircularBuffer(1024 * 10))
	{
	}

	NetworkSystem::~NetworkSystem()
	{
	}

	bool NetworkSystem::Init()
	{
		WSADATA WsaDat;

		int nResult = WSAStartup(MAKEWORD(2, 2), &WsaDat);
		if (nResult != 0)
			return false;

		mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (mSocket == INVALID_SOCKET)
			return false;

		nResult = WSAAsyncSelect(mSocket, Application::GetInstance()->GetHWND(), WM_SOCKET, (FD_CLOSE | FD_CONNECT));
		if (nResult)
		{
			MessageBox(Application::GetInstance()->GetHWND(), L"WSAAsyncSelect failed", L"Critical Error", MB_ICONERROR);
			SendMessage(Application::GetInstance()->GetHWND(), WM_DESTROY, NULL, NULL);
			return false;
		}

		return true;
	}

	void NetworkSystem::Destroy()
	{
		for (auto& iter = mPacketHandler.begin(); iter != mPacketHandler.end(); iter++)
		{
			SafeDelete(iter->second);
			iter->second = nullptr;
		}
		mPacketHandler.clear();
	}

	bool NetworkSystem::Connect(const char* serverIP, int port)
	{
		// Resolve IP address for hostname
		struct hostent* host;

		if ((host = gethostbyname(serverIP)) == NULL)
			return false;

		// Set up our socket address structure
		SOCKADDR_IN SockAddr;
		SockAddr.sin_port = htons(port);
		SockAddr.sin_family = AF_INET;
		SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

		if (SOCKET_ERROR == connect(mSocket, (LPSOCKADDR)(&SockAddr), sizeof(SockAddr)))
		{
			if (GetLastError() != WSAEWOULDBLOCK)
				return false;
		}

		return true;
	}

	void NetworkSystem::Disconnect()
	{
		closesocket(mSocket);
		WSACleanup();
	}

	void NetworkSystem::SetPacketHandler(short packetType, BaseHandler* handler)
	{
		mPacketHandler[packetType] = handler;
	}

	void NetworkSystem::Write(const char* data, size_t size)
	{
		if (mSendBuffer.Write(data, size))
		{
			PostMessage(Application::GetInstance()->GetHWND(), WM_SOCKET, NULL, FD_WRITE);
		}
	}

	void NetworkSystem::Read()
	{
		char inBuf[4096] = { 0, };

		int recvLen = recv(mSocket, inBuf, 4096, 0);

		if (!mRecvBuffer.Write(inBuf, recvLen))
		{
			/// ¹öÆÛ ²ËÃ¡´Ù. 
			//assert(false) ;
		}
		else
		{
			ProcessPacket();
		}
	}

	void NetworkSystem::ProcessPacket()
	{
		while (true)
		{
			PacketHeader header;

			if (mRecvBuffer.Peek((char*)&header, sizeof(PacketHeader)) == false)
			{
				break;
			}

			if (header.mSize > mRecvBuffer.GetCurrentSize()) /// warning
			{
				break;
			}

			mPacketHandler[header.mType]->HandlingPacket(header.mType, &mRecvBuffer, &header);
		}
	}
}