
#pragma once

#include "Scene.h"

namespace pooptube {
	class TestScene : public Scene {
	public:
		TestScene() {}
		virtual ~TestScene() {}

		bool Init() {
			EnableKeyEvent();
			EnableMouseEvent();
			return true; 
		}

		void Render() {
		}

		void Update(float dTime) {
		}

		void KeyDown( KeyEvent* pKeyEvent ) {
			printf( "KeyDown\n" );
		}
		void KeyPressed( KeyEvent* pKeyEvent ) {
			printf( "KeyPressed\n" );
		}
		void KeyUp( KeyEvent* pKeyEvent ) {
			printf( "KeyUp\n" );
		}

		void MouseDown( MouseEvent* pMouseEvent ) {
			printf( "MouseDown\n" );
		}

		void MouseMove( MouseEvent* pMouseEvent ) {
			printf( "MouseMove %d %d\n", pMouseEvent->GetX(), pMouseEvent->GetY() );
		}

		void MouseUp( MouseEvent* pMouseEvent ) {
			printf( "MouseUp\n" );
		}

		void MousePressed( MouseEvent* pMouseEvent ) {
			printf( "MousePressed\n" );
		}

		void MouseWheel( MouseEvent* pMouseEvent ) {
			printf( "MouseWheel\n" );
		}

		CREATE_FUNC(TestScene);
	};
}