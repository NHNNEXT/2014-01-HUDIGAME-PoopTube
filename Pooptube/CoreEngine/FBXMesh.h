#pragma once
#include "stdafx.h"
#include "Node.h"

//제작중
namespace pooptube {

	class FBXMesh : public Node {
	public:
		FBXMesh();
		virtual ~FBXMesh();

		static FBXMesh *Create(const std::string& FilePath);
		bool Init(const std::string& FilePath);

		//문제는 업데이트와 렌더가 나뉜 모델에 맞춰야함
		void Update(float dTime);
		void Render();

	protected:

		void _RenderFBXRecursive(FbxNode* pNode);
		void _RenderFBX(FbxNode* pNode);
		void _RenderMesh(FbxNode* pNode);
		

	private:
		FbxScene* mFbxScene = nullptr;

	};
}


