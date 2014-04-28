#pragma once
#include "SkinnedMesh.h"

namespace pooptube{
	class XMesh : public SkinnedMesh {
	public:
		XMesh();
		virtual ~XMesh();

		static std::shared_ptr<XMesh> Create(const std::string& FilePath);

		bool Init();

	private:
		D3DMATERIAL9*			mMaterial = nullptr;
	};
}



