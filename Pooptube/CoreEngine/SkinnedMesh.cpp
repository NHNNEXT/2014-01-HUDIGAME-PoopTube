#include "stdafx.h"
#include "SkinnedMesh.h"
#include "ObjectManager.h"
namespace pooptube {

	SkinnedMesh::SkinnedMesh() {
	}

	SkinnedMesh::~SkinnedMesh() {
	}

	SkinnedMesh* SkinnedMesh::Create() {
		SkinnedMesh* pMesh = new SkinnedMesh();
		if (pMesh->Init()) {
			ObjectManager::GetInstance()->AddObject(pMesh);
		}
		else {
			delete pMesh;
			pMesh = nullptr;
		}
		return pMesh;
	}

	bool SkinnedMesh::Init() {
		return true;
	}

}