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
#include "MainCharacter.h"
#include "StageOne.h"
#include "Ground.h"
#include "CollisionManager.h"
#include "SoundManager.h"

namespace pooptube {
	class SkinnedMesh;
	class CollisionBox;
	class Ground;
}
class StageOne;

class LightOrb : public pooptube::Node
{
public:
	LightOrb();
	~LightOrb();

	static LightOrb *Create();

	bool Init(LightOrb *pCreature);

	void Render();
	void Update(float dTime);

	pooptube::SkinnedMesh *GetSkinnedMesh() const { return mSkinnedMesh; }

private:
	void _CollsionHandle( pooptube::CollisionBox* collisionResult );

	D3DXVECTOR3 mInitialPosition = { 10.f, 0.f, 0.f };

	pooptube::SkinnedMesh	*mSkinnedMesh = nullptr;
//	pooptube::CollisionBox *mCollisionBox = nullptr;
	bool					isRender = true;
	FMOD::Studio::EventInstance *mEffectSound = nullptr;
};