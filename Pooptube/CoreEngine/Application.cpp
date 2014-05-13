
#include "stdafx.h"
#include "Application.h"
#include "NetworkSystem.h"
#include "ObjectManager.h"
#include "InputManager.h"

namespace pooptube {

	// singletone
	Application* Application::mInstance = nullptr;
	Application* Application::GetInstance() {
		if ( mInstance == nullptr ) {
			mInstance = new Application();
			ObjectManager::GetInstance()->AddObject( mInstance );
		}

		return mInstance;
	}

	// public
	bool Application::Init( wchar_t* title, int width, int height, bool isWindowed ) {
		mTitle = title;
		mScreenWidth = width;
		mScreenHeight = height;
		mIsWindowed = isWindowed;

		if ( _CreateWindowFrame( title, width, height ) == false ) {
			return false;
		}

		if ( _CreateSceneManager() == false ) {
			return false;
		}

		if ( _CreateEventDispatcher() == false ) {
			return false;
		}

		return true;
	}

	bool Application::Init(HWND hWnd, int Height, int Width)
	{
//		mTitle = L"";
		mScreenWidth = Width;
		mScreenHeight = Height;
		mIsWindowed = true;

		mHwnd = hWnd;
// 		if (_CreateWindowFrame(title, width, height) == false) {
// 			return false;
// 		}

		if (_CreateSceneManager() == false) {
			return false;
		}

		if (_CreateEventDispatcher() == false) {
			return false;
		}

		return true;
	}

	bool Application::Release() {
		//mRenderer->Release();
		
// 		for (auto& object : mKeyEventList) {
// 			delete object;
// 		}
// 		mKeyEventList.clear();
// 
// 		for (auto& object : mMouseEventList) {
// 			delete object;	
// 		}
// 		mMouseEventList.clear();
		
		ObjectManager::ReleaseInstance();

		return true;
	}
	bool Application::Run(bool isAsync) {
		// 프레임 계산에 쓰일 변수들
		float fpsTimer = 0.f;
		int prevTime = 0;
		int nowTime = 0;
		int frameCount = 0;

		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );

		do {
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
				if ( msg.message == WM_QUIT )
					return true;

				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else {
				_UpdateFrame( fpsTimer, prevTime, nowTime, frameCount );
				
				if ( GetKeyState(VK_ESCAPE) & 0x8000 ) {
					//mRenderer->ToggleFullscreen();
					break;
				}

				mKeyEventDispatcher->Dispatch( mKeyEventList );
				mMouseEventDispatcher->Dispatch( mMouseEventList );

				//gInputManager.GetKey();

				mSceneManager->Update( mDeltaTime );
				mSceneManager->Render();
			}
		} while (isAsync == false);

