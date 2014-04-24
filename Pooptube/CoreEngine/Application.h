/**
* @author 양현찬
* @brief
* 프레임워크를 초기화하고 기본적인 기능을 수행하는 객체
* 윈도우 프레임, 렌더러, 씬매니저 생성 및 관리를 수행한다.
* 해당 프레임워크를 사용하기 위해서는 아래 싱글톤객체를 init해주어야 한다.
* @date 2014/04/20
* @file Application.h
*/
#pragma once

#include "Object.h"
#include "D3D9Renderer.h"
#include "SceneManager.h"
#include "KeyEventDispatcher.h"
#include "MouseEventDispatcher.h"

namespace pooptube {

	//@brief 
	//D3DXMATRIXA16사용에 대해서 최적화된 메모리 할당을 위한 설정
	//자세한 사항은 위키확인
	const size_t POOPTUBE_ALIGNMENT_SIZE = 16;

	class Application : public Object {
	public:
		static Application* GetInstance();

	public:
		bool Init( wchar_t* title, int width, int height, bool isWindowed );
		bool Release();
		bool Run();

	public:
		inline HWND			GetHWND() const { return mHwnd; }
		inline HINSTANCE	GetHandleInstance() const { return mHInstance; }

		// TODO: Getter, Setter
		// TODO: Pause - Issue: Network?

		SceneManager*			GetSceneManager() { return mSceneManager; }
		KeyEventDispatcher*		GetKeyEventDispatcher() { return mKeyEventDispatcher; }
		MouseEventDispatcher*	GetMouseEventDispatcher() { return mMouseEventDispatcher; }

		float					GetFps() const { return mFps; }

	private:
		Application() : mTitle(nullptr), mScreenWidth(0), mScreenHeight(0),
			mFps(0.f), mElapsedTime(0.f), mDeltaTime(0), mIsWindowed(false) {}
		~Application() {}

		bool _CreateWindowFrame( wchar_t* title, int width, int height );
		bool _CreateSceneManager();
		bool _CreateEventDispatcher();

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
		KeyEventDispatcher* mKeyEventDispatcher;
		MouseEventDispatcher* mMouseEventDispatcher;

		std::forward_list<std::shared_ptr<KeyEvent>> mKeyEventList;
		std::forward_list<std::shared_ptr<MouseEvent>> mMouseEventList;
	};
}