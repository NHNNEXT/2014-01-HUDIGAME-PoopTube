
#pragma once

#include "D3D9Renderer.h"

namespace pooptube {

	class Scene;

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
		D3D9Renderer* _CreateRenderer(HWND hwnd, int width, int height, bool isWindowed);

	private:
		Scene* mScene;

		D3D9Renderer* mRenderer;

		friend class Application;
	};
}