#include "stdafx.h"
#include "SkinnedMesh.h"
#include "ObjectManager.h"
#include "resourcemanager.h"
#include "FBXMesh.h"

namespace pooptube {

	SkinnedMesh::SkinnedMesh() {
	}

	SkinnedMesh::~SkinnedMesh() {
	}

	SkinnedMesh* SkinnedMesh::Create(std::string FilePath) {
		SkinnedMesh* pMesh = new SkinnedMesh();
		if (pMesh->Init(FilePath)) {
			ObjectManager::GetInstance()->AddObject(pMesh);
		}
		else {
			delete pMesh;
			pMesh = nullptr;
		}
		return pMesh;
	}

	bool SkinnedMesh::Init(std::string FilePath) {

		mFBXMesh = ResourceManager::GetInstance()->LoadMeshFromFBX(FilePath);

		return true;
	}

	void SkinnedMesh::Render()
	{
		mFBXMesh->Render();
	}

	void SkinnedMesh::Update(float dTime)
	{

	}



}