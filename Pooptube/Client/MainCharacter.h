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
	class CollisionBox;
}

enum CHAR_STATE {
	NONE,
	JUMP,
};

class MainCharacter : public pooptube::Node{
public:
	MainCharacter();
	~MainCharacter();

	static std::shared_ptr<MainCharacter> Create();

	bool Init();

	void Render();
	void Update(float dTime);

<<<<<<< HEAD
	void KeyDown(pooptube::KeyEvent* pKeyEvent);
	void KeyPressed(pooptube::KeyEvent* pKeyEvent);
	void KeyUp(pooptube::KeyEvent* pKeyEvent);
	void MouseDown(pooptube::MouseEvent* pMouseEvent);
	void MouseMove(pooptube::MouseEvent* pMouseEvent);
	void MouseUp(pooptube::MouseEvent* pMouseEvent);
	void MousePressed(pooptube::MouseEvent* pMouseEvent);
||||||| merged common ancestors
	void KeyDown(pooptube::KeyEvent* pKeyEvent);
	void KeyPressed(pooptube::KeyEvent* pKeyEvent);
	void KeyUp(pooptube::KeyEvent* pKeyEvent);
	void MouseDown(pooptube::MouseEvent* pMouseEvent);
	void MouseMove(pooptube::MouseEvent* pMouseEvent);
	void MouseUp(pooptube::MouseEvent* pMouseEvent);
	void MousePressed(pooptube::MouseEvent* pMouseEvent);
=======
	CHAR_STATE	GetState() const { return mState; }
	void		SetState(CHAR_STATE val) { mState = val; }

	float		GetJumpSpeed() const { return mJumpSpeed; }

	void KeyDown(pooptube::KeyEvent* pKeyEvent);
	void KeyPressed(pooptube::KeyEvent* pKeyEvent);
	void KeyUp(pooptube::KeyEvent* pKeyEvent);
	void MouseDown(pooptube::MouseEvent* pMouseEvent);
	void MouseMove(pooptube::MouseEvent* pMouseEvent);
	void MouseUp(pooptube::MouseEvent* pMouseEvent);
	void MousePressed(pooptube::MouseEvent* pMouseEvent);
>>>>>>> deb9f8d65228cc649e04bbe1ac19a847f818407f
	void MouseWheel(pooptube::MouseEvent* pMouseEvent);

	std::shared_ptr<pooptube::SkinnedMesh> GetSkinnedMesh() const { return mSkinnedMesh; }

protected:


private:

	CHAR_STATE				mState = NONE;
	
	float					mSpeed = 0.1f;
	float					mJumpSpeed = 7.f;

	std::shared_ptr<pooptube::SkinnedMesh>	mSkinnedMesh = nullptr;
	std::shared_ptr<pooptube::CollisionBox> mCollisionBox = nullptr;
};

