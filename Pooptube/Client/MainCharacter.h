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
#include "StageOne.h"
#include "..\Library\FMOD\fmod_common.h"

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

	static MainCharacter *Create();

//	bool Init();
	bool Init( MainCharacter *pMainCharacter );

	void Render();
	void Update(float dTime);

	void KeyDown(pooptube::KeyEvent* pKeyEvent);
	void KeyPressed(pooptube::KeyEvent* pKeyEvent);
	void KeyUp(pooptube::KeyEvent* pKeyEvent);
	void MouseDown(pooptube::MouseEvent* pMouseEvent);
	void MouseMove(pooptube::MouseEvent* pMouseEvent);
	void MouseUp(pooptube::MouseEvent* pMouseEvent);
	void MousePressed(pooptube::MouseEvent* pMouseEvent);
	void JumpUpdate( float dTime );

	CHAR_STATE	GetState() const { return mState; }
	void		SetState(CHAR_STATE val) { mState = val; }

	float		GetJumpSpeed() const { return mJumpSpeed; }

	void MouseWheel(pooptube::MouseEvent* pMouseEvent);

	pooptube::SkinnedMesh *GetSkinnedMesh() const { return mSkinnedMesh; }

//	virtual void CollsionReceive( std::shared_ptr<Node> target );
//	virtual void CollsionReceive( Node* target );
//	virtual void CollsionFeedBack( std::shared_ptr<Node> target );
//	virtual void CollsionFeedBack( Node* target );

protected:


private:

	CHAR_STATE				mState = NONE;
	
	float					mSpeed = 0.1f;
	float					mJumpSpeed = 10.f;

	pooptube::SkinnedMesh	*mSkinnedMesh = nullptr;
	FMOD_3D_ATTRIBUTES		mListener;
};

