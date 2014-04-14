// Client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Application.h"
#include "Object.h"
#include "ObjectManager.h"
#include "StageOne.h"

int main()
{
	//#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc( );
	//#endif

	pooptube::Application::GetInstance()->Init(L"Test", 800, 600, true);

	StageOne* pStageOne = StageOne::Create();

	if (pStageOne == nullptr)
	{
		//아마 cone.fbx파일이 없어서 일겁니다. 라인에 문의하세요.
		printf("TestScene Create Error!\n");
		system("pause");
		pooptube::Application::GetInstance()->Release();

		return 1;
	}

	pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pStageOne);
	//pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( pooptube::TestScene::Create() );


	// agebreak : Run 함수를 보면 무조건 True를 반환하는데, 아래 if 문이 필요한가?
	// 2. 만약 Run이 실행되지 않으면, 메모리 릭이 발생할텐데??
	//pooptube::TestScene::Create();
	if (pooptube::Application::GetInstance()->Run()) {
		pooptube::Application::GetInstance()->Release();
	}
	else {
		return 1;
	}

	return 0;
}

