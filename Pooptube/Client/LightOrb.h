/**
* @author ����ȯ
* @version 
* @brief
* ���������� �����ϴ� ��ü��
* @date 2014/04/29
* @file LightOrb.h
*/

// �÷��̾ ȹ���ϴ� '���� ����', ����, ���� ���� �����ϴ� Ŭ����

#pragma once
#include "stdafx.h"
#include "Node.h"
#include "SoundManager.h"
#include "SkinnedMesh.h"

namespace pooptube {
	class CollisionBox;
	class Ground;
	class XMesh;
}
class StageOne;
class IntroScene;

class LightOrb : public pooptube::Node
{
public:
	LightOrb();
	~LightOrb();

	static LightOrb *Create();

	bool Init(LightOrb *pCreature);

	void Render();
	void Update(float dTime);

	bool IsRender() { return mIsRender; }
	bool IsHealable() { return mIsHealable; }
	void SetHealable() { mIsHealable = false; }

	virtual std::vector<D3DXVECTOR3> *GetVertices() { return mMesh->GetVertices(); }
	virtual std::vector<D3DXVECTOR3> *GetIndices() { return mMesh->GetIndices(); }

//	pooptube::SkinnedMesh *GetSkinnedMesh() const { return mSkinnedMesh; }

private:
	void _CollsionHandle( pooptube::CollisionBox* collisionResult );

	//D3DXVECTOR3 mInitialPosition = { 10.f, 0.f, 0.f };


	pooptube::SkinnedMesh	*mMesh = nullptr;
	pooptube::XMesh			*mXMesh = nullptr;

//	pooptube::CollisionBox *mCollisionBox = nullptr;
	bool					mIsRender = true;
	bool					mIsHealable = true;

	FMOD::Studio::EventInstance *mEffectSound = nullptr;

	

	float					mTime = 0.f;

};