// CoreEngine.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Application.h"
#include "Object.h"
#include "ObjectManager.h"

int main() {
//#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc( );
//#endif
	
// 	pooptube::Application::GetInstance()->Init(L"Test", 400, 300, true);
// 
// 	pooptube::TestScene* TestScene = pooptube::TestScene::Create();
// 
// 	if (TestScene == nullptr)
// 	{
// 		//아마 cone.fbx파일이 없어서 일겁니다. 라인에 문의하세요.
// 		printf("TestScene Create Error!\n");
// 		system("pause");
// 		pooptube::Application::GetInstance()->Release();
// 
// 		return 1;
// 	}
// 
// 	pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( TestScene );
// 	//pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( pooptube::TestScene::Create() );
// 
// 	//pooptube::TestScene::Create();
// 	if ( pooptube::Application::GetInstance()->Run() ) {
// 		pooptube::Application::GetInstance()->Release();
// 	} else {
// 		return 1;
// 	}
// 
// 	return 0;
}

