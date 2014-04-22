// Client.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

	

	StageOne* pStageOne = StageOne::Create();

	if (pStageOne == nullptr)
	{
		//�Ƹ� cone.fbx������ ��� �ϰ̴ϴ�. ���ο� �����ϼ���.
		printf("TestScene Create Error!\n");
		system("pause");
		pooptube::Application::GetInstance()->Release();

		return 1;
	}

	pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene(pStageOne);
	//pooptube::Application::GetInstance()->GetSceneManager()->ChangeScene( pooptube::TestScene::Create() );

	// agebreak : Run �Լ��� ���� ������ True�� ��ȯ�ϴµ�, �Ʒ� if ���� �ʿ��Ѱ�?
	// 2. ���� Run�� ������� ������, �޸� ���� �߻����ٵ�??
	//pooptube::TestScene::Create();
	pooptube::Application::GetInstance()->Run();
	pooptube::Application::GetInstance()->Release();

	return 0;
}