		return true;
	}


	// private
	bool Application::_CreateWindowFrame( wchar_t* title, int width, int height ) {
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = _WndProc;
		wcex.cbClsExtra = NULL;
		wcex.cbWndExtra = NULL;
		wcex.hInstance = mHInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = title;
		wcex.hIconSm = NULL;
		wcex.hIcon = NULL;

		RegisterClassEx( &wcex );

		DWORD style = WS_OVERLAPPEDWINDOW; // Fullscreen일때는 WS_POPUP으로 하던데 그 이유는?

		RECT wr = {0, 0, width, height};
		AdjustWindowRect( &wr, style, FALSE );

		mHwnd = CreateWindow( title, title, style, CW_USEDEFAULT, CW_USEDEFAULT,
			wr.right-wr.left, wr.bottom-wr.top, NULL, NULL, mHInstance, NULL);

		ShowWindow( mHwnd, SW_SHOWNORMAL );

		return true;
	}
	
	bool Application::_CreateSceneManager() {
		mSceneManager = new SceneManager();

		if ( mSceneManager->Init( mHwnd, mScreenWidth, mScreenHeight, mIsWindowed ) == true ) {
			ObjectManager::GetInstance()->AddObject( mSceneManager );
		} else {
			delete mSceneManager;
			mSceneManager = nullptr;
			return false;
		}

		return true;
	}

	bool Application::_CreateEventDispatcher() {
		// Key
		mKeyEventDispatcher = new KeyEventDispatcher();

		if ( mKeyEventDispatcher->Init() == true ) {
			ObjectManager::GetInstance()->AddObject( mKeyEventDispatcher );
		} else {
			delete mKeyEventDispatcher;
			mKeyEventDispatcher = nullptr;
			return false;
		}

		mMouseEventDispatcher = new MouseEventDispatcher();

		if ( mMouseEventDispatcher->Init() == true ) {
			ObjectManager::GetInstance()->AddObject( mMouseEventDispatcher );
		} else {
			delete mMouseEventDispatcher;
			mMouseEventDispatcher = nullptr;
			return false;
		}

		return true;
	}

	bool Application::_UpdateFrame( float& fpsTimer, int& prevTime, int& nowTime, int& frameCount ) {
		frameCount++;
		nowTime = timeGetTime(); // 후에 std::chrono 수정하여 개선하면 안됨 -과부하가 큼
		//gettickcount로 바꿀예정
		if ( prevTime == 0.f )
			prevTime = nowTime;

		mDeltaTime = (static_cast<float>(nowTime - prevTime)) / 1000.f;
		mElapsedTime += mDeltaTime;
		fpsTimer += mDeltaTime;
		if( fpsTimer > 0.1f ) {
			mFps = ((float)frameCount) / fpsTimer;
			frameCount = 0;
			fpsTimer = 0.f;
		}
		prevTime = nowTime;

		return true;
	}

	// winproc
	LRESULT CALLBACK Application::_WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch ( message ) {
		case WM_CREATE: {
				break;
			}
		case WM_DESTROY: {
				PostQuitMessage(0);
				break;
			}

		case WM_SOCKET:
		{
			if (WSAGETSELECTERROR(lParam))
			{
				MessageBox(hWnd, L"WSAGETSELECTERROR", L"Error", MB_OK | MB_ICONERROR);
				SendMessage(hWnd, WM_DESTROY, NULL, NULL);
				break;
			}

			switch (WSAGETSELECTEVENT(lParam))
			{
			case FD_CONNECT:
			{
				NetworkSystem::GetInstance();
				break;
			}

			case FD_READ:
			{
				NetworkSystem::GetInstance()->Read();
				break;
			}

			case FD_WRITE:
			{
				/// 실제로 버퍼에 있는것들 꺼내서 보내기
				NetworkSystem::GetInstance()->Send();
			}
				break;

			case FD_CLOSE:
			{
				MessageBox(hWnd, L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION | MB_OK);
				NetworkSystem::GetInstance()->Disconnect();
				SendMessage(hWnd, WM_DESTROY, NULL, NULL);
				break;
			}
			}
			break; // WM_SOCKET end;
		}
		case WM_PAINT: {
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint( hWnd, &ps );
				EndPaint( hWnd, &ps );
				break;
			}
		case WM_MOUSEMOVE: {
				MouseEvent* pMouseEvent = new MouseEvent();
				pMouseEvent->SetMouseEventType( MouseEventType::MOUSE_MOVE );
				pMouseEvent->SetX( (int)(short)LOWORD(lParam) );
				pMouseEvent->SetY( (int)(short)HIWORD(lParam) );

				GetInstance()->mMouseEventList.push_front( pMouseEvent );
				break;
			}
		case WM_LBUTTONDOWN:
			{
				if ( GetInstance()->mMouseEventDispatcher->IsMouseButtonPressed( MOUSE_LBUTTON ) == false ) {
					MouseEvent* pMouseEvent = new MouseEvent();
					pMouseEvent->SetMouseEventType( MouseEventType::MOUSE_LBUTTON_DOWN );
					pMouseEvent->SetMouseButton( MOUSE_LBUTTON );
					pMouseEvent->SetX( (int)(short)LOWORD(lParam) );
					pMouseEvent->SetY( (int)(short)HIWORD(lParam) );

					GetInstance()->mMouseEventDispatcher->SetMouseButtonState( MOUSE_LBUTTON, true );
				
					GetInstance()->mMouseEventList.push_front( pMouseEvent );
				} 
				break;
			}
		case WM_LBUTTONUP:
			{
				if ( GetInstance()->mMouseEventDispatcher->IsMouseButtonPressed( MOUSE_LBUTTON ) == true ) {
					GetInstance()->mMouseEventDispatcher->SetMouseButtonState( MOUSE_LBUTTON, false );
				}
				break;
			}
		case WM_MBUTTONDOWN:
			{
				if ( GetInstance()->mMouseEventDispatcher->IsMouseButtonPressed( MOUSE_MBUTTON ) == false ) {
					MouseEvent* pMouseEvent = new MouseEvent();
					pMouseEvent->SetMouseEventType( MouseEventType::MOUSE_MBUTTON_DOWN );
					pMouseEvent->SetMouseButton( MOUSE_MBUTTON );

					GetInstance()->mMouseEventDispatcher->SetMouseButtonState( MOUSE_MBUTTON, true );
					pMouseEvent->SetX( (int)(short)LOWORD(lParam) );
					pMouseEvent->SetY( (int)(short)HIWORD(lParam) );
				
					GetInstance()->mMouseEventList.push_front( pMouseEvent );
				} 
				break;
			}
		case WM_MBUTTONUP:
			{
				if ( GetInstance()->mMouseEventDispatcher->IsMouseButtonPressed( MOUSE_MBUTTON ) == true ) {
					GetInstance()->mMouseEventDispatcher->SetMouseButtonState( MOUSE_MBUTTON, false );
				}
				break;
			}
		case WM_RBUTTONDOWN:
			{
				if ( GetInstance()->mMouseEventDispatcher->IsMouseButtonPressed( MOUSE_RBUTTON ) == false ) {
					MouseEvent* pMouseEvent = new MouseEvent();
					pMouseEvent->SetMouseEventType( MouseEventType::MOUSE_RBUTTON_DOWN );
					pMouseEvent->SetMouseButton( MOUSE_RBUTTON );

					GetInstance()->mMouseEventDispatcher->SetMouseButtonState( MOUSE_RBUTTON, true );
					pMouseEvent->SetX( (int)(short)LOWORD(lParam) );
					pMouseEvent->SetY( (int)(short)HIWORD(lParam) );
				
					GetInstance()->mMouseEventList.push_front( pMouseEvent );
				} 
				break;
			}
		case WM_RBUTTONUP:
			{
				if ( GetInstance()->mMouseEventDispatcher->IsMouseButtonPressed( MOUSE_RBUTTON ) == true ) {
					GetInstance()->mMouseEventDispatcher->SetMouseButtonState( MOUSE_RBUTTON, false );
				}
				break;
			}
		case WM_MOUSEWHEEL:
			{
				if( (SHORT)HIWORD(wParam) > 0 ) {
					MouseEvent* pMouseEvent = new MouseEvent();
					pMouseEvent->SetMouseEventType( MouseEventType::MOUSE_WHEEL_UP );
					GetInstance()->mMouseEventList.push_front( pMouseEvent );
				}
				else {
					MouseEvent* pMouseEvent = new MouseEvent();
					pMouseEvent->SetMouseEventType( MouseEventType::MOUSE_WHEEL_DOWN );
					GetInstance()->mMouseEventList.push_front( pMouseEvent );
				}
				break;
			}

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN: {
				if ( GetInstance()->mKeyEventDispatcher->IsKeyPressed( wParam ) == false ) {
					KeyEvent* pKeyEvent = new KeyEvent();
					pKeyEvent->SetKeyEventType( KeyEventType::KEY_DOWN );
					pKeyEvent->SetKeyCode( wParam );

					GetInstance()->mKeyEventDispatcher->SetKeyPressed( wParam, true );
				
					GetInstance()->mKeyEventList.push_front( pKeyEvent );
				} 
				break;
			}
		case WM_SYSKEYUP:
		case WM_KEYUP: {
				if ( GetInstance()->mKeyEventDispatcher->IsKeyPressed( wParam ) == true ) {
					GetInstance()->mKeyEventDispatcher->SetKeyPressed( wParam, false );
				}

				break;
			}
		}
		return(DefWindowProc(hWnd,message,wParam,lParam));
	}
}