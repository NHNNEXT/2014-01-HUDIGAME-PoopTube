#include "stdafx.h"
#include "NetworkManager.h"

namespace pooptube
{

	NetworkManager::NetworkManager()
	{

	}

	NetworkManager::~NetworkManager()
	{
		
	}

	void NetworkManager::Init()
	{
		NetworkSystem::GetInstance()->Init();
		RegisterPacketHandler();
	}

	void NetworkManager::Destroy()
	{
		
	}


	void NetworkManager::Connect()
	{
		NetworkSystem::GetInstance()->Connect("localhost", 9001);
	}

	void NetworkManager::Disconnect()
	{
		NetworkSystem::GetInstance()->Disconnect();
	}

	void NetworkManager::HandleLoginResult(LoginResult& inPacket)
	{
		LoginResult packet = inPacket;
		NetworkSystem::GetInstance()->GetPacketData((char*)&packet, inPacket.mSize);

		mMyPlayerId = inPacket.mPlayerId;

		printf("PlayerId [%d] Login Success \n", mMyPlayerId);
	}

	void NetworkManager::RegisterPacketHandler()
	{
		LoginResultHandler* loginresulthandler = new LoginResultHandler;
		NetworkSystem::GetInstance()->SetPacketHandler(PKT_SC_LOGIN, loginresulthandler);
	}
}