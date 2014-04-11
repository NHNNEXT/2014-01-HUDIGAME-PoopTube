
#include "stdafx.h"
#include "Mesh.h"

#include "Application.h"
#include "ObjectManager.h"

namespace pooptube {
	Mesh::Mesh()
	{
	}
	Mesh::~Mesh() {
	}

	Mesh* Mesh::Create() {
		Mesh* pMesh = new Mesh();
		if ( pMesh->Init() ) {
			ObjectManager::GetInstance()->AddObject( pMesh );
		} else {
			delete pMesh;
			pMesh = nullptr;
		}
		return pMesh;
	}

	bool Mesh::Init() {
		return true;
	}

	void Mesh::Render() {
	}

	void Mesh::Update( float dTime ) {
	}
}