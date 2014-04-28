/**
* @author 이선협
* @version 2014/04/29 김지환 GetCurrentScene 추가
* @brief
* @date 2014/04/22
* @file SceneManager.h
*/
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

		D3D9Renderer *GetRenderer() const { return mRenderer; }
		Scene		 *GetCurrentScene() const { return mScene; }

	private:
		D3D9Renderer* _CreateRenderer(HWND hwnd, int width, int height, bool isWindowed);

	private:
		Scene* mScene = nullptr;

		D3D9Renderer* mRenderer = nullptr;

		friend class Application;
	};
}