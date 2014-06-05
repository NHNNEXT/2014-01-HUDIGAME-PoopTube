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
#include "SoundManager.h"
#include "ResourceDef.h"

namespace pooptube{
	class SkinnedMesh;
	class CollisionBox;
	class Light;
	class Scene;
}

enum CHAR_STATE {
	NONE,
	JUMP,
	MOVE,
	RUN,
	SHAKEHAND,
};

class MainCharacter : public pooptube::Node{
public:
	MainCharacter();
	~MainCharacter();

	static MainCharacter *Create( pooptube::Scene* scene );

	bool Init( pooptube::Scene* scene );

	void Render();
	void Update(float dTime);

	void UpdateInput(float dTime);


	CHAR_STATE	GetState() const { return mState; }
	void		SetState(CHAR_STATE val) { mState = val; }

	int			GetHP() { return mHP; }
	void        SetHP(int hp) { mHP = hp; }
	void		DecreaseHP(int damage) { mHP -= damage; }
	void		InCreaseHP(int point) { mHP += point; }

	float		GetJumpSpeed() const { return mJumpSpeed; }

	pooptube::SkinnedMesh *GetSkinnedMesh() const { return mMesh; }

//	virtual void CollsionReceive( std::shared_ptr<Node> target );
//	virtual void CollsionReceive( Node* target );
//	virtual void CollsionFeedBack( std::shared_ptr<Node> target );
//	virtual void CollsionFeedBack( Node* target );

protected:


private:
	void _CollsionHandle( float dTime, pooptube::CollisionBox* collisionResult );
	void _JumpUpdate( float dTime );

	CHAR_STATE				mState = NONE;
	
	float					mSpeed = 10.f;
	float					mRotateSpeed = 3.f;
	float					mJumpSpeed = 10.f;
	int						mHP = 0;

	pooptube::SkinnedMesh	*mMesh = nullptr;
	pooptube::Light			*mLight = nullptr;
	pooptube::Scene			*mScene = nullptr;

	FMOD_3D_ATTRIBUTES		mListener;
};

