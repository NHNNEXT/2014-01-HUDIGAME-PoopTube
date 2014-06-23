// Client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Application.h"
#include "NetworkManager.h"
#include "Object.h"
#include "ObjectManager.h"
#include "StageOne.h"
#include "IntroScene.h"
#include "EndScene.h"
#include "BugTrapHeader.h"
#include "Stage.h"

int ClientRun()
{
	SetupExceptionHandler();
	BT_SetTerminate(); // set_terminate() must be called from every thread
	BT_InsLogEntry( g_iLogHandle, BTLL_INFO, _T( "Entering ClientRun() function" ) );
	
	//#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(323);
	//#endif

	pooptube::Application::GetInstance()->Init(L"Test", 1024, 768, false);

	// test
	/*pooptube::NetworkManager a;
	a.Init();
	a.Connect();

	LoginRequest lr;
	lr.mPlayerId = 1000;
	pooptube::NetworkSystem::GetInstance()->Write((char*)&lr, lr.mSize);*/

// 	if (pStageOne == nullptr) {
// 		printf("TestScene Create Error!\n");
// 		system("pause");
// 		pooptube::Application::GetInstance()->Release();
// 
// 		return 1;
// 	}

	// json로드 스테이지

 	/*Stage* pStage = Stage::Create( "test.json" );
 	pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( pStage );*/
	
	IntroScene *pIntroScene = IntroScene::Create();
	pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pIntroScene);

	/*EndScene *pEndScene = EndScene::Create();
	pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pEndScene);*/

	//pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( pooptube::TestScene::Create() );

	pooptube::Application::GetInstance()->Run();
	pooptube::Application::GetInstance()->Release();

	BT_InsLogEntry( g_iLogHandle, BTLL_INFO, _T( "Leaving ClientRun() function" ));
	BT_CloseLogFile( g_iLogHandle );

	return 0;
}

