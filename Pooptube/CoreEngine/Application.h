/**
* @author 이선협
* @version 2014/04/26 김지환 ( Tool에서 사용할 수 있게 개조함 )
* @brief
* 프레임워크를 초기화하고 기본적인 기능을 수행하는 객체
* 윈도우 프레임, 렌더러, 씬매니저 생성 및 관리를 수행한다.
* 해당 프레임워크를 사용하기 위해서는 아래 싱글톤객체를 init해주어야 한다.
* @date 2014/04/20
* @file Application.h
*/
#pragma once

#include <windows.h>
#include "Object.h"
#include "D3D9Renderer.h"
#include "SceneManager.h"
//#include "KeyEventDispatcher.h"
//#include "MouseEventDispatcher.h"

namespace pooptube {

	//@brief 
	//D3DXMATRIXA16사용에 대해서 최적화된 메모리 할당을 위한 설정
	//자세한 사항은 위키확인
	const size_t POOPTUBE_ALIGNMENT_SIZE = 16;

	class Application : public Object {
	public:
		static Application* GetInstance();

	public:
		bool Init(wchar_t* title, int width, int height, bool isWindowed);	// for Client
		bool Init(HWND hWnd, int Height, int Width);						// for Tool
		bool Release();
		bool Run(bool isAsync=false);

	public:
		HWND		 GetHWND() const { return mHwnd; }
		HINSTANCE	 GetHandleInstance() const { return mHInstance; }
		VOID		 SetScreenSize(int Height, int Width) { mScreenHeight = Height; mScreenWidth = Width; }
		D3DXVECTOR2	 GetScreenSize() { return D3DXVECTOR2((float)mScreenHeight, (float)mScreenWidth); }

		// TODO: Getter, Setter
		// TODO: Pause - Issue: Network?

		SceneManager*			GetSceneManager() { return mSceneManager; }
		//KeyEventDispatcher*		GetKeyEventDispatcher() { return mKeyEventDispatcher; }
		//MouseEventDispatcher*	GetMouseEventDispatcher() { return mMouseEventDispatcher; }

		float					GetFps() const { return mFps; }
		int						GetCountDPCall() const { return mCountDPCall; }
		void					UpdateDPCall() { ++mCountDPCall; }

	private:
		Application() : mTitle(nullptr), mScreenWidth(0), mScreenHeight(0),
			mFps(0.f), mElapsedTime(0.f), mDeltaTime(0), mIsWindowed(false) {}
		~Application() {}

		bool _CreateWindowFrame( wchar_t* title, int width, int height );
		bool _CreateSceneManager();
		//bool _CreateEventDispatcher();

		bool _UpdateFrame( float& fpsTimer, int& prevTime, int& nowTime, int& frameCount );

		static LRESULT CALLBACK _WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	private:
		static Application* mInstance;

		HWND mHwnd;
		HINSTANCE mHInstance;

		wchar_t* mTitle;
		int mScreenWidth;
		int mScreenHeight;

		float mFps;
		float mElapsedTime;
		float mDeltaTime;

		bool mIsWindowed;

		SceneManager* mSceneManager;

		int mCountDPCall = 0;


		//KeyEventDispatcher* mKeyEventDispatcher;
		//MouseEventDispatcher* mMouseEventDispatcher;

		//std::forward_list<KeyEvent *> mKeyEventList;
		//std::forward_list<MouseEvent *> mMouseEventList;
	};
}