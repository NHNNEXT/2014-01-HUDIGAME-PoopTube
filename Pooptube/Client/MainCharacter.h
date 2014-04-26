/**
* @author 양현찬
* @brief
* 메인 케릭터
* @date 2014/04/22
* @file MainCharacter.h
*/
#pragma once
#include "stdafx.h"
#include "Node.h"

namespace pooptube{
	class SkinnedMesh;
}

class MainCharacter : public pooptube::Node{
public:
	MainCharacter();
	~MainCharacter();

	static std::shared_ptr<MainCharacter> Create();

	bool Init();

	void Render();
	void Update(float dTime);

	void KeyDown(pooptube::KeyEvent* pKeyEvent);
	void KeyPressed(pooptube::KeyEvent* pKeyEvent);
	void KeyUp(pooptube::KeyEvent* pKeyEvent);
	void MouseDown(pooptube::MouseEvent* pMouseEvent);
	void MouseMove(pooptube::MouseEvent* pMouseEvent);
	void MouseUp(pooptube::MouseEvent* pMouseEvent);
	void MousePressed(pooptube::MouseEvent* pMouseEvent);
	void MouseWheel(pooptube::MouseEvent* pMouseEvent);

protected:


private:

	float					mSpeed = 0.1f;

	std::shared_ptr<pooptube::SkinnedMesh>	mSkinnedMesh = nullptr;

};

