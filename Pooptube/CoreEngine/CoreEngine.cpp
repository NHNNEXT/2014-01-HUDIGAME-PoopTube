// CoreEngine.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Application.h"
#include "Object.h"
#include "ObjectManager.h"
#include "TestScene.h"

int main() {
//#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc( );
//#endif
	
	pooptube::Application::GetInstance()->Init(L"Test", 400, 300, true);
	pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( pooptube::TestScene::Create() );
	//pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( pooptube::TestScene::Create() );

	//pooptube::TestScene::Create();
	if ( pooptube::Application::GetInstance()->Run() ) {
		pooptube::Application::GetInstance()->Release();
	} else {
		return 1;
	}

	return 0;
}

