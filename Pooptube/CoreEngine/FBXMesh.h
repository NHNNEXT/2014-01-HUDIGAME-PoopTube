#pragma once
#include "stdafx.h"
#include "Node.h"

//������
namespace pooptube {

	class FBXMesh : public Node {
	public:
		FBXMesh();
		virtual ~FBXMesh();

		static FBXMesh *Create(const std::string& FilePath);
		bool Init(const std::string& FilePath);

		//������ ������Ʈ�� ������ ���� �𵨿� �������
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


