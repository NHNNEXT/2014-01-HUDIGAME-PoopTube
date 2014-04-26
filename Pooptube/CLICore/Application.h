// CLICore.h

#pragma once
#pragma comment(lib, "CoreEngine")
#include "..\CoreEngine\Application.h"

using namespace System;

namespace Core {

	public ref class Application
	{
	protected:
		pooptube::Application *pInstance = nullptr;

	public :
		Application() : pInstance(pooptube::Application::GetInstance()) { }
		~Application() { pInstance->Release(); }


		bool init(int hWnd, int Height, int Width) { return pInstance->Init((HWND)hWnd, Height, Width); };
		void Run() { pInstance->Run(true); };
		// TODO: 여기에 이 클래스에 대한 메서드를 추가합니다.
	};
}
