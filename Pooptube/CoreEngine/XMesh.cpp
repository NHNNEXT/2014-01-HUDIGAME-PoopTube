#include "stdafx.h"
#include "XMesh.h"

namespace pooptube {

	XMesh::XMesh() {
	}


	XMesh::~XMesh() {
	}

	std::shared_ptr<XMesh> XMesh::Create(const std::string& FilePath) {
		std::shared_ptr<XMesh> pMesh(new XMesh);
		if (pMesh->Init())
			return pMesh;
		else
			return nullptr;
	}

	bool XMesh::Init() {
		return true;
	}

}
