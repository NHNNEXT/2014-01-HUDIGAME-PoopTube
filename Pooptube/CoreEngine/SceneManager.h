
#pragma once

#include "Scene.h"
#include "D3D9Renderer.h"

namespace pooptube {
	class SceneManager : public Object {
	private:
		SceneManager();
		~SceneManager();

	public:
		bool Init( HWND hwnd, int width, int height, bool isWindowed );

		void ChangeScene( Scene* pScene );

		void Render();
		void Update( float dTime );

		D3D9Renderer* GetRenderer() const { return mRenderer; }

	private:
		bool _CreateRenderer( HWND hwnd, int width, int height, bool isWindowed );

	private:
		Scene* mScene;

		D3D9Renderer* mRenderer;

		friend class Application;
	};
}