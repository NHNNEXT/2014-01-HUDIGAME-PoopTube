
#pragma once

/*
 * Application
 * 싱글톤 클래스
 * 윈도우 프레임, 렌더러, 씬매니저 생성 및 관리
 */

#include "Object.h"
#include "D3D9Renderer.h"
#include "SceneManager.h"
#include "KeyEventDispatcher.h"
#include "MouseEventDispatcher.h"

namespace pooptube {

	class Application : public Object {
	public:
		static Application* GetInstance();

	public:
		bool Init( wchar_t* title, int width, int height, bool isWindowed );
		bool Release();
		bool Run();

	public:
		inline HWND GetHWND() const { return mHwnd; }
		inline HINSTANCE GetHandleInstance() const { return mHInstance; }

		// TODO: Getter, Setter
		// TODO: Pause - Issue: Network?

		SceneManager* GetSceneManager() { return mSceneManager; }
		KeyEventDispatcher* GetKeyEventDispatcher() { return mKeyEventDispatcher; }
		MouseEventDispatcher* GetMouseEventDispatcher() { return mMouseEventDispatcher; }

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