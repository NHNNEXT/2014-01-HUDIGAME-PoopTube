// Client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Application.h"
#include "NetworkManager.h"
#include "Object.h"
#include "ObjectManager.h"
#include "StageOne.h"
#include "IntroScene.h"
#include "BugTrapHeader.h"

int ClientRun()
{
	SetupExceptionHandler();
	BT_SetTerminate(); // set_terminate() must be called from every thread
	BT_InsLogEntry( g_iLogHandle, BTLL_INFO, _T( "Entering ClientRun() function" ) );
	
	//#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(1154);
	//#endif

	pooptube::Application::GetInstance()->Init(L"Test", 1024, 768, true);

	// test
	/*pooptube::NetworkManager a;
	a.Init();
	a.Connect();

	LoginRequest lr;
	lr.mPlayerId = 1000;
	pooptube::NetworkSystem::GetInstance()->Write((char*)&lr, lr.mSize);*/

	//StageOne* pStageOne = StageOne::Create();
	IntroScene *pIntroScene = IntroScene::Create();

// 	if (pStageOne == nullptr) {
// 		printf("TestScene Create Error!\n");
// 		system("pause");
// 		pooptube::Application::GetInstance()->Release();
// 
// 		return 1;
// 	}

	
	//pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pStageOne);
	pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pIntroScene);
	//pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( pooptube::TestScene::Create() );

	pooptube::Application::GetInstance()->Run();
	pooptube::Application::GetInstance()->Release();

	BT_InsLogEntry( g_iLogHandle, BTLL_INFO, _T( "Leaving ClientRun() function" ) );
	BT_CloseLogFile( g_iLogHandle );

	return 0;
}

