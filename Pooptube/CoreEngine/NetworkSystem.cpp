
#include "stdafx.h"
#include "NetworkSystem.h"
#include "Application.h"

// Socket
//#include <WinSock2.h>

namespace pooptube
{
	NetworkSystem* NetworkSystem::m_Instance = nullptr;
	NetworkSystem* NetworkSystem::GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = (NetworkSystem*)_aligned_malloc(sizeof(NetworkSystem), ALIGNMENT_SIZE);
			new (m_Instance)NetworkSystem();
		}
		return m_Instance;
	}

	void NetworkSystem::Release(void)
	{
		if (m_Instance)
		{
			m_Instance->~NetworkSystem();
			_aligned_free(m_Instance);
		}
	}

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

		nResult = WSAAsyncSelect(mSocket, Application::GetInstance()->GetHWND(), WM_SOCKET, (FD_CLOSE | FD_CONNECT | FD_READ | FD_WRITE));
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

	void NetworkSystem::Send()
	{
		int size = mSendBuffer.GetCurrentSize();
		if (size > 0)
		{
			char* data = new char[size];
			mSendBuffer.Peek(data);

			int sent = send(mSocket, data, size, 0);

			/// 다를수 있다
			if (sent != size)
				OutputDebugStringA("sent != request\n");

			mSendBuffer.Consume(sent);

			delete[] data;
		}
	}

	void NetworkSystem::Read()
	{
		char inBuf[4096] = { 0, };

		int recvLen = recv(mSocket, inBuf, 4096, 0);

		if (recvLen < 4) return;

		short size;
		short type;
		memcpy(&size, inBuf, sizeof(size));
		memcpy(&type, inBuf + sizeof(size), sizeof(type));

		if (1 <= type && type <= 2)
		{
			// 패킷 사이즈 체크하고 안맞으면 write 안하도록 하는 부분 추가해야함.
			if (!mRecvBuffer.Write(inBuf, recvLen))
			{
				/// 버퍼 꽉찼다. 
				//assert(false) ;
			}
			else
			{
				ProcessPacket(recvLen);
			}
		}
	}

	void NetworkSystem::ProcessPacket(int recvLen)
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

			mPacketHandler[header.mType]->HandlingPacket(header.mType, &mRecvBuffer, &header, recvLen);
		}
	}
}